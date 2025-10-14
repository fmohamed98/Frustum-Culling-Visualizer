@echo off
setlocal

set "SOLUTION=Frustum Culling.sln"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo Visual Studio Build Tools were not found.
    exit /b 1
)

for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do set "VSINSTALL=%%I"
if not defined VSINSTALL (
    echo Visual Studio Build Tools were not found.
    exit /b 1
)

set "MSBUILD=%VSINSTALL%\MSBuild\Current\Bin\MSBuild.exe"
if not exist "%MSBUILD%" (
    echo MSBuild was not found.
    exit /b 1
)

> "%SOLUTION%" echo Microsoft Visual Studio Solution File, Format Version 12.00
>> "%SOLUTION%" echo # Visual Studio Version 16
>> "%SOLUTION%" echo VisualStudioVersion = 16.0.29001.49
>> "%SOLUTION%" echo MinimumVisualStudioVersion = 10.0.40219.1
>> "%SOLUTION%" echo Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Frustum Culling", "Frustum Culling\Frustum Culling.vcxproj", "{6C2B768D-6889-404E-9AF1-F35CBD6AF423}"
>> "%SOLUTION%" echo EndProject
>> "%SOLUTION%" echo Global
>> "%SOLUTION%" echo GlobalSection(SolutionConfigurationPlatforms) = preSolution
>> "%SOLUTION%" echo Debug^|x64 = Debug^|x64
>> "%SOLUTION%" echo Debug^|x86 = Debug^|x86
>> "%SOLUTION%" echo Release^|x64 = Release^|x64
>> "%SOLUTION%" echo Release^|x86 = Release^|x86
>> "%SOLUTION%" echo EndGlobalSection
>> "%SOLUTION%" echo GlobalSection(ProjectConfigurationPlatforms) = postSolution
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Debug^|x64.ActiveCfg = Debug^|x64
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Debug^|x64.Build.0 = Debug^|x64
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Debug^|x86.ActiveCfg = Debug^|Win32
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Debug^|x86.Build.0 = Debug^|Win32
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Release^|x64.ActiveCfg = Release^|x64
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Release^|x64.Build.0 = Release^|x64
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Release^|x86.ActiveCfg = Release^|Win32
>> "%SOLUTION%" echo {6C2B768D-6889-404E-9AF1-F35CBD6AF423}.Release^|x86.Build.0 = Release^|Win32
>> "%SOLUTION%" echo EndGlobalSection
>> "%SOLUTION%" echo GlobalSection(SolutionProperties) = preSolution
>> "%SOLUTION%" echo HideSolutionNode = FALSE
>> "%SOLUTION%" echo EndGlobalSection
>> "%SOLUTION%" echo GlobalSection(ExtensibilityGlobals) = postSolution
>> "%SOLUTION%" echo SolutionGuid = {D964C697-6E36-479C-A96E-F0BE9C19201B}
>> "%SOLUTION%" echo EndGlobalSection
>> "%SOLUTION%" echo EndGlobal

"%MSBUILD%" "%SOLUTION%" /m /p:Configuration=Release /p:Platform=x64
