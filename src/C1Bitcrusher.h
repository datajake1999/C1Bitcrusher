#ifndef __C1Bitcrusher__
#define __C1Bitcrusher__

#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	kBitDepth = 0,
	kDither,
	kDitherType,
	kNoiseShaping,
	kQuantize,
	kOnlyError,
	kAutoDither,
	kClipPreQuantization,
	kClipValue,
	kInGain,
	kOutGain,
	kDitherGain,
	kNoiseShapingGain,
	kNumParams
};

class C1Bitcrusher;

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
	virtual bool getEffectName (char* name);
	virtual bool getProductString (char* text);
	virtual bool getVendorString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
private:
	float BitDepth;
	float Dither;
	float DitherType;
	float NoiseShaping;
	float Quantize;
	float OnlyError;
	float AutoDither;
	float ClipPreQuantization;
	float ClipValue;
	float InGain;
	float OutGain;
	float DitherGain;
	float NoiseShapingGain;
	float RPDF();
	float TPDF();
	float AWGN_generator();
	float DitherNoise();
	float ClipSample(float sample, float value);
	float QuantizeSample(float sample, float depth);
	float noise[2];
	float quantized[2];
	float error[2];
};

#endif
