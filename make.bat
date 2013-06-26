@ECHO OFF

TITLE Making OpenFiber...

ECHO.
ECHO ---------------------------------
ECHO Making the GUI version of OpenFiber
ECHO ---------------------------------
ECHO.

CD build

cmake -G "MinGW Makefiles" ..
mingw32-make %*
pause>null