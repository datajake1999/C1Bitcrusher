@ECHO OFF
set path="C:\Program Files\7-Zip";"C:\Program Files\Inno Setup 5"
call "c:\program files\microsoft visual studio 8\vc\bin\vcvars32.bat"
call "C:\program files\Microsoft Platform SDK\SetEnv.Cmd" /SRV32 /RETAIL
cl /nologo -c /D _CRT_SECURE_NO_DEPRECATE /I ".\VST2_SDK" /O2 src\*.cpp VST2_SDK\public.sdk\source\vst2.x\*.cpp
link *.obj bufferoverflowU.lib kernel32.lib user32.lib /nologo /dll /def:".\src\vstplug.def" /OUT:C1Bitcrusher.dll
del *.obj
call "C:\program files\Microsoft Platform SDK\SetEnv.Cmd" /X64 /RETAIL
cl /nologo -c /D _CRT_SECURE_NO_DEPRECATE /I ".\VST2_SDK" /O2 src\*.cpp VST2_SDK\public.sdk\source\vst2.x\*.cpp
link *.obj bufferoverflowU.lib kernel32.lib user32.lib /nologo /dll /def:".\src\vstplug.def" /OUT:C1Bitcrusher_X64.dll
del *.obj
call "C:\program files\Microsoft Platform SDK\SetEnv.Cmd" /SRV64 /RETAIL
cl /nologo -c /D _CRT_SECURE_NO_DEPRECATE /I ".\VST2_SDK" /O2 src\*.cpp VST2_SDK\public.sdk\source\vst2.x\*.cpp
link *.obj bufferoverflowU.lib kernel32.lib user32.lib /nologo /dll /def:".\src\vstplug.def" /OUT:C1Bitcrusher_IA64.dll
del *.exp *.lib *.obj
iscc /Qp "install.iss"
7z a -tzip output\C1Bitcrusher.zip C1Bitcrusher.dll C1Bitcrusher_IA64.dll C1Bitcrusher_X64.dll license.txt readme.md
