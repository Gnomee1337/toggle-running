#ifndef KEYSIMULATOR_H
#define KEYSIMULATOR_H

#include <windows.h>

#include "OverlayManager.h"

class KeySimulator
{
public:
	KeySimulator(OverlayManager* overlay);

	void toggleWSimulation(HWND gameWindow);

private:
	bool isRunning;
	bool isF3Pressed;
	OverlayManager* overlayManager;

	void simulateKeyPress(WORD key, bool press);
};

#endif // KEYSIMULATOR_H