/* This work is licensed under the Creative Commons Attribution 4.0 International License.
To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/
or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA. */

/*
 *	Prosty debugger, ktory bardziej pelni role informacyjna niz funkcjonalna.
 *	Autor: Foralost (SZ. M.)
*/

#include <iostream>
#include <string>
#include <windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#define printf printf_s

#include "Debug.h"
using namespace std;

void genProcessList()
{
	char szGen[] = "Generating actual list of processes...";
	cout << szGen << endl;
	
		HANDLE HL_PrcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
		if (HL_PrcSnap == INVALID_HANDLE_VALUE)
		 {
			char szErr[] = "Failed to create snapshot";
			cout << szErr << endl;
			return;
		}
	
		char szListBegin[] = "---PID---PID PARENT---THR COUNT---.EXE---";
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
int main()
{
	genProcessList();
	Debug deb;

	{
		char szHello[] = "My simple debugger! Please type in PID of proccess you would like to debug! \nCreated for 32bit processes!\n";
		puts(szHello);
	}

	string PID;
	getline(cin, PID);

	DWORD nmb = (DWORD)atol(PID.c_str());

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nmb);

	deb.attach(hProcess, nmb);

	if(!hProcess)
		CloseHandle(hProcess);

	system("pause");
	return 0;
}