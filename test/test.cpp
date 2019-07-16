#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[]){
	string value;
	DEVMODEW dMode = { sizeof(DEVMODE) };
	HDC dc = GetDC(NULL);
	BOOL exitFlag = FALSE;

	while (!exitFlag){
		HANDLE currentThread = GetCurrentThread();
		cout << "Target PID: " << GetProcessIdOfThread(currentThread) << "\n";
		CloseHandle(currentThread);

		EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &dMode);

		cout << "Refresh Rate: " << (int)dMode.dmDisplayFrequency << "\n";
		cout << "VRefresh: " << GetDeviceCaps(dc, VREFRESH) << "\n";

		cout << "Press Any Key to Continue. Press X to Exit.\n\n";

		wchar_t stop = _getch(); //prevent autoexit
		if (GetKeyState(88) != false) {
			exitFlag = TRUE;
		}
	}

	return 0;
}

