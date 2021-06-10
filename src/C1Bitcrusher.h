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
	char ProgramName[32];
	double NumAmplitudes;
	double scale;
	double quantized[2];
	double error[2];
	void Reset();
	double MT_generator();
	double RPDF();
	double TPDF();
	double AWGN_generator();
	double DitherNoise();
	double DitherSample(double sample);
	double NoiseShapeSample(double sample, double noise);
	double DCSample(double sample);
	double ClipSample(double sample);
	double QuantizeSample(double sample);
	double ProcessSample(double sample, int channel);
};

#endif
