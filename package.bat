@ECHO OFF

SET ExitCode=%ERRORLEVEL%

IF %ExitCode% EQU 0 GOTO PackageOpenFiber

EXIT /B %ExitCode%

:PackageOpenFiber

TITLE Packaging OpenFiber...

CD build

cpack -C CPackConfig.cmake

SET ExitCode=%ERRORLEVEL%

CD ..

EXIT /B %ExitCode%
 