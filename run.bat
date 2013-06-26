@ECHO OFF

TITLE Running OpenFiber...

IF NOT EXIST build\bin\OpenFiber.exe GOTO Information

build\bin\OpenFiber %*

GOTO End

:Information

ECHO OpenFiber must first be built before being run.
ECHO.

pause>null

:End