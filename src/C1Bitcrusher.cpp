#include "C1Bitcrusher.h"

C1Bitcrusher::C1Bitcrusher (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	setUniqueID ('C1BC');	// identify
	Disable = 0;
	InGain = 1;
	OutGain = 1;
	BitDepth = 16;
	DCBias = 0;
	Dither = 1;
	DitherType = 1;
	InvertDither = 0;
	DitherGain = 1;
	MersenneTwister = 1;
	MersenneGenerator = 1;
	Seed = 0;
	SeedWithTime = 0;
	NoiseShaping = 1;
	NoiseShapingFocus = 1;
	NoiseShapingGain = 1;
	AutoBlank = 0;
	Clip = 0;
	ClipThreshold = 1;
	Quantize = 1;
	QuantizationMode = 1;
	Clip0dB = 1;
	DitherInError = 1;
	OnlyError = 0;
	NumAmplitudes = pow((double)2, (double)BitDepth);
	strcpy (ProgramName, "C1Bitcrusher");
	canProcessReplacing ();
	canDoubleReplacing ();
	Reset();
}

void C1Bitcrusher::setParameter (VstInt32 index, float value)
{
	switch(index)
	{
	case kDisable:
		Disable = value;
		break;
	case kInGain:
		InGain = value;
		break;
	case kOutGain:
		OutGain = value;
		break;
	case kBitDepth:
		BitDepth = value*32;
		if (BitDepth > 32)
		{
			BitDepth = 32;
		}
		else if (BitDepth < 1)
		{
			BitDepth = 1;
		}
		NumAmplitudes = pow((double)2, (double)BitDepth);
		break;
	case kDCBias:
		DCBias = (value*4.0f)-2.0f;
		if (DCBias > 2)
		{
			DCBias = 2;
		}
		else if (DCBias < -2)
		{
			DCBias = -2;
		}
		break;
	case kDither:
		Dither = value;
		break;
	case kDitherType:
		DitherType = value;
		break;
	case kInvertDither:
		InvertDither = value;
		break;
	case kDitherGain:
		DitherGain = value;
		break;
	case kMersenneTwister:
		MersenneTwister = value;
		break;
	case kMersenneGenerator:
		MersenneGenerator = value;
		break;
	case kSeed:
		Seed = value*1000;
		if (Seed < 0)
		{
			Seed = 0;
		}
		break;
	case kSeedWithTime:
		SeedWithTime = value;
		break;
	case kNoiseShaping:
		NoiseShaping = value;
		break;
	case kNoiseShapingFocus:
		NoiseShapingFocus = value;
		break;
	case kNoiseShapingGain:
		NoiseShapingGain = value;
		break;
	case kAutoBlank:
		AutoBlank = value;
		break;
	case kClip:
		Clip = value;
		break;
	case kClipThreshold:
		ClipThreshold = value;
		if (ClipThreshold < 0)
		{
			ClipThreshold = 0;
		}
		break;
	case kQuantize:
		Quantize = value;
		break;
	case kQuantizationMode:
		QuantizationMode = value;
		break;
	case kClip0dB:
		Clip0dB = value;
		break;
	case kDitherInError:
		DitherInError = value;
		break;
	case kOnlyError:
		OnlyError = value;
		break;
	}
}

float C1Bitcrusher::getParameter (VstInt32 index)
{
	float value = 0;
	switch(index)
	{
	case kDisable:
		value = Disable;
		break;
	case kInGain:
		value = InGain;
		break;
	case kOutGain:
		value = OutGain;
		break;
	case kBitDepth:
		value = BitDepth/32;
		break;
	case kDCBias:
		value = (DCBias+2.0f)/4.0f;
		break;
	case kDither:
		value = Dither;
		break;
	case kDitherType:
		value = DitherType;
		break;
	case kInvertDither:
		value = InvertDither;
		break;
	case kDitherGain:
		value = DitherGain;
		break;
	case kMersenneTwister:
		value = MersenneTwister;
		break;
	case kMersenneGenerator:
		value = MersenneGenerator;
		break;
	case kSeed:
		value = Seed/1000;
		break;
	case kSeedWithTime:
		value = SeedWithTime;
		break;
	case kNoiseShaping:
		value = NoiseShaping;
		break;
	case kNoiseShapingFocus:
		value = NoiseShapingFocus;
		break;
	case kNoiseShapingGain:
		value = NoiseShapingGain;
		break;
	case kAutoBlank:
		value = AutoBlank;
		break;
	case kClip:
		value = Clip;
		break;
	case kClipThreshold:
		value = ClipThreshold;
		break;
	case kQuantize:
		value = Quantize;
		break;
	case kQuantizationMode:
		value = QuantizationMode;
		break;
	case kClip0dB:
		value = Clip0dB;
		break;
	case kDitherInError:
		value = DitherInError;
		break;
	case kOnlyError:
		value = OnlyError;
		break;
	}
	return value;
}

void C1Bitcrusher::getParameterDisplay (VstInt32 index, char* text)
{
	switch(index)
	{
	case kDisable:
		if (Disable >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kInGain:
		float2string (InGain, text, kVstMaxParamStrLen);
		break;
	case kOutGain:
		float2string (OutGain, text, kVstMaxParamStrLen);
		break;
	case kBitDepth:
		float2string (BitDepth, text, kVstMaxParamStrLen);
		break;
	case kDCBias:
		float2string (DCBias, text, kVstMaxParamStrLen);
		break;
	case kDither:
		if (Dither >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kDitherType:
		if (DitherType >= 0.0 && DitherType < 0.25)
		{
			strcpy (text, "Rectangular");
		}
		else if (DitherType >= 0.25 && DitherType < 0.5)
		{
			strcpy (text, "Triangular");
		}
		else
		{
			strcpy (text, "Gaussian");
		}
		break;
	case kInvertDither:
		if (InvertDither >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kDitherGain:
		float2string (DitherGain, text, kVstMaxParamStrLen);
		break;
	case kMersenneTwister:
		if (MersenneTwister >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kMersenneGenerator:
		if (MersenneGenerator >= 0.0 && MersenneGenerator < 0.25)
		{
			strcpy (text, "Generator1");
		}
		else if (MersenneGenerator >= 0.25 && MersenneGenerator < 0.5)
		{
			strcpy (text, "Generator2");
		}
		else
		{
			strcpy (text, "Generator3");
		}
		break;
	case kSeed:
		int2string ((int)Seed, text, kVstMaxParamStrLen);
		break;
	case kSeedWithTime:
		if (SeedWithTime >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kNoiseShaping:
		if (NoiseShaping >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kNoiseShapingFocus:
		if (NoiseShapingFocus >= 0.5)
		{
			strcpy (text, "High");
		}
		else
		{
			strcpy (text, "Low");
		}
		break;
	case kNoiseShapingGain:
		float2string (NoiseShapingGain, text, kVstMaxParamStrLen);
		break;
	case kAutoBlank:
		if (AutoBlank >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kClip:
		if (Clip >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kClipThreshold:
		dB2string (ClipThreshold, text, kVstMaxParamStrLen);
		break;
	case kQuantize:
		if (Quantize >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kQuantizationMode:
		if (QuantizationMode >= 0.0 && QuantizationMode < 0.25)
		{
			strcpy (text, "Floor");
		}
		else if (QuantizationMode >= 0.25 && QuantizationMode < 0.5)
		{
			strcpy (text, "Ceiling");
		}
		else if (QuantizationMode >= 0.5 && QuantizationMode < 0.75)
		{
			strcpy (text, "Truncate");
		}
		else
		{
			strcpy (text, "Round");
		}
		break;
	case kClip0dB:
		if (Clip0dB >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kDitherInError:
		if (DitherInError >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kOnlyError:
		if (OnlyError >= 0.5)
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
	switch(index)
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
	switch(index)
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
	case kNoiseShapingFocus:
		strcpy (text, "NoiseShapingFocus");
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
	Reset();
}

void C1Bitcrusher::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	int i;
	for (i=0; i<sampleFrames; i++)
	{
		*out1 = (float)ProcessSample(*in1, 0);
		*out2 = (float)ProcessSample(*in2, 1);
		*in1++;
		*in2++;
		*out1++;
		*out2++;
	}
}

void C1Bitcrusher::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];
	int i;
	for (i=0; i<sampleFrames; i++)
	{
		*out1 = ProcessSample(*in1, 0);
		*out2 = ProcessSample(*in2, 1);
		*in1++;
		*in2++;
		*out1++;
		*out2++;
	}
}
