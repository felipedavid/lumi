@echo off
SetLocal EnableDelayedExpansion

if not exist bin (
    mkdir bin
)

rem Get a list of all .c files
set c_filenames= 
for /R %%f in (*.c) do (
    set c_filenames=!c_filenames! %%f
)

set assembly=lumi
set compiler_flags=/Zi
set linker_flags=

echo "Building %assembly%..."
cl.exe %c_filenames% %compiler_flags% /Fe:bin\%assembly%.exe %linker_flags%
