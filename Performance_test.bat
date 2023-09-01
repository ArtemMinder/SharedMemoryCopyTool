@echo off
setlocal EnableDelayedExpansion

set /A fileSize = 1000000000
fsutil file createnew source.txt %fileSize%
fsutil file createnew target.txt 1

set "startTime=%time%"

start..\Debug\Shared_Memory_Copy_Tool.exe
call..\Debug\Shared_Memory_Copy_Tool.exe
set "endTime=%time%"

set "end=!endTime:%time:~8,1%=%%100)*100+1!"  &  set "start=!startTime:%time:~8,1%=%%100)*100+1!"
set /A "elap=((((10!end:%time:~2,1%=%%100)*60+1!%%100)-((((10!start:%time:~2,1%=%%100)*60+1!%%100), elap-=(elap>>31)*24*60*60*100"

set /A speed = (%filesize%/10000)/%elap%

set /A "cc=elap%%100+100,elap/=100,ss=elap%%60+100,elap/=60,mm=elap%%60+100,hh=elap/60+100"

echo Start:    %startTime%
echo End:      %endTime%
echo Elapsed:  %hh:~1%%time:~2,1%%mm:~1%%time:~2,1%%ss:~1%%time:~8,1%%cc:~1%
echo Speed: %speed% mb/s

del source.txt

pause