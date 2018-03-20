@echo off
set workDir=%cd%
set binOriginal=..\..\psp_rel\bin_original
set batBulidAfi=buildAFI_card.bat
cd %binOriginal%
call %binOriginal%\%batBulidAfi%
cd %workDir%