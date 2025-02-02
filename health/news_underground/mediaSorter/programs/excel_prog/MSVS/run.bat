@echo off
set PATH=C:\dev\FFmpeg-master\FFmpeg-master\libswresample;C:\dev\FFmpeg-master\FFmpeg-master\libavcodec;C:\dev\FFmpeg-master\FFmpeg-master\libavformat;C:\dev\FFmpeg-master\FFmpeg-master\libavutil;C:\dev\FFmpeg-master\FFmpeg-master\libswscale;C:\dev\SDL2-devel-2.30.8-VC\SDL2-2.30.8\lib;C:\dev\SDL2-devel-2.30.8-VC\SDL2-2.30.8\lib\x64;%PATH%
start C:\Users\abarb\Documents\health\news_underground\mediaSorter\programs\c_prog\mediaSorter\x64\Debug\test.exe "[[\"https://example.com/file1\", \"https://example.com/file2\"], [\"https://example.com/file3\"]]"
