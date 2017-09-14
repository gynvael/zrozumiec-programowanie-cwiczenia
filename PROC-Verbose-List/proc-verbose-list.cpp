/* This work is licensed under the Creative Commons Attribution 4.0 International License.
To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/
or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA. */


#define _WIN32_WINNT 0x6000

#include <iostream>			
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
using namespace std;

/* Program ten sluzy do wyswietlenia pewnych informacji nt procesow dzialajacych pod Windowsem.
 * Informacje jakie pobiera: 
 *		-> Podstawowe informacje (ilosc watkow, pid, pid rodzica, exec wykonawczy)
 *		-> ilosc zuzytych cyklow procesora
 *		-> czas trwania procesu
 *		-> czy proces jest debugowany
 *		-> czas procesu w trybie uzytkownika / kernela
 *		-> czy proces jest 64bitowy
 *		-> dane zuzycia pamieci przez proces
 *		Autor: Foralost (SZ. M.)
 */

#include <strsafe.h>

namespace process
{
	

	void GetRAMUsage(HANDLE& HL_Prc)
	{
		PROCESS_MEMORY_COUNTERS PRC_MEM;
		PRC_MEM.cb = sizeof(PROCESS_MEMORY_COUNTERS);
		if (!HL_Prc || !GetProcessMemoryInfo(HL_Prc, &PRC_MEM, PRC_MEM.cb))
		{
			char szError[] = "Blad podczas pobierania informacji nt uzycia pamieci przez proces";
			cout << szError << endl;
			return;
		}

		char szInfo[] = "Wypisywanie informacji nt pamieci uzytej przez proces: ";
		char szDet[] = "Bledy stronnicowania: %u \nSzczyt zuzycia pamieci: %u\nAktualne zuzycie pamieci: %u\nSzczyt puli stronicowanej\
: %u\nAktualna pula stronicowana: %u \nSzczyt puli NIEstronicowanej: %u\nAktualna pula NIEstronicowana: %u\nZuzycie puli stronicowanej: %u\n\
Szczyt zuzycia puli stronicowanej: %u\n\nWszystkie dane podane sa w bajtach.\n";

		printf(szDet, PRC_MEM.PageFaultCount, PRC_MEM.PeakWorkingSetSize, PRC_MEM.WorkingSetSize,
			PRC_MEM.QuotaPeakPagedPoolUsage, PRC_MEM.QuotaPagedPoolUsage, PRC_MEM.QuotaPeakNonPagedPoolUsage,
			PRC_MEM.QuotaNonPagedPoolUsage, PRC_MEM.PagefileUsage, PRC_MEM.PeakPagefileUsage);

	}
	void check64(HANDLE& HL_Prc, BOOL* Result) {
		if (!IsWow64Process(HL_Prc, Result)) {
			char szErr[] = "Nie udalo sie uzyskac informacji nt tego czy dany process jest procesem 64 bitowym.";
			cout << szErr << endl;
		}
		else {
			char szIs64Bit[] = "Proces 64 bitowy: ";
			cout << szIs64Bit;
			if (*Result) {
				char szYes[] = "tak";
				cout << szYes << endl;
			}
			else {
				char szNo[] = "nie";
				cout << szNo << endl;
			}
		}
	}
	void GetIOCounters(HANDLE& HL_Prc)
	{
		if (!HL_Prc)
		{
			char szErr[] = "Nie udalo sie uzyskac informacji nt ilosci operacji wykonanych przez proces.";
			cout << szErr << endl;
			return;
		}
		_IO_COUNTERS Counters;
		
		char szCounters[] = "Wypisywanie ilosci wykonanych operacji przez proces typu Wejscie/Wyjscie oraz ilosci wykonanych bajtow na kazda operacje.";
		cout << szCounters << endl;
		GetProcessIoCounters(HL_Prc, &Counters);

		char szReadOp[] = "Ilosc operacji typu odczyt: %u \n";
		printf(szReadOp, Counters.ReadOperationCount);

		char szWriteOp[] = "Ilosc operacji typu zapis: %u \n";
		printf(szWriteOp, Counters.WriteOperationCount);

		char szOtherOp[] = "Ilosc operacji typu inne: %u \n";
		printf(szOtherOp, Counters.OtherOperationCount);

		char szBReadOp[] = "Ilosc bajtow typu odczyt: %u \n";
		printf(szBReadOp, Counters.ReadTransferCount);

		char szBWriteOp[] = "Ilosc bajtow typu zapis: %u \n";
		printf(szBWriteOp, Counters.WriteTransferCount);

		char szBOtherOp[] = "Ilosc bajtow typu inne: %u \n";
		printf(szBOtherOp, Counters.OtherTransferCount);
	}
	void checkDebug(HANDLE& HL_Prc, BOOL* Result) {

		if (!CheckRemoteDebuggerPresent(HL_Prc, Result) || !HL_Prc) {
			char szErr[] = "Nie udalo sie uzyskac informacji nt tego czy dany process jest debugowany.";
			cout << szErr << endl;
		}
		else {
			char szIsDebug[] = "Proces jest debugowany: ";
			cout << szIsDebug;
			if (*Result) {
				char szYes[] = "tak";
				cout << szYes << endl;
			}
			else {
				char szNo[] = "nie";
				cout << szNo << endl;
			}
		}
	}
	void processTimes(HANDLE& HL_Prc)
	{

		ULONG64 PRC_CPU = 0;
		char szCountCPU[] = "Ilosc cykli procesora uzyte przez ten proces: %u \n";
		
		if (!QueryProcessCycleTime(HL_Prc, &PRC_CPU) || !HL_Prc)
		{
			char szErrorCPU[] = "Nie udalo sie pobrac ilosci cykli procesora, ktore zostaly uzyte od powstania procesu.";
			cout << szErrorCPU << endl;
			return;
		}
		printf(szCountCPU, PRC_CPU);

		// ---
		FILETIME PRC[4];

		SYSTEMTIME PRC_ST[4];
		SYSTEMTIME PRC_ST_LOC;

		if (!GetProcessTimes(HL_Prc, &PRC[0], &PRC[1], &PRC[2], &PRC[3]))
		{
			char szError[] = "Blad podczas wyzyskiwania czasow procesu!";
			cout << szError << endl;
			return;
		}

		DWORD res;
		char buff[4][32];

		FileTimeToSystemTime(&PRC[0], &PRC_ST[0]);
		SystemTimeToTzSpecificLocalTime(NULL, &PRC_ST[0], &PRC_ST_LOC);
		res = StringCchPrintf(buff[0], 32, "%d/%02d/%02d  %02d:%02d:%02d", PRC_ST_LOC.wYear, PRC_ST_LOC.wMonth, PRC_ST_LOC.wDay,
			PRC_ST_LOC.wHour, PRC_ST_LOC.wMinute, PRC_ST_LOC.wSecond);

		if (res != S_OK)
		{
			char szError[] = "Blad podczas konwersji SYSTEMTIME utworzenia procesu do ciagu znakow.";
			cout << szError << endl;
				return;
		}

		for (int i = 2; i < 4; i++)
		{
			FileTimeToSystemTime(&PRC[i], &PRC_ST[i]);
			res = StringCchPrintf(buff[i], 32, "%02d:%02d:%02d",
				PRC_ST[i].wHour, PRC_ST[i].wMinute, PRC_ST[i].wSecond);

			if (res != S_OK)
			{
				char szError[] = "Blad podczas konwersji SYSTEMTIME do ciagu znakow.";
				cout << szError << endl;
				return;
			}
		}

		char szStart[] = "Data rozpoczecia procesu: ";
		cout << szStart << buff[0] << endl;

		char szUser[] = "Czas w trybie uzytkownika (HH:MM:SS) : ";
		cout << szUser << buff[2] << endl;

		char szKernel[] = "Czas w trybie kernela (HH:MM:SS) : ";
		cout << szKernel << buff[3] << endl;
	}

	bool TryToRetDetails(HANDLE& HL_Prc)
	{
		char szBar[] = "---------------------------------";
		PBOOL Infos = new BOOL[2]();

		if (!HL_Prc)
		{
			char szErr[] = "Nie udalo sie pobrac informacji o danym procesie.";
			cout << szErr << endl;
			return false;
		}
		cout << szBar << endl;

		{
			#define SIZE 128
			DWORD numb = SIZE;
			char szProcName[SIZE];
			if (!QueryFullProcessImageName(HL_Prc, 0, szProcName, &numb))
			{
				char szErrName[] = "Nie udalo sie pozyskac adresu wywolania.";
				cout << szErrName << GetLastError() << endl;
			}
			else {
				char szShowingInfo[] = "Wyswietlanie informacji dla procesu PID: %u, wywolany z: \n %s \n";
				printf(szShowingInfo, GetProcessId(HL_Prc), szProcName);
			}
		}

		cout << szBar << endl;
		check64(HL_Prc, &Infos[0]);
		cout << szBar << endl;
		GetIOCounters(HL_Prc);
		cout << szBar << endl;
		checkDebug(HL_Prc, &Infos[1]);
		cout << szBar << endl;
		processTimes(HL_Prc);
		cout << szBar << endl;
		GetRAMUsage(HL_Prc);
		cout << szBar << endl;
		delete[] Infos;

		return true;
	}
};

namespace commandline {

	void getpid()
	{
		string PID;
		char szInvite[] = "Podaj nr PID procesu aby uzyskac dodatkowe informacje o nim.";
		char szErrPID[] = "Podano nieprawidlowy PID.";

		cout << szInvite << endl;

		cin >> PID;

		DWORD temp = (DWORD)atol(PID.c_str());

		if (!temp)
		{
			cout << szErrPID << endl;
			return;
		}

		HANDLE Hl_Process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE,
			temp);

		if (!Hl_Process)
		{
			Hl_Process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE,
				temp);

			if (!Hl_Process) {
				cout << szErrPID << endl;
				return;
			}
		}

		process::TryToRetDetails(Hl_Process);
		CloseHandle(Hl_Process);
	}
	
	void genProcessList()
	{
		char szGen[] = "Generowanie listy aktualnych procesow...";
		cout << szGen << endl;

		HANDLE HL_PrcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (HL_PrcSnap == INVALID_HANDLE_VALUE)
		{
			char szErr[] = "Nie uda³o sie utworzenie HL_PrcSnap";
			cout << szErr << endl;
			return;
		}

		char szListBegin[] = "---PID---PID RODZICA---ILOSC WATKOW---.EXE---";
		cout << szListBegin << endl;

		PROCESSENTRY32 PrcEntr32;
		PrcEntr32.dwSize = sizeof(PROCESSENTRY32);

		BOOL PrcRes = Process32First(HL_PrcSnap, &PrcEntr32);
		char printfmsg[] = "%6u\t %12u \t %12u \t %-20s \n";

		while (PrcRes)
		{
			printf(printfmsg, PrcEntr32.th32ProcessID,
				PrcEntr32.th32ParentProcessID, PrcEntr32.cntThreads, PrcEntr32.szExeFile);

			PrcRes = Process32Next(HL_PrcSnap, &PrcEntr32);
		}

		CloseHandle(HL_PrcSnap);
		return;
	}

	void resolveCommand()
	{
		
		string command;
		getline(cin,command);

		if (!strcmp("help", command.c_str()) || !strcmp("/?", command.c_str())) {
			char szHelp[] = "####################################### \n exit - wyjscie \n details - wypisz wiecej informacji \
dla podanego PID \n list - wygeneruj liste procesow \n cls - systemowy czysciciel konsoli \n####################################### ";
			cout << szHelp << endl;
			return;
		}

		if (!strcmp("exit", command.c_str())) {
			exit(0);
			return;
		}

		if (!strcmp("cls", command.c_str())) {
			system("cls");
			return;
		}
		if (!strcmp("details", command.c_str())) {
			getpid();
			return;
		}

		if (!strcmp("list", command.c_str()))
			genProcessList();

	}
	
}
int main()
{
	commandline::genProcessList();

	while(true)
		commandline::resolveCommand();
	return 0;
}