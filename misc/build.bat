@echo off
if not exist ..\build mkdir ..\build
pushd build

set CompilerFlags=-Zi
set Sources=..\src\lumi.c ..\src\lexer.c ..\src\common.c

cl.exe %CompilerFlags% %Sources%

popd
