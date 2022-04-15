@echo off
if not exist build mkdir build
pushd build

set CompilerFlags=-Zi
set Sources=..\src\lumi.c

cl.exe %CompilerFlags% %Sources%

popd
