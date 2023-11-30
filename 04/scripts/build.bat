@echo off

rem Don't ask, got this from cpythons 'PCbuild/env.bat'

if defined DevEnvDir (
    goto :vs_done
)

if not exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" goto :skip_vswhere
set VSTOOLS=
for /F "tokens=*" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath -latest -prerelease -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64') DO @(set VSTOOLS=%%i\VC\Auxiliary\Build\vcvarsall.bat)
if not defined VSTOOLS goto :skip_vswhere
call "%VSTOOLS%" x64
goto :vs_done

:skip_vswhere
if not defined VSTOOLS set VSTOOLS=%VS140COMNTOOLS%
if not defined VSTOOLS set VSTOOLS=%VS120COMNTOOLS%
if not defined VSTOOLS set VSTOOLS=%VS110COMNTOOLS%
if not defined VSTOOLS set VSTOOLS=%VS100COMNTOOLS%
call "%VSTOOLS%..\..\VC\vcvarsall.bat" x64

:vs_done

cmake . -B build
cmake --build build %*
