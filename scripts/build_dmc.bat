@echo off
set path=C:\DM\Bin;%path%
dmc -c -I..\VST2_SDK -o @src.lst
link /exetype:nt C1Bitcrusher.obj C1BitcrusherProc.obj main.obj mt19937ar.obj audioeffect.obj audioeffectx.obj vstplugmain.obj,C1Bitcrusher.dll,,kernel32.lib user32.lib,..\src\vstplug.def
del *.map *.obj
