@echo off
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET OutputDir=%ScriptsDir%..\out
SET ClientCLIDir=%SourceDir%\Client\CLI
SET ExtensionCppDir=%SourceDir%\GenerateSharp\Extensions\Cpp\Extension
SET ExtensionCSharpDir=%SourceDir%\GenerateSharp\Extensions\CSharp\Extension
SET MonitorClientDir=%SourceDir%\Monitor\Client
if %Flavor% == release (SET OutputX64DirectorPath=txTMowfPh1V3rPmbvNBmBW9Z8Jg) else (SET OutputX64DirectorPath=J3mu4cpISw6nDaCPED8gkqZ-q84)
if %Flavor% == release (SET OutputX86DirectorPath=ci_UJP5zJKyF-O0VVSVDMNi1Wwg) else (SET OutputX86DirectorPath=9e1e87dea39eeb349e44752a9a37cdf3ed1c2e7aea3044a45ed8bb44609efd)
SET ClientCLIOutputDirectory=%OutputDir%\C++\Soup\%OutputX64DirectorPath%
SET MonitorClientOutputX64Directory=%OutputDir%\C++\Monitor.Client\%OutputX64DirectorPath%
SET MonitorClientOutputX86Directory=%OutputDir%\C++\Monitor.Client\%OutputX86DirectorPath%

REM - Build each version of the monitor client dll
echo soup build %MonitorClientDir% -architecture x64 -flavor %Flavor%
call soup build %MonitorClientDir% -architecture x64 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
echo soup build %MonitorClientDir% -architecture x86 -flavor %Flavor%
call soup build %MonitorClientDir% -architecture x86 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Build the C++ Extension
echo soup build %ExtensionCppDir% -flavor %Flavor%
call soup build %ExtensionCppDir% -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Build the C# Extension
echo soup build %ExtensionCSharpDir% -flavor %Flavor%
call soup build %ExtensionCSharpDir% -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Build the host
echo soup build %ClientCLIDir% -flavor %Flavor%
call soup build %ClientCLIDir% -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Copy the monitor client dlls
echo copy "%MonitorClientOutputX64Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.64.dll"
copy "%MonitorClientOutputX64Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.64.dll"
echo copy "%MonitorClientOutputX86Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.32.dll"
copy "%MonitorClientOutputX86Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.32.dll"