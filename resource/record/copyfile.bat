@echo off
set mydir=%cd%\
xcopy  %mydir%*.sty  %mydir%\..\..\fwpkg\sty /d /s /y
xcopy  %mydir%*.h  %mydir%\..\..\ap\ap_record\ /d /s /y
pause
