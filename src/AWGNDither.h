#ifndef __AWGNDither__
#define __AWGNDither__

#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	kBitDepth = 0,
	kDither,
	kNumParams
};

class AWGNDither;

class AWGNDither : public AudioEffectX
{
public:
	AWGNDither (audioMasterCallback audioMaster);
	virtual void setParameter (VstInt32 index, float value);
	virtual void setParameterAutomated (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterName (VstInt32 index, char* text);
	virtual bool getEffectName (char* name);
	virtual bool getProductString (char* text);
	virtual bool getVendorString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
private:
	float BitDepth;
	float Dither;
	float AWGN_generator();
};

#endif
