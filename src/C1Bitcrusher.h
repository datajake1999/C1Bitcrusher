#ifndef __C1Bitcrusher__
#define __C1Bitcrusher__

#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	kDisable = 0,
	kInGain,
	kOutGain,
	kBitDepth,
	kDCBias,
	kDither,
	kDitherType,
	kInvertDither,
	kDitherGain,
	kMersenneTwister,
	kMersenneGenerator,
	kSeed,
	kSeedWithTime,
	kNoiseShaping,
	kNoiseShapingFocus,
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
	virtual void setParameterAutomated (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterName (VstInt32 index, char* text);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getEffectName (char* name);
	virtual bool getProductString (char* text);
	virtual bool getVendorString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }
	virtual void resume ();
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
private:
	float Disable;
	float InGain;
	float OutGain;
	float BitDepth;
	float DCBias;
	float Dither;
	float DitherType;
	float InvertDither;
	float DitherGain;
	float MersenneTwister;
	float MersenneGenerator;
	float Seed;
	float SeedWithTime;
	float NoiseShaping;
	float NoiseShapingFocus;
	float NoiseShapingGain;
	float AutoBlank;
	float Clip;
	float ClipThreshold;
	float Quantize;
	float QuantizationMode;
	float Clip0dB;
	float DitherInError;
	float OnlyError;
	float MT_generator();
	float RPDF();
	float TPDF();
	float AWGN_generator();
	float DitherNoise();
	float DitherSample(float sample);
	float NoiseShapeSample(float sample, float noise);
	float DCSample(float sample);
	float ClipSample(float sample);
	float QuantizeSample(float sample);
	float quantized[2];
	float error[2];
	float NumAmplitudes;
	char ProgramName[32];
};

#endif
