#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include <windows.h>
#include <string>

class OverlayManager
{
public:
	OverlayManager();
	~OverlayManager();

	void createOverlayWindow(HINSTANCE hInstance, HWND targetWindow);
	void updateOverlayText(const std::wstring& text);
	void updateOverlayPosition(HWND targetWindow);

private:
	std::wstring currentText;
	HWND overlayWindow;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // OVERLAYMANAGER_H