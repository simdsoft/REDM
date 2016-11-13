@echo off
echo hgy copy dbghelp.dll start---------------------------------

set batDir=%~dp0
shift & set OutDir=%1

cd /d %batDir%
echo hgy delete DesignerRes folder begin------------------------------
rmdir  /s /q %OutDir%\DesignerRes\
echo hgy delete DesignerRes folder end--------------------------------
echo hgy copy dbghelp.dll begin---------------------------------
copy /y .\dbghelp.dll %OutDir%
echo hgy copy dbghelp.dll end-----------------------------------
echo hgy copy DesignerRes folder begin--------------------------------
copy /y ..\..\Docs\TestData\dmplugins_design.cfg %OutDir%
copy /y ..\..\Docs\TestData\dmpluginsd_design.cfg %OutDir%
xcopy /y /s ..\..\Docs\TestData\DesignerRes %OutDir%\DesignerRes\
echo hgy copy DesignerRes folder end----------------------------------

