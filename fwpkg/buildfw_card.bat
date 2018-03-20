@echo off
for /f %%i in ('dir /b .\config_txt\*.txt') do (
copy .\config_txt\%%i .\config.txt
copy .\config_txt\fwimage_US216A_EVB_card.cfg .\fwimage.cfg
..\tools\Gen_config\Genbin.exe          .\config.txt         .\config.bin
..\..\psp_rel\tools\maker_py\Maker.exe -c .\fwimage.cfg -o .\US216A_card.fw -mf
::生成升级文件
..\..\psp_rel\tools\maker_py\FW2X.exe -fssf  .\US216A_card.fw  MakeBin MakeUPG .\US216A_card.FWU
copy .\US216A_card.FWU .\US216A_card.HEX
)
pause
