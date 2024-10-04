@echo off
setlocal enabledelayedexpansion

for %%f in (*.fbx) do (
    set "filename=%%~nf"
    set "extension=%%~xf"
    
    if "!filename:~-3!" == "out" (
        set "newname=!filename:~0,-3!!extension!"
        echo 파일명 변경: %%f -> !newname!
        ren "%%f" "!newname!"
    )
)

echo 완료!
pause