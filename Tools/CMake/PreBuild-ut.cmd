@echo off
echo hgy copy dbghelp.dll start---------------------------------

set batDir=%~dp0
shift & set OutDir=%1

cd /d %batDir%
echo hgy delete UTRes folder begin------------------------------
rmdir  /s /q %OutDir%\UTRes\
echo hgy delete UTRes folder end--------------------------------
echo hgy copy dbghelp.dll begin---------------------------------
copy /y .\dbghelp.dll %OutDir%
echo hgy copy dbghelp.dll end-----------------------------------
echo hgy copy UTRes folder begin--------------------------------
copy /y ..\..\Docs\TestData\dmplugins.cfg %OutDir%
copy /y ..\..\Docs\TestData\dmpluginsd.cfg %OutDir%
xcopy /y /s ..\..\Docs\TestData\UTRes %OutDir%\UTRes\
echo hgy copy UTRes folder end----------------------------------

