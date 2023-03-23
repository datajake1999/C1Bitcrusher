@echo off
g++ -I..\VST2_SDK -O3 ..\src\*.cpp ..\VST2_SDK\public.sdk\source\vst2.x\*.cpp -s -static -shared -o C1Bitcrusher.dll
