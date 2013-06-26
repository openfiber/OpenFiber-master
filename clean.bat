@ECHO OFF

TITLE Cleaning OpenFiber...

FOR /D %%I IN (build\*.* windows\build\*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (build\*.* windows\build\*.*) DO DEL /Q "%%I"