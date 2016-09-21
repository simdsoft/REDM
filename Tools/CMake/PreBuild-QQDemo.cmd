@echo off
echo hgy copy dbghelp.dll start---------------------------------

set batDir=%~dp0
shift & set OutDir=%1

cd /d %batDir%
echo hgy delete QQRes folder begin------------------------------
rmdir  /s /q %OutDir%\QQRes\
echo hgy delete QQRes folder end--------------------------------
echo hgy copy dbghelp.dll begin---------------------------------
copy /y .\dbghelp.dll %OutDir%
echo hgy copy dbghelp.dll end-----------------------------------
echo hgy copy QQRes folder begin--------------------------------
copy /y ..\..\Docs\TestData\dmplugins.cfg %OutDir%
copy /y ..\..\Docs\TestData\dmpluginsd.cfg %OutDir%
copy /y ..\..\Docs\TestData\DMWebkit.dll %OutDir%
copy /y ..\..\Docs\TestData\NPSWF32.dll %OutDir%
xcopy /y /s ..\..\Docs\TestData\QQRes %OutDir%\QQRes\
echo hgy copy QQRes folder end----------------------------------

