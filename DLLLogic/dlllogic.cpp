#include "stdafx.h"

#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

#define ALTERNATIVE_REFRESH 144

BOOL WINAPI HookEnumDisplaySettingsW(LPCWSTR lpszDeviceName, DWORD iModeNum, DEVMODEW* lpDevMode){
	BOOL setFlag = FALSE;

	cout << "Call to HookEnumDisplaySettingsW with struct: 0x" << &lpDevMode << "\n";

	if (EnumDisplaySettingsW(lpszDeviceName, iModeNum, lpDevMode)) {
		setFlag = TRUE;
	}

	lpDevMode->dmDisplayFrequency = 144;

	return setFlag;
}

BOOL WINAPI HookGetDeviceCaps(HDC hdc,int index) {

	cout << "Call to GetDeviceCaps\n";

	if (index == VREFRESH) {
		return ALTERNATIVE_REFRESH;
	}
	else {
		return GetDeviceCaps(hdc, index);
	}
}

extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo){

	cout << "Injected by PID " << inRemoteInfo->HostPID << "\n";
	cout << "Size of Data Argument: " << inRemoteInfo->UserDataSize << "\n";

	HOOK_TRACE_INFO hooktrace_0 = { NULL };
	HOOK_TRACE_INFO hooktrace_1 = { NULL };

	cout << "\n" << "Success: EnumWindowSettingsW found at address: 0x" << GetProcAddress(GetModuleHandle(L"user32"), "EnumDisplaySettingsW") << "\n";
	cout << "\n" << "Success: GetDeviceCaps found at address: 0x" << GetProcAddress(GetModuleHandle(L"gdi32"), "GetDeviceCaps") << "\n";

	NTSTATUS result1 = LhInstallHook(
		GetProcAddress(GetModuleHandle(L"user32"), "EnumDisplaySettingsW"),
		HookEnumDisplaySettingsW,
		0,
		&hooktrace_0);

	NTSTATUS result2 = LhInstallHook(
		GetProcAddress(GetModuleHandle(L"gdi32"), "GetDeviceCaps"),
		HookGetDeviceCaps,
		0,
		&hooktrace_1);

	if (FAILED(result1)) {
		cout << "Failure: Hook for 'EnumDisplaySettingsW': " << RtlGetLastErrorString() << "\n";
	}
	else {
		cout << "Success: Hook for 'EnumDisplaySettingsW' installed successfully.\n";
	}

	if (FAILED(result2)) {
		cout << "Failure: Hook for 'GetDeviceCaps': " << RtlGetLastErrorString() << "\n";
	}
	else {
		cout << "Success: Hook for 'GetDeviceCaps' installed successfully.\n";
	}

	ULONG ACLEntries[1] = { 0 };
	LhSetExclusiveACL(ACLEntries, 1, &hooktrace_0);
	LhSetExclusiveACL(ACLEntries, 1, &hooktrace_1);

	return;
}