@echo off

call !clear1.bat Debug
call !clear1.bat Release

if exist *.pdb del *.pdb
if exist *.ncb del *.ncb
if exist *.user del *.user
if exist Pictor_deb.exe del Pictor_deb.exe
if exist src\Pictor.aps del src\Pictor.aps

attrib -H Pictor.suo > nul
if exist Pictor.suo del Pictor.suo
