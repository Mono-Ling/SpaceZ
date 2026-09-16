@echo off
setlocal enabledelayedexpansion

rem ===========================================================================
rem  SpaceZ_NativePlugins -- native plugin build entry point (x64 / MSVC)
rem  Shared by the VSCode tasks (tasks.json) and the command line: this is the
rem  ONLY place the compiler/linker flags live, so edit here and nowhere else.
rem
rem    build.bat console-debug    console debug build  -> Main\Debug\SpaceZ_Console.exe
rem    build.bat plugin-release   Unity plugin (Release) -> Assets\Plugins\x86_64\SpaceZ_NativePlugins.dll
rem    build.bat plugin-debug     same, but Debug DLL (overwrites the Release one)
rem    build.bat clean            delete build outputs under Main\Debug (keeps the DLL in Assets)
rem
rem  KEEP THIS FILE PURE ASCII. Do not put Chinese (or any non-ASCII) back in.
rem  cmd.exe reads .bat files with the system ANSI code page (cp936 on a Chinese
rem  Windows), so UTF-8 comments get mis-decoded, and fragments of them can even
rem  be executed as commands ("xxx is not recognized as an internal command").
rem  All messages printed to the console are English for the same reason.
rem
rem  Layout: this plugin root sits NEXT TO the Unity Assets folder, i.e.
rem  <UnityProject>\SpaceZ_NativePlugins. Unity only scans Assets\ and Packages\,
rem  so nothing in here is imported and no .meta files are ever generated.
rem  All intermediates go under Main\Debug\.obj\ ; only sources plus the project
rem  config are meant to be committed.
rem
rem  To switch compiler version, edit VSDEV below.
rem ===========================================================================

set "VSDEV=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

rem --- locate directories: this script lives in <plugin root>\tools\ ---
pushd "%~dp0.."
set "ROOT=%CD%"
rem ROOT = the plugin root, e.g. <UnityProject>\SpaceZ_NativePlugins
rem It sits NEXT TO the Unity Assets folder, so the Unity project root is its
rem parent. Resolve the parent once here, instead of scattering ".." around --
rem getting this level wrong is the #1 way to break this script silently.
pushd ".."
set "UNITYROOT=%CD%"
popd
set "PLUGINDIR=%UNITYROOT%\Assets\Plugins\x86_64"
set "DLLNAME=SpaceZ_NativePlugins"

rem Layout guard: fail fast if this folder was moved somewhere else, instead of
rem silently creating an Assets\Plugins directory in some random location.
if not exist "%UNITYROOT%\Assets" goto :bad_layout
if not exist "%UNITYROOT%\ProjectSettings" goto :bad_layout

rem Every cl/link argument below is UNQUOTED (a quote in the middle of an MSVC
rem option is easily mis-parsed). So reject paths containing a space up front and
rem print a clear message instead of an unreadable compiler error.
if not "%ROOT%"=="%ROOT: =%" (
  echo [ERROR] the plugin path contains a space:
  echo         %ROOT%
  echo         Move the project to a space-free path, or add quotes back in
  echo         tools\build.bat and re-test the cl/link command lines.
  popd
  endlocal
  exit /b 1
)

set "TARGET=%~1"
if "%TARGET%"=="" set "TARGET=console-debug"

if /I "%TARGET%"=="console-debug"  goto :console_debug
if /I "%TARGET%"=="plugin-release" goto :plugin_release
if /I "%TARGET%"=="plugin-debug"   goto :plugin_debug
if /I "%TARGET%"=="clean"          goto :clean
echo [ERROR] unknown target: %TARGET%
goto :usage


rem ====================================================== console debug build
:console_debug
set "CFLAGS=/nologo /c /std:c++20 /utf-8 /EHsc /W4 /Zi /Od /RTC1 /MDd /D_DEBUG /I%ROOT%"
set "OBJDIR=Main\Debug\.obj\console"
set "OUTEXE=%ROOT%\Main\Debug\SpaceZ_Console.exe"

call :vcvars
if errorlevel 1 goto :fail
echo [1/3] compiling Core ...
call :compile "Core" "%OBJDIR%\core"
if errorlevel 1 goto :fail
echo [2/3] compiling Main ...
call :compile "Main" "%OBJDIR%\main"
if errorlevel 1 goto :fail
echo [3/3] linking ...
call :collect_objs "%OBJDIR%"
link /nologo /DEBUG /INCREMENTAL /SUBSYSTEM:CONSOLE /OUT:%OUTEXE% /PDB:%ROOT%\Main\Debug\SpaceZ_Console.pdb !OBJS!
if errorlevel 1 goto :fail
echo.
echo [OK] console app -^> Main\Debug\SpaceZ_Console.exe
goto :done


rem ============================================================ Unity plugin
:plugin_release
set "CFG=release"
set "CFLAGS=/nologo /c /std:c++20 /utf-8 /EHsc /W4 /Zi /O2 /MD /DNDEBUG /I%ROOT%"
goto :plugin_build

:plugin_debug
set "CFG=debug"
set "CFLAGS=/nologo /c /std:c++20 /utf-8 /EHsc /W4 /Zi /Od /RTC1 /MDd /D_DEBUG /I%ROOT%"
goto :plugin_build

:plugin_build
set "OBJDIR=Main\Debug\.obj\plugin-%CFG%"
set "OUTDLL=%ROOT%\%OBJDIR%\%DLLNAME%.dll"

call :vcvars
if errorlevel 1 goto :fail
echo [1/4] compiling Core ...
call :compile "Core" "%OBJDIR%\core"
if errorlevel 1 goto :fail
echo [2/4] compiling UnityNativePlugins ...
call :compile "UnityNativePlugins" "%OBJDIR%\unity"
if errorlevel 1 goto :fail
echo [3/4] linking %DLLNAME%.dll (%CFG%) ...
call :collect_objs "%OBJDIR%"
link /nologo /DEBUG /INCREMENTAL:NO /DLL /OUT:%OUTDLL% /PDB:%ROOT%\Main\Debug\%DLLNAME%.pdb /IMPLIB:%ROOT%\%OBJDIR%\%DLLNAME%.lib !OBJS!
if errorlevel 1 goto :fail
echo [4/4] copying DLL into the Unity plugins folder ...
if not exist "%PLUGINDIR%" mkdir "%PLUGINDIR%"
copy /Y "%OUTDLL%" "%PLUGINDIR%\" >nul
if errorlevel 1 goto :copy_fail
echo.
echo [OK] Unity plugin DLL -^> Assets\Plugins\x86_64\%DLLNAME%.dll
dumpbin /nologo /exports %OUTDLL% | findstr /C:"number of names"
goto :done


rem =================================================================== clean
:clean
if exist "Main\Debug\.obj" rd /s /q "Main\Debug\.obj"
del /q "Main\Debug\*.exe" 2>nul
del /q "Main\Debug\*.pdb" 2>nul
del /q "Main\Debug\*.ilk" 2>nul
del /q "Main\Debug\*.exp" 2>nul
del /q "Main\Debug\*.lib" 2>nul
echo [OK] build outputs under Main\Debug removed.
echo      (the DLL in Assets\Plugins\x86_64 was kept)
goto :done


rem ============================================================= subroutines
rem Initialise the MSVC environment; skip if already in an x64 dev prompt.
:vcvars
if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" exit /b 0
if not exist "%VSDEV%" (
  echo [ERROR] vcvars64.bat not found:
  echo         %VSDEV%
  echo         edit VSDEV at the top of tools\build.bat
  exit /b 1
)
call "%VSDEV%" >nul
if errorlevel 1 (
  echo [ERROR] failed to initialize the MSVC environment.
  exit /b 1
)
exit /b 0

rem Compile one source directory
rem   %1 = source dir (relative to plugin root), %2 = obj dir (relative to plugin root)
rem   One obj dir per source directory on purpose: Core\Add.cpp and
rem   UnityNativePlugins\Add.cpp share a base name and would overwrite each other.
:compile
set "SRC=%~1"
set "OBJSUB=%~2"
if not exist "%OBJSUB%" mkdir "%OBJSUB%"
cl %CFLAGS% %SRC%\*.cpp /Fo%OBJSUB%\ /Fd%OBJSUB%\vc.pdb
if errorlevel 1 exit /b 1
exit /b 0

rem Collect the obj list into OBJS
rem   %1 = obj root dir (relative to plugin root)
:collect_objs
set "OBJS="
for /f "delims=" %%f in ('dir /s /b "%~1\*.obj" 2^>nul') do set "OBJS=!OBJS! "%%f""
if not defined OBJS (
  echo [ERROR] no object files found under %~1
  exit /b 1
)
exit /b 0


rem ================================================================= finish
:usage
echo.
echo usage: build.bat [console-debug ^| plugin-release ^| plugin-debug ^| clean]
echo   console-debug    console app (Debug)     -^> Main\Debug\SpaceZ_Console.exe
echo   plugin-release   Unity plugin (Release)  -^> Assets\Plugins\x86_64\%DLLNAME%.dll
echo   plugin-debug     Unity plugin (Debug)    -^> same DLL path (overwrites release)
echo   clean            remove build outputs under Main\Debug
popd
endlocal
exit /b 1

:done
popd
endlocal
exit /b 0

:fail
echo.
echo [FAILED] see the compiler/linker output above.
popd
endlocal
exit /b 1

:copy_fail
echo.
echo [FAILED] cannot copy the DLL into Assets\Plugins\x86_64.
echo          The DLL is most likely locked by a running Unity Editor.
echo          Close Unity (or restart the Editor) and build again.
popd
endlocal
exit /b 1

:bad_layout
echo.
echo [FAILED] cannot locate the Unity project.
echo          Expected: %UNITYROOT%\Assets      (and %UNITYROOT%\ProjectSettings)
echo          SpaceZ_NativePlugins must stay next to the Unity Assets folder,
echo          that is  ^<UnityProject^>\SpaceZ_NativePlugins\
popd
endlocal
exit /b 1
