#include "stdafx.h"

#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>

#include <easyhook.h>

constexpr auto PATH_01 = L"C:\\files\\tl2\\tl2p.exe";

using namespace std;

int _tmain(int argc, _TCHAR* argv[]){
	BOOL exitFlag = FALSE;
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	int* errorCode = 0;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	while (!exitFlag) {
		cout << "Begin Execution?\n";
		_getch();
		
		if (!CreateProcessW(PATH_01, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi)) {
			cout << "Failure: Initialization\n";
			cout << "Error: " << GetLastError() << "\n";
		}

		DWORD processID = pi.dwProcessId;
		//cout << "Enter the target PID: ";
		//cin >> processID;
		
		Sleep(1000);

		GetExitCodeProcess(pi.hThread, (LPDWORD)*errorCode);

		cout << "ECODE: " << errorCode << "\n";

		WCHAR* TargetDLL = L"DLLLogic.dll";
		cout << "Injecting " << &TargetDLL << " into PID " << processID << "\n";

		NTSTATUS InjectState = RhInjectLibrary(processID, 0, EASYHOOK_INJECT_DEFAULT, TargetDLL, NULL, NULL, 0);

		if (InjectState != 0){
			cout << "Failure: RhInjectLibrary Error Code " << InjectState << "\n";
			PWCHAR s(RtlGetLastErrorString());
			cout << s << "\n";
		}
		else{
			cout << L"Success: Library Injected.\n";
		}

		cout << "\n" << "Press Any Key to Continue. Press X to Exit.\n\n";

		_getch(); //prevent autoexit
		if (GetKeyState(88) != false) {
			exitFlag = TRUE;
		}
	}

	/*WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	*/
	return 0;
}