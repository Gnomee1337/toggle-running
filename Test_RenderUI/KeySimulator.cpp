#include <windows.h>
#include <iostream>
#include <mmsystem.h>

#include "resource.h"
#include "KeySimulator.h"

#pragma comment(lib, "winmm.lib") // Link against the Windows multimedia library

KeySimulator::KeySimulator(OverlayManager* overlay) : isRunning(false), isF3Pressed(false), overlayManager(overlay) {}

void KeySimulator::toggleWSimulation(HWND gameWindow) {
	if (GetAsyncKeyState(VK_F3) & 0x8000) {  // Check if F3 is pressed
		if (!isF3Pressed) { // Only toggle on first press
			isF3Pressed = true;

			if (isRunning) {
				// Stop running by releasing "W"
				std::cout << "Stopping movement (Releasing W)" << std::endl;
				// Play "start.wav" when turning off
				PlaySound(MAKEINTRESOURCE(BYE_WAVE), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
				simulateKeyPress('W', false);  // Release 'W'
				overlayManager->updateOverlayText(L"Running Off");
				isRunning = false;
			}
			else {
				// Start running by holding "W"
				std::cout << "Starting movement (Pressing W)" << std::endl;
				// Play "start.wav" when turning on
				PlaySound(MAKEINTRESOURCE(HELLO_WAVE), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
				simulateKeyPress('W', true);  // Press 'W'
				overlayManager->updateOverlayText(L"Running On");
				isRunning = true;
			}
		}
	}
	else {
		isF3Pressed = false;
	}

	overlayManager->updateOverlayPosition(gameWindow);
}


void KeySimulator::simulateKeyPress(WORD key, bool press) {
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	input.ki.dwFlags = press ? 0 : KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

