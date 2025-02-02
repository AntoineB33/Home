@echo off
cls
echo Running Hello World Program...
cd C:\Users\abarb\Documents\health\news_underground\mediaSorter\programs\c_prog\mediaSorter\test0
echo Current directory (before running program): %cd%
set PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64;$(VC_ExecutablePath_x64);C:\dev\FFmpeg-master\FFmpeg-master\libswresample;C:\dev\FFmpeg-master\FFmpeg-master\libavcodec;C:\dev\FFmpeg-master\FFmpeg-master\libavformat;C:\dev\FFmpeg-master\FFmpeg-master\libavutil;C:\dev\FFmpeg-master\FFmpeg-master\libswscale;C:\dev\SDL2-devel-2.30.8-VC\SDL2-2.30.8\lib;C:\dev\SDL2-devel-2.30.8-VC\SDL2-2.30.8\lib\x64;%PATH%
C:\Users\abarb\Documents\health\news_underground\mediaSorter\programs\c_prog\mediaSorter\x64\Debug\test_watch.exe "[[\"https://example.com/file1\", \"https://example.com/file2\"], [\"https://example.com/file3\"]]"
pause
