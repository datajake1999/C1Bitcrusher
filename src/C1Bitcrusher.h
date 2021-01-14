#ifndef __C1Bitcrusher__
#define __C1Bitcrusher__

#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	kDisable = 0,
	kBitDepth,
	kDCBias,
	kDither,
	kDitherType,
	kInvertDither,
	kDitherInError,
	kNoiseShaping,
	kNoiseShapingMode,
	kQuantize,
	kQuantizationMode,
	kOnlyError,
	kAutoBlank,
	kMersenneTwister,
	kMersenneGenerator,
	kSeed,
	kSeedWithTime,
	kClipPreQuantization,
	kClipValue,
	kClipPostQuantization,
	kInGain,
	kOutGain,
	kDitherGain,
	kNoiseShapingGain,
	kNumParams
};

class C1Bitcrusher : public AudioEffectX
{
public:
	C1Bitcrusher (audioMasterCallback audioMaster);
	virtual void resume ();
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
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
private:
	float Disable;
	float BitDepth;
	float DCBias;
	float Dither;
	float DitherType;
	float InvertDither;
	float DitherInError;
	float NoiseShaping;
	float NoiseShapingMode;
	float Quantize;
	float QuantizationMode;
	float OnlyError;
	float AutoBlank;
	float MersenneTwister;
	float MersenneGenerator;
	float Seed;
	float SeedWithTime;
	float ClipPreQuantization;
	float ClipValue;
	float ClipPostQuantization;
	float InGain;
	float OutGain;
	float DitherGain;
	float NoiseShapingGain;
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
