#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>

DWORD FindProcessIdByExeName(const std::wstring& exeName);
HWND FindMainWindowByProcessId(DWORD processId);
bool IsRunningAsAdmin();

#endif // UTILS_H
