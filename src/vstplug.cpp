#include "vstplug.h"

C1Bitcrusher::C1Bitcrusher (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	setUniqueID ('C1BC');	// identify
	canProcessReplacing ();
	canDoubleReplacing ();
	strcpy (ProgramName, "Default");
	C1Init(&state);
	C1GetSettings(&state, &settings);
	C1ResetChannel(&channel[0]);
	C1ResetChannel(&channel[1]);
}

void C1Bitcrusher::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
	case kDisable:
		settings.Disable = (int)value;
		break;
	case kInGain:
		settings.InGain = value;
		break;
	case kOutGain:
		settings.OutGain = value;
		break;
	case kBitDepth:
		settings.BitDepth = value*32.0f;
		break;
	case kDCBias:
		settings.DCBias = (value*4.0f)-2.0f;
		break;
	case kDither:
		settings.Dither = (int)value;
		break;
	case kDitherType:
		settings.DitherType = (int)(value*(kNumDitherTypes-1));
		break;
	case kInvertDither:
		settings.InvertDither = (int)value;
		break;
	case kHighpassDither:
		settings.HighpassDither = (int)value;
		break;
	case kHighpassGain:
		settings.HighpassGain = value;
		break;
	case kDitherGain:
		settings.DitherGain = value;
		break;
	case kMersenneTwister:
		settings.MersenneTwister = (int)value;
		break;
	case kMersenneGenerator:
		settings.MersenneGenerator = (int)(value*(kNumMersenneGenerators-1));
		break;
	case kSeed:
		settings.Seed = (int)(value*1000);
		break;
	case kSeedWithTime:
		settings.SeedWithTime = (int)value;
		break;
	case kNoiseShaping:
		settings.NoiseShaping = (int)value;
		break;
	case kNoiseShapingFilter:
		settings.NoiseShapingFilter = (int)(value*(kNumNoiseShapingFilters-1));
		break;
	case kPsychoacousticCurve:
		settings.PsychoacousticCurve = (int)(value*(kNumPsychoacousticCurves-1));
		break;
	case kNoiseShapingGain:
		settings.NoiseShapingGain = value;
		break;
	case kAutoBlank:
		settings.AutoBlank = (int)value;
		break;
	case kClip:
		settings.Clip = (int)value;
		break;
	case kClipThreshold:
		settings.ClipThreshold = value;
		break;
	case kQuantize:
		settings.Quantize = (int)value;
		break;
	case kQuantizationMode:
		settings.QuantizationMode = (int)(value*(kNumQuantizationModes-1));
		break;
	case kClip0dB:
		settings.Clip0dB = (int)value;
		break;
	case kDitherInError:
		settings.DitherInError = (int)value;
		break;
	case kOnlyError:
		settings.OnlyError = (int)value;
		break;
	case kNormalizeError:
		settings.NormalizeError = (int)value;
		break;
	}
	C1LoadSettings(&state, &settings);
}

float C1Bitcrusher::getParameter (VstInt32 index)
{
	float value = 0;
	C1GetSettings(&state, &settings);
	switch (index)
	{
	case kDisable:
		value = settings.Disable;
		break;
	case kInGain:
		value = (float)settings.InGain;
		break;
	case kOutGain:
		value = (float)settings.OutGain;
		break;
	case kBitDepth:
		value = settings.BitDepth/32.0f;
		break;
	case kDCBias:
		value = (settings.DCBias+2.0f)/4.0f;
		break;
	case kDither:
		value = settings.Dither;
		break;
	case kDitherType:
		value = settings.DitherType/(float)(kNumDitherTypes-1);
		break;
	case kInvertDither:
		value = settings.InvertDither;
		break;
	case kHighpassDither:
		value = settings.HighpassDither;
		break;
	case kHighpassGain:
		value = (float)settings.HighpassGain;
		break;
	case kDitherGain:
		value = (float)settings.DitherGain;
		break;
	case kMersenneTwister:
		value = settings.MersenneTwister;
		break;
	case kMersenneGenerator:
		value = settings.MersenneGenerator/(float)(kNumMersenneGenerators-1);
		break;
	case kSeed:
		value = settings.Seed/1000.0f;
		break;
	case kSeedWithTime:
		value = settings.SeedWithTime;
		break;
	case kNoiseShaping:
		value = settings.NoiseShaping;
		break;
	case kNoiseShapingFilter:
		value = settings.NoiseShapingFilter/(float)(kNumNoiseShapingFilters-1);
		break;
	case kPsychoacousticCurve:
		value = settings.PsychoacousticCurve/(float)(kNumPsychoacousticCurves-1);
		break;
	case kNoiseShapingGain:
		value = (float)settings.NoiseShapingGain;
		break;
	case kAutoBlank:
		value = settings.AutoBlank;
		break;
	case kClip:
		value = settings.Clip;
		break;
	case kClipThreshold:
		value = (float)settings.ClipThreshold;
		break;
	case kQuantize:
		value = settings.Quantize;
		break;
	case kQuantizationMode:
		value = settings.QuantizationMode/(float)(kNumQuantizationModes-1);
		break;
	case kClip0dB:
		value = settings.Clip0dB;
		break;
	case kDitherInError:
		value = settings.DitherInError;
		break;
	case kOnlyError:
		value = settings.OnlyError;
		break;
	case kNormalizeError:
		value = settings.NormalizeError;
		break;
	}
	return value;
}

void C1Bitcrusher::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index)
	{
	case kDisable:
		if (settings.Disable)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kInGain:
		float2string ((float)settings.InGain, text, kVstMaxParamStrLen);
		break;
	case kOutGain:
		float2string ((float)settings.OutGain, text, kVstMaxParamStrLen);
		break;
	case kBitDepth:
		float2string ((float)settings.BitDepth, text, kVstMaxParamStrLen);
		break;
	case kDCBias:
		float2string ((float)settings.DCBias, text, kVstMaxParamStrLen);
		break;
	case kDither:
		if (settings.Dither)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kDitherType:
		switch (settings.DitherType)
		{
		case kRectangular:
			strcpy (text, "Rectangular");
			break;
		case kTriangular:
			strcpy (text, "Triangular");
			break;
		case kGaussian:
			strcpy (text, "Gaussian");
			break;
		}
		break;
	case kInvertDither:
		if (settings.InvertDither)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kHighpassDither:
		if (settings.HighpassDither)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kHighpassGain:
		float2string ((float)settings.HighpassGain, text, kVstMaxParamStrLen);
		break;
	case kDitherGain:
		float2string ((float)settings.DitherGain, text, kVstMaxParamStrLen);
		break;
	case kMersenneTwister:
		if (settings.MersenneTwister)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kMersenneGenerator:
		switch (settings.MersenneGenerator)
		{
		case kGenerator1:
			strcpy (text, "Generator1");
			break;
		case kGenerator2:
			strcpy (text, "Generator2");
			break;
		case kGenerator3:
			strcpy (text, "Generator3");
			break;
		}
		break;
	case kSeed:
		int2string (settings.Seed, text, kVstMaxParamStrLen);
		break;
	case kSeedWithTime:
		if (settings.SeedWithTime)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kNoiseShaping:
		if (settings.NoiseShaping)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kNoiseShapingFilter:
		switch (settings.NoiseShapingFilter)
		{
		case kFirstOrder:
			strcpy (text, "First Order");
			break;
		case kSecondOrder:
			strcpy (text, "Second Order");
			break;
		case kPsychoacoustic:
			strcpy (text, "Psychoacoustic");
			break;
		}
		break;
	case kPsychoacousticCurve:
		switch (settings.PsychoacousticCurve)
		{
		case kWannamaker3Tap:
			strcpy (text, "Wannamaker 3-tap");
			break;
		case kLipshitz:
			strcpy (text, "Lipshitz");
			break;
		case kWannamaker9Tap:
			strcpy (text, "Wannamaker 9-tap");
			break;
		}
		break;
	case kNoiseShapingGain:
		float2string ((float)settings.NoiseShapingGain, text, kVstMaxParamStrLen);
		break;
	case kAutoBlank:
		if (settings.AutoBlank)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kClip:
		if (settings.Clip)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kClipThreshold:
		dB2string ((float)settings.ClipThreshold, text, kVstMaxParamStrLen);
		break;
	case kQuantize:
		if (settings.Quantize)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kQuantizationMode:
		switch (settings.QuantizationMode)
		{
		case kFloor:
			strcpy (text, "Floor");
			break;
		case kCeiling:
			strcpy (text, "Ceiling");
			break;
		case kTruncate:
			strcpy (text, "Truncate");
			break;
		case kRound:
			strcpy (text, "Round");
			break;
		}
		break;
	case kClip0dB:
		if (settings.Clip0dB)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kDitherInError:
		if (settings.DitherInError)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kOnlyError:
		if (settings.OnlyError)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kNormalizeError:
		if (settings.NormalizeError)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	}
}

void C1Bitcrusher::getParameterLabel (VstInt32 index, char* label)
{
	switch (index)
	{
	case kInGain:
		strcpy (label, "F");
		break;
	case kOutGain:
		strcpy (label, "F");
		break;
	case kBitDepth:
		strcpy (label, "Bits");
		break;
	case kDCBias:
		strcpy (label, "LSB");
		break;
	case kHighpassGain:
		strcpy (label, "F");
		break;
	case kDitherGain:
		strcpy (label, "F");
		break;
	case kSeed:
		strcpy (label, "I");
		break;
	case kNoiseShapingGain:
		strcpy (label, "F");
		break;
	case kClipThreshold:
		strcpy (label, "dB");
		break;
	}
}

void C1Bitcrusher::getParameterName (VstInt32 index, char* text)
{
	switch (index)
	{
	case kDisable:
		strcpy (text, "Disable");
		break;
	case kInGain:
		strcpy (text, "InGain");
		break;
	case kOutGain:
		strcpy (text, "OutGain");
		break;
	case kBitDepth:
		strcpy (text, "BitDepth");
		break;
	case kDCBias:
		strcpy (text, "DCBias");
		break;
	case kDither:
		strcpy (text, "Dither");
		break;
	case kDitherType:
		strcpy (text, "DitherType");
		break;
	case kInvertDither:
		strcpy (text, "InvertDither");
		break;
	case kHighpassDither:
		strcpy (text, "HighpassDither");
		break;
	case kHighpassGain:
		strcpy (text, "HighpassGain");
		break;
	case kDitherGain:
		strcpy (text, "DitherGain");
		break;
	case kMersenneTwister:
		strcpy (text, "MersenneTwister");
		break;
	case kMersenneGenerator:
		strcpy (text, "MersenneGenerator");
		break;
	case kSeed:
		strcpy (text, "Seed");
		break;
	case kSeedWithTime:
		strcpy (text, "SeedWithTime");
		break;
	case kNoiseShaping:
		strcpy (text, "NoiseShaping");
		break;
	case kNoiseShapingFilter:
		strcpy (text, "NoiseShapingFilter");
		break;
	case kPsychoacousticCurve:
		strcpy (text, "PsychoacousticCurve");
		break;
	case kNoiseShapingGain:
		strcpy (text, "NoiseShapingGain");
		break;
	case kAutoBlank:
		strcpy (text, "AutoBlank");
		break;
	case kClip:
		strcpy (text, "Clip");
		break;
	case kClipThreshold:
		strcpy (text, "ClipThreshold");
		break;
	case kQuantize:
		strcpy (text, "Quantize");
		break;
	case kQuantizationMode:
		strcpy (text, "QuantizationMode");
		break;
	case kClip0dB:
		strcpy (text, "Clip0dB");
		break;
	case kDitherInError:
		strcpy (text, "DitherInError");
		break;
	case kOnlyError:
		strcpy (text, "OnlyError");
		break;
	case kNormalizeError:
		strcpy (text, "NormalizeError");
		break;
	}
}

void C1Bitcrusher::setProgramName (char *name)
{
	strcpy (ProgramName, name);
}

void C1Bitcrusher::getProgramName (char *name)
{
	strcpy (name, ProgramName);
}

bool C1Bitcrusher::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index == 0)
	{
		strcpy (text, ProgramName);
		return true;
	}
	return false;
}

bool C1Bitcrusher::getEffectName (char* name)
{
	strcpy (name, "C1Bitcrusher");
	return true;
}

bool C1Bitcrusher::getProductString (char* text)
{
	strcpy (text, "C1Bitcrusher");
	return true;
}

bool C1Bitcrusher::getVendorString (char* text)
{
	strcpy (text, "Datajake");
	return true;
}

VstInt32 C1Bitcrusher::getVendorVersion ()
{
	return 1000;
}

VstPlugCategory C1Bitcrusher::getPlugCategory ()
{
	return kPlugCategEffect;
}

void C1Bitcrusher::resume ()
{
	C1ResetPRNG(&state);
	C1ResetChannel(&channel[0]);
	C1ResetChannel(&channel[1]);
}

void C1Bitcrusher::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	int i;
	if (in1 && out1)
	{
		for (i = 0; i < sampleFrames; i++)
		{
			out1[i] = (float)C1ProcessSample(&state, &channel[0], in1[i]);
		}
	}
	if (in2 && out2)
	{
		for (i = 0; i < sampleFrames; i++)
		{
			out2[i] = (float)C1ProcessSample(&state, &channel[1], in2[i]);
		}
	}
}

void C1Bitcrusher::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];
	int i;
	if (in1 && out1)
	{
		for (i = 0; i < sampleFrames; i++)
		{
			out1[i] = C1ProcessSample(&state, &channel[0], in1[i]);
		}
	}
	if (in2 && out2)
	{
		for (i = 0; i < sampleFrames; i++)
		{
			out2[i] = C1ProcessSample(&state, &channel[1], in2[i]);
		}
	}
}
