#ifndef __AWGNDither__
#define __AWGNDither__

#include "public.sdk/source/vst2.x/audioeffectx.h"

class AWGNDither;

class AWGNDither : public AudioEffectX
{
public:
	AWGNDither (audioMasterCallback audioMaster);
	virtual bool getEffectName (char* name);
	virtual bool getProductString (char* text);
	virtual bool getVendorString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
private:
	float AWGN_generator();
};

#endif
