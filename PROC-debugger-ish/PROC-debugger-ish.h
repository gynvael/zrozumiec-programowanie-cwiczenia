#pragma once
/* This work is licensed under the Creative Commons Attribution 4.0 International License.
To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/
or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA. */
// Autor: Foralost (SZ. M.)
// Obiekt Debug pelniacy role debuggera
class Debug {

	DWORD dwPID = -1;
	DWORD* dwThreadPID = 0;
	HANDLE hProcess = INVALID_HANDLE_VALUE;

	bool bAttached = false;
	bool bStarted = false;

	void DebuggerLoop();

	DEBUG_EVENT EV;
	DWORD PrintEx();
	DWORD OnCreateThread(HANDLE hThread);
	DWORD OnCreateProcess();
	DWORD OnLoadDLL();
	DWORD OnUnloadDLL();
	DWORD OnExitThread();
	DWORD OnExitProcess();
	DWORD OnOutputString();
	DWORD OnRipEvent();
public:
	bool attach(HANDLE& hProcess, DWORD& PID);
	bool start();
};

bool Debug::start()
{
	char szStart[] = "Preparing to launch debugger with given PID: %u \n";
	printf(szStart, dwPID);

	if (!bAttached)
	{
		char szErrorAT[] = "Debugger nie zostal prawidlowo podpiety!";
		puts(szErrorAT);
		return false;
	}

	//DEBUGGER STARTS RIGHT HERE
	if (!DebugActiveProcess(dwPID))
	{
		char szErrorSTR[] = "Couldnt launch debugger!";
		puts(szErrorSTR);
		return false;
	}

	bStarted = true;
	DebuggerLoop();
	return true;
}
bool Debug::attach(HANDLE& hProcess, DWORD& PID)
{
	char szAttach[] = "Preparing to attach debugger with given PID: %u \n";
	char szErrorDBG[] = "Failed to check if there is already attached debugger!";
	char szErrorALR[] = "There is already a debugger attached to this process!";
	char szErrorPROC[] = "Invalid process!";

	printf(szAttach, PID);
	if (!hProcess)
	{
		puts(szErrorPROC);
		return false;
	}


	PBOOL pBOOL = new BOOL;
	if (!CheckRemoteDebuggerPresent(hProcess, pBOOL))
	{
		puts(szErrorDBG);
		return false;
	}

	if (*pBOOL)
	{
		puts(szErrorALR);
		return false;
	}

	bAttached = true;
	dwPID = PID;
	Debug::hProcess = hProcess;
	start();
	delete pBOOL;
	return true;
}


// --------------------------------------
DWORD Debug::PrintEx()
{
	char szLog[] = "[INF] Last exception address: %p\n";
	printf(szLog, EV.u.Exception.ExceptionRecord.ExceptionAddress);

	HANDLE Thread = OpenThread(THREAD_ALL_ACCESS, FALSE, EV.dwThreadId);
	if (!Thread)
	{
		char szError[] = "[ERR] Error during opening debugged thread!";
		puts(szError);
		return ERROR_DBG_CONTINUE;
	}

	CONTEXT ThreadCTX;
	ThreadCTX.ContextFlags = CONTEXT_FULL;

	SuspendThread(Thread);
	GetThreadContext(Thread, &ThreadCTX);

	char szListReg[] = "[INF] List of registers:\neax:\t%x\nebx:\t%x\necx:\t%x\nedx:\t%x\nesp:\t%x\neip:\t%x\nesi:\t%x\n";

	printf(szListReg, ThreadCTX.Eax, ThreadCTX.Ebx, ThreadCTX.Ecx, ThreadCTX.Edx, ThreadCTX.Esp, ThreadCTX.Eip, ThreadCTX.Esi);
	ResumeThread(Thread);
	return 0;
}
// --------------------------------------
DWORD Debug::OnCreateThread(HANDLE hThread = INVALID_HANDLE_VALUE)
{
	char szListReg[] = "[INF] List of created thread registers:\neax:\t%x\nebx:\t%x\necx:\t%x\nedx:\t%x\nesp:\t%x\neip:\t%x\nesi:\t%x\n";
	char szThrCrt[] = "[EVENT] Thread created!";
	char szThrPID[] = "[INF] Thread ID: %u \n";
	char szInfoThread[] = "[INF] Thread start address: %p\n";
	HANDLE DbgThread;

	if (hThread == INVALID_HANDLE_VALUE) {
		DbgThread = EV.u.CreateThread.hThread;
		puts(szThrCrt);
		printf(szInfoThread, EV.u.CreateThread.lpStartAddress);
	}
	else {
		char szThrUSD[] = "[INF] Using this function to print info about other thread!";
		puts(szThrUSD);
		if (hThread) 
			DbgThread = hThread;
		else
		{
			char szThrWRG[] = "[INF] The given thread is broken!";
			puts(szThrWRG);
			return ERROR_DBG_CONTINUE;
		}
	}

	DWORD dwThreadID = GetThreadId(DbgThread);
	printf(szThrPID, dwThreadID);

	if (SuspendThread(DbgThread) == (DWORD)-1)
	{
		char szError[] = "[ERR] Failed to suspend thread!";
		printf(szError);
		return ERROR_DBG_CONTINUE;
	}

	CONTEXT ThreadCTX;
	ThreadCTX.ContextFlags = CONTEXT_FULL;

	if(GetThreadContext(DbgThread, &ThreadCTX))
		printf(szListReg, ThreadCTX.Eax, ThreadCTX.Ebx, ThreadCTX.Ecx, ThreadCTX.Edx, ThreadCTX.Esp, ThreadCTX.Eip, ThreadCTX.Esi);
	else
	{
		char szError[] = "[ERR] Failed to get thread context!";
		puts(szError);
	}

	while(ResumeThread(DbgThread) == (DWORD)-1)
	{
		char szError[] = "[ERR] Failed to resume thread!\n[Q] Kill it or try again? y/n";
		puts(szError);
		char szForm[] = "%c";
		char cDec;
		scanf_s(szForm, &cDec);

		if (cDec == 'y')
		{
			size_t count = 0;
			char szErrorTHR[] = "[ERR] Failed to kill the thread! Tryin again...";
			while (!TerminateThread(DbgThread, ERROR_DBG_TERMINATE_THREAD)) {
				puts(szErrorTHR);
				++count;
				if (count == 20)
				{
					char szError10[] = "[ERR] Failed to kill thread after 20 tries! Returning fatal error!";
					return ERROR_DBG_EXCEPTION_NOT_HANDLED;
				}
				else
					return ERROR_DBG_TERMINATE_THREAD;
			}
		}
	}

	return DBG_CONTINUE;
}
// --------------------------------------
DWORD Debug::OnExitThread()
{
	char szThrEx[] = "[EVENT] Thread exited!";
	char szInfoThread[] = "[INF] Thread exit code: %x\n";
	DWORD DbgThread = EV.u.ExitThread.dwExitCode;

	puts(szThrEx);
	printf(szInfoThread, EV.u.ExitThread.dwExitCode);

	return DBG_CONTINUE;
}
// --------------------------------------
DWORD Debug::OnCreateProcess()
{
	char szImageName[] = "[INF] Address to child process image name: %p \n";
	char szBaseImageAddr[] = "[INF] Base executable address: %p \n";
	char szThreadAddress[] = "[INF] Child process thread address: %p \n";
	char szProcCRT[] = "[EVENT] Process Created!";
	char szProcINF[] = "[INF] Pending process info:";
	char szProcINFTHR[] = "[INF] Pending process initial thread info:";
	HANDLE hProcess = EV.u.CreateProcessInfo.hProcess;

	puts(szProcINF);
	printf(szImageName, EV.u.CreateProcessInfo.lpImageName);
	printf(szThreadAddress, EV.u.CreateProcessInfo.lpStartAddress);
	printf(szBaseImageAddr, EV.u.CreateProcessInfo.lpBaseOfImage);

	DWORD childPID;
	childPID = GetProcessId(EV.u.CreateProcessInfo.hProcess);

	char szChildPID[] = "[INF] Child Process PID: %u\n";
	printf(szChildPID, childPID);

	puts(szProcINFTHR);

	OnCreateThread(EV.u.CreateProcessInfo.hThread);
	CloseHandle(EV.u.CreateProcessInfo.hFile);
	return DBG_CONTINUE;
}
// --------------------------------------
DWORD Debug::OnLoadDLL()
{
	char szLoadDLL[] = "[EVENT] Loading a DLL!";
	char szName[] = "[INF] Address to DLLs Name: %p \n";
	char szAddr[] = "[INF] DLLs base address: %p \n";
	puts(szLoadDLL);
	printf(szName, EV.u.LoadDll.lpImageName);
	printf(szAddr, EV.u.LoadDll.lpBaseOfDll);
	CloseHandle(EV.u.LoadDll.hFile);
	return DBG_CONTINUE;

}
// --------------------------------------
DWORD Debug::OnUnloadDLL()
{
	char szUnLoadDLL[] = "[EVENT] UnLoading a DLL!";
	char szAddr[] = "[INF] DLLs base address: %p \n";

	puts(szUnLoadDLL);
	printf(szAddr, EV.u.UnloadDll.lpBaseOfDll);

	return DBG_CONTINUE;
}
// --------------------------------------
DWORD Debug::OnExitProcess()
{
	char szExitCode[] = "[EVENT] Process Exiting!\n[INF] Process code: %u\n";
	printf(szExitCode, EV.u.ExitProcess.dwExitCode);
	return DBG_CONTINUE;
}
// --------------------------------------
DWORD Debug::OnOutputString()
{

	char szOutputString[] = "[EVENT] Output string!";
	char szInfo[] = "[INF] String Data: %s \n";
	puts(szOutputString);

	printf(szInfo, EV.u.DebugString.lpDebugStringData);
	
	return DBG_CONTINUE;
}
// --------------------------------------
DWORD Debug::OnRipEvent()
{
	char szRIP[] = "[EVENT] Rip Event Occurred!";
	puts(szRIP);
	char szDwError[] = "[INF] DwError value: %i \n";
	char szDwType[] = "[INF] DwType indicates:";
	char szINVData[] = "Invalid data was passed to function and caused programs failure.";
	char szNINVData[] = "Invalid data was passed to function and probably will not cause program to fail.";
	char szSUCCData[] = "Invalid data was passed to function however function succeed.";

	printf(szDwError, EV.u.RipInfo.dwError);
	if (!EV.u.RipInfo.dwType) {
		puts(szDwType);
		switch (EV.u.RipInfo.dwType)
		{
		case 1:
			puts(szINVData);
				break;
		case 2:
			puts(szNINVData);
			break;
		case 3:
			puts(szSUCCData);
			break;
		}
	}
	return DBG_CONTINUE;
}
// --------------------------------------
void Debug::DebuggerLoop()
{
	DebugSetProcessKillOnExit(FALSE);
	DWORD dwContinueStat = DBG_CONTINUE;
	char szBarST[] = "\n//|-----------------------------|\\\\";
	char szBarEND[] = "\\\\|-----------------------------|//\n";

	char szExcTypeV[] = "[EXC] EXCEPTION_ACCESS_VIOLATON";
	char szExcTypeBK[] = "[EXC] EXCEPTION_BREAKPOINT";
	char szExcTypeOB[] = "[EXC] EXCEPTION_OUT_OF_BOUNDS";
	char szExcTypeSO[] = "[EXC] EXCEPTION_STACK_OVERLFOW";
	char szExcTypeSP[] = "[EXC] EXCEPTION_SINGLE_STEP";
	char szExcTypeDZ[] = "[EXC] EXCEPTION_INT_DIV_ZERO";

	char szExc[] = "[EXC] Exception caught!\n[EXC] Exception type:";

	while (bAttached && bStarted)
	{
		WaitForDebugEvent(&EV, INFINITE);
		puts(szBarST);
		switch (EV.dwDebugEventCode)
		{

		case EXCEPTION_DEBUG_EVENT:
			switch (EV.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				puts(szExc);
				puts(szExcTypeOB);
				PrintEx();
				break;

			case EXCEPTION_SINGLE_STEP:
				puts(szExc);
				puts(szExcTypeSP);
				PrintEx();
				break;

			case EXCEPTION_STACK_OVERFLOW:
				puts(szExc);
				puts(szExcTypeSO);
				PrintEx();
				break;

			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				puts(szExc);
				puts(szExcTypeDZ);
				PrintEx();
				break;

			case EXCEPTION_ACCESS_VIOLATION:
				puts(szExc);
				puts(szExcTypeV);
				PrintEx();
			break;

			case EXCEPTION_BREAKPOINT:
				puts(szExc);
				puts(szExcTypeBK);
				PrintEx();
				break;

			case DBG_CONTROL_C:
				bStarted = false;
			break;

				default:
					break;
			}
		break;
		
		case CREATE_THREAD_DEBUG_EVENT:
			dwContinueStat = OnCreateThread();
		break;

		case EXIT_THREAD_DEBUG_EVENT:
			dwContinueStat = OnExitThread();
		break;

		case CREATE_PROCESS_DEBUG_EVENT:
			dwContinueStat = OnCreateProcess();
		break;

		case EXIT_PROCESS_DEBUG_EVENT:
			dwContinueStat = OnExitProcess();
		break;

		case LOAD_DLL_DEBUG_EVENT:
			dwContinueStat = OnLoadDLL();
		break;

		case UNLOAD_DLL_DEBUG_EVENT:
			dwContinueStat = OnUnloadDLL();
		break;
		case RIP_EVENT:
			dwContinueStat = OnRipEvent();
			break;
		default:
			break;
		}
		puts(szBarEND);
		ContinueDebugEvent(EV.dwProcessId, EV.dwThreadId, dwContinueStat);
	}

	DebugActiveProcessStop(dwPID);
	return;
}