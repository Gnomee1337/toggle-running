#include "utils.h"
#include <tlhelp32.h>
#include <iostream>
#include <sddl.h>

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
	return 0;
}

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

bool IsRunningAsAdmin() {
	BOOL isAdmin = FALSE;
	PSID administratorsGroup;
	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

	if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup))
	{
		CheckTokenMembership(NULL, administratorsGroup, &isAdmin);
		FreeSid(administratorsGroup);
	}

	return isAdmin;
}