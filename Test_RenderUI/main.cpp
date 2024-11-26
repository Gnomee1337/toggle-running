#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

// Global variables
bool isF3Pressed = false;  // Tracks whether "F3" key was pressed
bool isRunning = false;    // Tracks if the character is currently running
HWND overlayWindow = NULL; // Handle for the overlay window

// Function to find the process ID of a running process by its .exe name
DWORD FindProcessIdByExeName(const std::wstring& exeName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create snapshot!" << std::endl;
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (exeName == pe32.szExeFile) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID; // Return the process ID of the target game
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    std::cout << "Target process not found!" << std::endl;
    return 0;
}

// Function to find the main window handle by process ID
HWND FindMainWindowByProcessId(DWORD processId) {
    HWND hwnd = GetTopWindow(NULL);
    while (hwnd) {
        DWORD windowProcessId;
        GetWindowThreadProcessId(hwnd, &windowProcessId);
        if (windowProcessId == processId && GetWindow(hwnd, GW_OWNER) == NULL) {
            return hwnd; // Found the main window
        }
        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
    }
    return NULL;
}

// Function to bring the game window to the foreground
void BringWindowToForeground(HWND hwnd) {
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

// Function to simulate key press and release using SendInput
void SimulateKeyPress(WORD key, bool press) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = press ? 0 : KEYEVENTF_KEYUP; // Send key up if not pressing
    SendInput(1, &input, sizeof(INPUT));
}

// Window procedure for overlay
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
        SetBkMode(hdc, TRANSPARENT);

        // Update text based on the global state
        std::wstring text = isRunning ? L"Running On" : L"Running Off";
        TextOut(hdc, 10, 10, text.c_str(), static_cast<int>(text.length()));

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Function to create the overlay window
void CreateOverlayWindow(HINSTANCE hInstance, HWND targetWindow) {
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

    // Make the overlay transparent
    SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(overlayWindow, SW_SHOW);
    std::cout << "Overlay created." << std::endl;
}

// Main function
int main() {
    const std::wstring exeName = L"Stalker2-Win64-Shipping.exe"; // Replace with your target .exe name
    DWORD processId = FindProcessIdByExeName(exeName);

    if (processId == 0) {
        std::cout << "Target process not found!" << std::endl;
        MessageBoxW(NULL, L"Target process not found!", L"Error", MB_ICONERROR);
        return -1;
    }

    std::cout << "Target process found with Process ID: " << processId << std::endl;

    HWND gameWindow = FindMainWindowByProcessId(processId);
    if (!gameWindow) {
        std::cout << "Game window not found!" << std::endl;
        MessageBoxW(NULL, L"Game window not found!", L"Error", MB_ICONERROR);
        return -1;
    }

    std::cout << "Game window found!" << std::endl;

    // Bring the game window to the foreground
    BringWindowToForeground(gameWindow);

    // Create the overlay window
    HINSTANCE hInstance = GetModuleHandle(NULL);
    CreateOverlayWindow(hInstance, gameWindow);

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

        if (GetAsyncKeyState(VK_F3) & 0x8000) { // Toggle on F3
            if (!isF3Pressed) { // Only toggle if it's the first press
                isF3Pressed = true;

                if (isRunning) {
                    // Stop running by releasing "W"
                    std::cout << "Stopping movement (Releasing W)" << std::endl;
                    SimulateKeyPress('W', false);  // Release 'W'
                    isRunning = false;
                }
                else {
                    // Start running by holding "W"
                    std::cout << "Starting movement (Pressing W)" << std::endl;
                    SimulateKeyPress('W', true);  // Press 'W'
                    isRunning = true;
                }

                // Trigger a repaint
                InvalidateRect(overlayWindow, NULL, TRUE);
            }
        }
        else {
            isF3Pressed = false;
        }

        RECT rect;
        GetWindowRect(gameWindow, &rect);
        SetWindowPos(overlayWindow, HWND_TOPMOST, rect.left, rect.top, 200, 50, SWP_NOSIZE | SWP_NOACTIVATE);

        Sleep(50);
    }

    return 0;
}
