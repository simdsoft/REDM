@echo off
echo hgy copy dbghelp.dll start---------------------------------

set batDir=%~dp0
shift & set OutDir=%1

cd /d %batDir%
echo hgy delete TGPRes folder begin------------------------------
rmdir  /s /q %OutDir%\TGPRes\
echo hgy delete TGPRes folder end--------------------------------
echo hgy copy dbghelp.dll begin---------------------------------
copy /y .\dbghelp.dll %OutDir%
echo hgy copy dbghelp.dll end-----------------------------------
echo hgy copy TGPRes folder begin--------------------------------
copy /y ..\..\Docs\TestData\dmplugins.cfg %OutDir%
copy /y ..\..\Docs\TestData\dmpluginsd.cfg %OutDir%
copy /y ..\..\Docs\TestData\DMWebkit.dll %OutDir%
copy /y ..\..\Docs\TestData\NPSWF32.dll %OutDir%
xcopy /y /s ..\..\Docs\TestData\TGPRes %OutDir%\TGPRes\
echo hgy copy TGPRes folder end----------------------------------

