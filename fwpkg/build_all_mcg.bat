@echo off

.\..\tools\TreeLayer.exe -s .\ap\mainmenu.ap
.\..\tools\TreeLayer.exe -s .\ap\music.ap
.\..\tools\TreeLayer.exe -s .\ap\browser.ap
.\..\tools\TreeLayer.exe -s .\ap\setting.ap
.\..\tools\TreeLayer.exe -s .\ap\radio.ap
.\..\tools\TreeLayer.exe -s .\ap\record.ap

del .\mcg\*.dat

pause