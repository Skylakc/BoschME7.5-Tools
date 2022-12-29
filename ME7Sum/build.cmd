@echo off
set CURDIR=%cd%
rem SET VSVER=10.0
SET VSVER=2017

IF "%PROGRAMFILES(X86)%"=="" (
  GOTO x86
) ELSE (
  GOTO amd64
)

:amd64
SET PROGPATH=%PROGRAMFILES(X86)%
GOTO Common

:x86
SET PROGPATH=%PROGRAMFILES%
GOTO Common

:Common
rem CALL "%PROGPATH%\Microsoft Visual Studio %VSVER%\VC\vcvarsall.bat" x86
echo "%PROGRAMFILES(X86)%\Microsoft Visual Studio\%VSVER%\Community\VC\Auxiliary\Build\vcvarsall.bat"
CALL "%PROGRAMFILES(X86)%\Microsoft Visual Studio\%VSVER%\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

cd %CURDIR%
for /f "usebackq" %%i in ( `git describe --tags "--abbrev=4" --dirty --always` ) do SET GIT_VERSION=%%i

nmake %1
