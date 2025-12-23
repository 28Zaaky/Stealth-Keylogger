// API Hashing - Minimal version for standalone keylogger
// Copyright (c) 2025 - 28zaakypro@proton.me

#ifndef APIHASHING_H
#define APIHASHING_H

#include <windows.h>

namespace APIHash {
    constexpr DWORD GetForegroundWindow = 0x12345678;
    constexpr DWORD CallNextHookEx = 0x87654321;
}

class APIResolver {
public:
    static FARPROC ResolveAPI(DWORD hash) {
        // Simplified: return NULL to use direct API calls as fallback
        return NULL;
    }
};

#endif // APIHASHING_H
