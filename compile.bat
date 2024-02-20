@echo off

setlocal enabledelayedexpansion
set FILE="%~dp0\path.conf"

if not exist !FILE! (
  set /p userInput="Add meg a GCC fordito utvonalat: "
    echo !userInput! > !FILE!
) 
set /p confPath=<"%FILE%"
echo %confPath%
set source=%1
set source=%source:~0,-2%

%confPath% %1 -o %source%.exe -l%2
endlocal
