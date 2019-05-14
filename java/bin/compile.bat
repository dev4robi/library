@echo off
setlocal
rem ################################################################
set BIN_DIR=%cd%
set SRC_DIR=%BIN_DIR%/../src
echo ================================================================
echo Initailizing...
rem ################################################################
cd %SRC_DIR%
dir /s /b *.java > compileList.txt
echo ================================================================
echo Making '*.java' file list...
rem ################################################################
set JAVAC=javac -cp . -d ../classes @compileList.txt
echo ================================================================
echo Compileing...
echo ================================================================
echo %JAVAC%
%JAVAC%
rem ################################################################
del compileList.txt
cd %BIN_DIR%
echo ================================================================
echo Done!
echo ================================================================
rem ################################################################
endlocal