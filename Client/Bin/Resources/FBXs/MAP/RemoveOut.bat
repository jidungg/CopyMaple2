@echo off
setlocal enabledelayedexpansion

for %%f in (*.fbx) do (
    set "filename=%%~nf"
    set "extension=%%~xf"
    
    if "!filename:~-3!" == "out" (
        set "newname=!filename:~0,-3!!extension!"
        echo ���ϸ� ����: %%f -> !newname!
        ren "%%f" "!newname!"
    )
)

echo �Ϸ�!
pause