#include <iostream>

#include "OverlayManager.h"

OverlayManager::OverlayManager() : overlayWindow(nullptr), currentText(L"Press F3") {}

OverlayManager::~OverlayManager() {
	if (overlayWindow) {
		DestroyWindow(overlayWindow);
	}
}

void OverlayManager::createOverlayWindow(HINSTANCE hInstance, HWND targetWindow) {
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"OverlayClass";

	RegisterClass(&wc);

	RECT rect;
	GetWindowRect(targetWindow, &rect);

	overlayWindow = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, // Extended styles
		L"OverlayClass",                                   // Class name
		L"Overlay",                                        // Window name
		WS_POPUP,                                          // No border or title bar
		rect.left, rect.top, 200, 50,                      // Position and size
		NULL, NULL, hInstance, NULL
	);

	// Store the OverlayManager instance in the window's user data
	SetWindowLongPtr(overlayWindow, GWLP_USERDATA, (LONG_PTR)this);

	// Make the overlay transparent
	SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 0, LWA_COLORKEY);
	ShowWindow(overlayWindow, SW_SHOW);

	std::cout << "Overlay created." << std::endl;
}

void OverlayManager::updateOverlayText(const std::wstring& text) {
	currentText = text;
	InvalidateRect(overlayWindow, NULL, TRUE);
}

void OverlayManager::updateOverlayPosition(HWND targetWindow) {
	RECT rect;
	GetWindowRect(targetWindow, &rect);
	SetWindowPos(overlayWindow, HWND_TOPMOST, rect.left, rect.top, 200, 50, SWP_NOSIZE | SWP_NOACTIVATE);
}

// Window procedure for overlay window to draw status text
LRESULT CALLBACK OverlayManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Retrieve the OverlayManager instance from the window's user data
	OverlayManager* overlayManager = reinterpret_cast<OverlayManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (uMsg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// Fill background with transparent color
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &ps.rcPaint, brush);
		DeleteObject(brush);

		// Set text color and transparency
		SetTextColor(hdc, RGB(255, 255, 255)); // White text
		SetBkMode(hdc, TRANSPARENT); // Transparent background

		// Update text based on the global state
		std::wstring text = overlayManager ? overlayManager->currentText : L"Press F3";
		TextOut(hdc, 10, 10, text.c_str(), text.length());

		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}