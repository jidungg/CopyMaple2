@echo off
setlocal enabledelayedexpansion

:: Loop through all .fbx files in the current directory
for %%f in (*.fbx) do (
    set "filename=%%~nf"
    set "newname=!filename:out=!"
    
    :: If the new name is different from the old name, rename the file
    if not "!filename!"=="!newname!" (
        ren "%%f" "!newname!%%~xf"
    )
)

endlocal