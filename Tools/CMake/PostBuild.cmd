@echo off
echo hgy copy dbghelp.dll start---------------------------------

set batDir=%~dp0
shift & set OutDir=%1

cd /d %batDir%
echo hgy clear  output start------------------------------------
cd /d %OutDir%
del /f /s /q *.aps
del /f /s /q *.ncb
del /f /s /q /ah *.suo
del /f /s /q *.vcproj.*.user
del /f /s /q *.plg
del /f /s /q *.opt
del /f /s /q *.pch
del /f /s /q *.idb
del /f /s /q *.ilk
del /f /s /q *.obj
del /f /s /q *.dep
del /f /s /q *.DPbcd
del /f /s /q *.manifest
del /f /s /q *.ib_tag
del /f /s /q *.res
del /f /s /q *.ib_pdb_index
del /f /s /q *.exp
del /f /s /q *.bak
echo hgy clear  output end------------------------------------
echo hgy clear  plugin start------------------------------------
cd /d %OutDir%\..\Plugins
del /f /s /q *.aps
del /f /s /q *.ncb
del /f /s /q /ah *.suo
del /f /s /q *.vcproj.*.user
del /f /s /q *.plg
del /f /s /q *.opt
del /f /s /q *.pch
del /f /s /q *.idb
del /f /s /q *.ilk
del /f /s /q *.obj
del /f /s /q *.htm
del /f /s /q *.dep
del /f /s /q *.DPbcd
del /f /s /q *.manifest
del /f /s /q *.ib_tag
del /f /s /q *.res
del /f /s /q *.ib_pdb_index
del /f /s /q *.exp
del /f /s /q *.bak
echo hgy clear  plugin end------------------------------------