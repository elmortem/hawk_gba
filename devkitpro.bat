@echo off
REM Укажите путь до DevKitPro!
set DEVKITPRO=e:\devkitPro
set DEVKITARM=%DEVKITPRO%\devkitarm

set PATH=%DEVKITARM%\bin;%DEVKITARM%\arm-none-eabi\bin;%DEVKITPRO%\msys\bin;%PATH%
set CXX=arm-none-eabi-g++.exe

REM Скомпонуем параметры командной строки для компилятора
set PARAMS=-specs=gba.specs -mcpu=arm7tdmi -mtune=arm7tdmi 
set PARAMS=%PARAMS% -Wall -fomit-frame-pointer -ffast-math -fno-rtti -fno-exceptions 
set PARAMS=%PARAMS% -mthumb -mthumb-interwork -g0 -O2
set PARAMS=%PARAMS% -Iout -I..
REM 
REM set PARAMS=%PARAMS% -I%DEVKITPRO%\libgba\include
REM set PARAMS=%PARAMS% -L%DEVKITPRO%\libgba\lib
@echo on