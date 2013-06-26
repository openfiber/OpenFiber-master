If CreateObject("Scripting.FileSystemObject").FileExists("build\bin\OpenFiber.exe") Then
    args = ""

    For Each arg in WScript.Arguments
        args = args&" "&arg
    Next

    CreateObject("WScript.Shell").Run "build\bin\OpenFiber.exe"&args, 1, False
Else
    WScript.Echo "OpenFiber must first be built before being run."
End If
