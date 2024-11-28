#include <windows.h>
#include <iostream>
#include "KeySimulator.h"
#include "OverlayManager.h"
#include "utils.h"

int main() {
	const std::wstring exeName = L"Stalker2-Win64-Shipping.exe"; // Your target exe name

	// Hide the console window
	HWND hwndConsole = GetConsoleWindow();
	ShowWindow(hwndConsole, SW_HIDE);

	if (!IsRunningAsAdmin()) {
		std::cout << "Running NOT as an Administrator!" << std::endl;
		MessageBoxW(NULL, L"Running NOT as an Administrator!", L"Error", MB_ICONERROR);
		return -1;
	}

	DWORD processId = FindProcessIdByExeName(exeName);
	if (processId == 0) {
		std::cout << "Target process not found!" << std::endl;
		MessageBoxW(NULL, L"Target process not found!", L"Error", MB_ICONERROR);
		return -1;
	}

	HWND gameWindow = FindMainWindowByProcessId(processId);
	if (!gameWindow) {
		std::cout << "Game window not found!" << std::endl;
		MessageBoxW(NULL, L"Game window not found!", L"Error", MB_ICONERROR);
		return -1;
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);
	OverlayManager overlayManager;
	overlayManager.createOverlayWindow(hInstance, gameWindow);

	KeySimulator keySimulator(&overlayManager);

	// Main loop
	MSG msg;
	while (true) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				return 0;
			}
		}
		keySimulator.toggleWSimulation(gameWindow);
		Sleep(50);
	}

	return 0;
}
