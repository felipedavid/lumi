@echo off
if not exist ..\build mkdir ..\build
pushd build

set CompilerFlags=-Zi
set Sources=..\src\vm.c ..\src\lexer.c ..\src\common.c ..\src\lumi.c ..\src\test.c

cl.exe %CompilerFlags% %Sources%

popd
