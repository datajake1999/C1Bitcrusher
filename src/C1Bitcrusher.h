#ifndef C1Bitcrusher_H
#define C1Bitcrusher_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "mt19937ar.h"
#include "public.sdk/source/vst2.x/audioeffectx.h"

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
	kDitherGain,
	kMersenneTwister,
	kMersenneGenerator,
	kSeed,
	kSeedWithTime,
	kNoiseShaping,
	kNoiseShapingFilter,
	kPsychoacousticCurve,
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

struct ChannelState
{
	double LastDither;
	double error[2];
	double PsychoError[9];
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
	float Disable;
	float InGain;
	float OutGain;
	float BitDepth;
	float DCBias;
	float Dither;
	float DitherType;
	float InvertDither;
	float HighpassDither;
	float DitherGain;
	float MersenneTwister;
	float MersenneGenerator;
	float Seed;
	float SeedWithTime;
	float NoiseShaping;
	float NoiseShapingFilter;
	float PsychoacousticCurve;
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
	char ProgramName[32];
	double scale;
	ChannelState chan[2];
	double coeffs[9];
	int n;
	void Reset();
	double MT_generator();
	double RPDF();
	double TPDF();
	double AWGN_generator();
	double DitherNoise();
	double DitherSample(double sample, double *lastNoise);
	double NoiseShapeSampleFirstOrder(double sample, double noise);
	double NoiseShapeSampleSecondOrder(double sample, double noise1, double noise2);
	double NoiseShapeSamplePsycho(double sample, ChannelState *cs);
	double DCSample(double sample);
	double ClipSample(double sample);
	double QuantizeSample(double sample);
	double ProcessSample(double sample, int channel);
};

#endif
