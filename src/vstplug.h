#ifndef VSTPLUG_H
#define VSTPLUG_H

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "C1.h"

enum
{
	// Global
	kNumPrograms = 1,

	// Parameters Tags
	kDisable = 0,
	kInGain,
	kOutGain,
	kBitDepth,
	kDCBias,
	kDither,
	kDitherType,
	kInvertDither,
	kHighpassDither,
	kHighpassGain,
	kDitherGain,
	kMersenneTwister,
	kMersenneGenerator,
	kSeed,
	kSeedWithTime,
	kNoiseShaping,
	kNoiseShapingFilter,
	kPsychoacousticCurve,
	kNoiseShapingGain,
	kAutoBlank,
	kClip,
	kClipThreshold,
	kQuantize,
	kQuantizationMode,
	kClip0dB,
	kDitherInError,
	kOnlyError,

	kNumParams
};

class C1Bitcrusher : public AudioEffectX
{
public:
	C1Bitcrusher (audioMasterCallback audioMaster);
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterName (VstInt32 index, char* text);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	virtual bool getEffectName (char* name);
	virtual bool getProductString (char* text);
	virtual bool getVendorString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstPlugCategory getPlugCategory ();
	virtual void resume ();
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
private:
	char ProgramName[32];
	C1Settings settings;
	C1State state;
	C1ChannelState channel[2];
};

#endif
