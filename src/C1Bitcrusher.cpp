#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "mt19937ar.h"
#ifndef __C1Bitcrusher__
#include "C1Bitcrusher.h"
#endif

C1Bitcrusher::C1Bitcrusher (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)
{
	// init
	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	Disable = 0;
	BitDepth = 16;
	DCBias = 0;
	Dither = 1;
	DitherType = 1;
	InvertDither = 0;
	DitherInError = 1;
	NoiseShaping = 1;
	NoiseShapingMode = 1;
	Quantize = 1;
	QuantizationMode = 1;
	OnlyError = 0;
	AutoBlank = 0;
	MersenneTwister = 1;
	MersenneGenerator = 1;
	Seed = 1;
	SeedWithTime = 0;
	ClipPreQuantization = 0;
	ClipValue = 1;
	ClipPostQuantization = 1;
	InGain = 1;
	OutGain = 1;
	DitherGain = 1;
	NoiseShapingGain = 1;
	NumAmplitudes = powf(2, BitDepth);
	strcpy (ProgramName, "C1Bitcrusher");
	resume ();
}

void C1Bitcrusher::resume ()
{
	if (SeedWithTime >= 0.5)
	{
		srand(time(0));
		init_genrand(time(0));
	}
	else
	{
		srand((int)Seed);
		init_genrand((int)Seed);
	}
	quantized[0] = 0;
	quantized[1] = 0;
	error[0] = 0;
	error[1] = 0;
	AudioEffectX::resume();
}

void C1Bitcrusher::setParameter (VstInt32 index, float value)
{
	switch(index)
	{
	case kDisable:
		Disable = value;
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
		NumAmplitudes = powf(2, BitDepth);
		break;
	case kDCBias:
		DCBias = (value*4.0f)-2.0f;
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
	case kDitherInError:
		DitherInError = value;
		break;
	case kNoiseShaping:
		NoiseShaping = value;
		break;
	case kNoiseShapingMode:
		NoiseShapingMode = value;
		break;
	case kQuantize:
		Quantize = value;
		break;
	case kQuantizationMode:
		QuantizationMode = value;
		break;
	case kOnlyError:
		OnlyError = value;
		break;
	case kAutoBlank:
		AutoBlank = value;
		break;
	case kMersenneTwister:
		MersenneTwister = value;
		break;
	case kMersenneGenerator:
		MersenneGenerator = value;
		break;
	case kSeed:
		Seed = value*1000;
		if (Seed < 1)
		{
			Seed = 1;
		}
		break;
	case kSeedWithTime:
		SeedWithTime = value;
		break;
	case kClipPreQuantization:
		ClipPreQuantization = value;
		break;
	case kClipValue:
		ClipValue = value;
		if (ClipValue < 0)
		{
			ClipValue = 0;
		}
		break;
	case kClipPostQuantization:
		ClipPostQuantization = value;
		break;
	case kInGain:
		InGain = value;
		break;
	case kOutGain:
		OutGain = value;
		break;
	case kDitherGain:
		DitherGain = value;
		break;
	case kNoiseShapingGain:
		NoiseShapingGain = value;
		break;
	}
}

void C1Bitcrusher::setParameterAutomated (VstInt32 index, float value)
{
	setParameter (index, value);
}

float C1Bitcrusher::getParameter (VstInt32 index)
{
	float value = 0;
	switch(index)
	{
	case kDisable:
		value = Disable;
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
	case kDitherInError:
		value = DitherInError;
		break;
	case kNoiseShaping:
		value = NoiseShaping;
		break;
	case kNoiseShapingMode:
		value = NoiseShapingMode;
		break;
	case kQuantize:
		value = Quantize;
		break;
	case kQuantizationMode:
		value = QuantizationMode;
		break;
	case kOnlyError:
		value = OnlyError;
		break;
	case kAutoBlank:
		value = AutoBlank;
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
	case kClipPreQuantization:
		value = ClipPreQuantization;
		break;
	case kClipValue:
		value = ClipValue;
		break;
	case kClipPostQuantization:
		value = ClipPostQuantization;
		break;
	case kInGain:
		value = InGain;
		break;
	case kOutGain:
		value = OutGain;
		break;
	case kDitherGain:
		value = DitherGain;
		break;
	case kNoiseShapingGain:
		value = NoiseShapingGain;
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
	case kNoiseShapingMode:
		if (NoiseShapingMode >= 0.5)
		{
			strcpy (text, "High");
		}
		else
		{
			strcpy (text, "Low");
		}
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
	case kClipPreQuantization:
		if (ClipPreQuantization >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kClipValue:
		dB2string (ClipValue, text, kVstMaxParamStrLen);
		break;
	case kClipPostQuantization:
		if (ClipPostQuantization >= 0.5)
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
	case kDitherGain:
		float2string (DitherGain, text, kVstMaxParamStrLen);
		break;
	case kNoiseShapingGain:
		float2string (NoiseShapingGain, text, kVstMaxParamStrLen);
		break;
	}
}

void C1Bitcrusher::getParameterLabel (VstInt32 index, char* label)
{
	switch(index)
	{
	case kBitDepth:
		strcpy (label, "Bits");
		break;
	case kDCBias:
		strcpy (label, "LSB");
		break;
	case kSeed:
		strcpy (label, "I");
		break;
	case kClipValue:
		strcpy (label, "dB");
		break;
	case kInGain:
		strcpy (label, "F");
		break;
	case kOutGain:
		strcpy (label, "F");
		break;
	case kDitherGain:
		strcpy (label, "F");
		break;
	case kNoiseShapingGain:
		strcpy (label, "F");
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
	case kDitherInError:
		strcpy (text, "DitherInError");
		break;
	case kNoiseShaping:
		strcpy (text, "NoiseShaping");
		break;
	case kNoiseShapingMode:
		strcpy (text, "NoiseShapingMode");
		break;
	case kQuantize:
		strcpy (text, "Quantize");
		break;
	case kQuantizationMode:
		strcpy (text, "QuantizationMode");
		break;
	case kOnlyError:
		strcpy (text, "OnlyError");
		break;
	case kAutoBlank:
		strcpy (text, "AutoBlank");
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
	case kClipPreQuantization:
		strcpy (text, "ClipPreQuantization");
		break;
	case kClipValue:
		strcpy (text, "ClipValue");
		break;
	case kClipPostQuantization:
		strcpy (text, "ClipPostQuantization");
		break;
	case kInGain:
		strcpy (text, "InGain");
		break;
	case kOutGain:
		strcpy (text, "OutGain");
		break;
	case kDitherGain:
		strcpy (text, "DitherGain");
		break;
	case kNoiseShapingGain:
		strcpy (text, "NoiseShapingGain");
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

float C1Bitcrusher::MT_generator()
{
	if (MersenneGenerator >= 0.0 && MersenneGenerator < 0.25)
	{
		return genrand_real1();
	}
	else if (MersenneGenerator >= 0.25 && MersenneGenerator < 0.5)
	{
		return genrand_real2();
	}
	else
	{
		return genrand_real3();
	}
}

float C1Bitcrusher::RPDF()
{
	if (MersenneTwister >= 0.5)
	{
		return MT_generator() * 2;
	}
	else
	{
		return rand() / (float)(RAND_MAX/2);
	}
}

float C1Bitcrusher::TPDF()
{
	if (MersenneTwister >= 0.5)
	{
		float s1 = MT_generator() * 2;
		float s2 = MT_generator() * 2;
		return s1 + s2;
	}
	else
	{
		float s1 = rand() / (float)(RAND_MAX/2);
		float s2 = rand() / (float)(RAND_MAX/2);
		return s1 + s2;
	}
}

#define PI 3.1415926536

float C1Bitcrusher::AWGN_generator()
{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

	float temp1;
	float temp2;
	float result;
	int p;

	p = 1;

	while( p > 0 )
	{
		if (MersenneTwister >= 0.5)
		{
			temp2 = ( MT_generator() );
		}
		else
		{
			temp2 = ( rand() / ( (float)RAND_MAX ) ); /*  rand() function generates an
													integer between 0 and  RAND_MAX,
													which is defined in stdlib.h.
												*/
		}

		if ( temp2 == 0 )
		{// temp2 is >= (RAND_MAX / 2)
			p = 1;
		}// end if
		else
		{// temp2 is < (RAND_MAX / 2)
			p = -1;
		}// end else

	}// end while()

	if (MersenneTwister >= 0.5)
	{
		temp1 = cosf( ( 2.0f * (float)PI ) * MT_generator() );
	}
	else
	{
		temp1 = cosf( ( 2.0f * (float)PI ) * rand() / ( (float)RAND_MAX ) );
	}
	result = sqrtf( -2.0f * logf( temp2 ) ) * temp1;

	return result;	// return the generated random sample to the caller

}// end AWGN_generator()

float C1Bitcrusher::DitherNoise()
{
	if (DitherType >= 0.0 && DitherType < 0.25)
	{
		return RPDF();
	}
	else if (DitherType >= 0.25 && DitherType < 0.5)
	{
		return TPDF();
	}
	else
	{
		return AWGN_generator();
	}
}

float C1Bitcrusher::DitherSample(float sample)
{
	if (sample == 0 && AutoBlank >= 0.5)
	{
		return 0;
	}
	float noise = (DitherNoise() / NumAmplitudes) * DitherGain;
	if (InvertDither >= 0.5)
	{
		noise = noise * -1;
	}
	return sample + noise;
}

float C1Bitcrusher::NoiseShapeSample(float sample, float noise)
{
	if (sample == 0 && AutoBlank >= 0.5)
	{
		return 0;
	}
	if (NoiseShapingMode >= 0.5)
	{
		return sample - (noise * NoiseShapingGain);
	}
	else
	{
		return sample + (noise * NoiseShapingGain);
	}
}

float C1Bitcrusher::DCSample(float sample)
{
	return sample + (DCBias / (NumAmplitudes / 2));
}

float C1Bitcrusher::ClipSample(float sample)
{
	if (sample > ClipValue)
	{
		sample = ClipValue;
	}
	else if (sample < ClipValue * -1)
	{
		sample = ClipValue * -1;
	}
	return sample;
}

float C1Bitcrusher::QuantizeSample(float sample)
{
	float scale = NumAmplitudes/2;
	sample = sample * scale;
	if (QuantizationMode >= 0.0 && QuantizationMode < 0.25)
	{
		sample = floorf(sample);
	}
	else if (QuantizationMode >= 0.25 && QuantizationMode < 0.5)
	{
		sample = ceilf(sample);
	}
	else if (QuantizationMode >= 0.5 && QuantizationMode < 0.75)
	{
		if (sample >= 0)
		{
			sample = floorf(sample);
		}
		else
		{
			sample = ceilf(sample);
		}
	}
	else
	{
		if (sample >= 0)
		{
			sample = floorf(sample + 0.5f);
		}
		else
		{
			sample = ceilf(sample - 0.5f);
		}
	}
	if (ClipPostQuantization >= 0.5)
	{
		if (sample > scale - 1)
		{
			sample = scale - 1;
		}
		else if (sample < scale * -1)
		{
			sample = scale * -1;
		}
	}
	sample = sample / scale;
	return sample;
}

void C1Bitcrusher::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	if (Disable >= 0.5)
	{
		for (int i=0; i<sampleFrames; i++)
		{
			*out1 = *in1;
			*out2 = *in2;
			*in1++;
			*in2++;
			*out1++;
			*out2++;
		}
		return;
	}
	for (int i=0; i<sampleFrames; i++)
	{
		*out1 = *in1 * InGain;
		*out2 = *in2 * InGain;
		*out1 = DCSample(*out1);
		*out2 = DCSample(*out2);
		if (Dither >= 0.5 && DitherInError < 0.5)
		{
			*out1 = DitherSample(*out1);
			*out2 = DitherSample(*out2);
		}
		if (Quantize >= 0.5)
		{
			if (ClipPreQuantization >= 0.5)
			{
				*out1 = ClipSample(*out1);
				*out2 = ClipSample(*out2);
			}
			if (NoiseShaping >= 0.5)
			{
				*out1 = NoiseShapeSample(*out1, error[0]);
				*out2 = NoiseShapeSample(*out2, error[1]);
			}
			if (Dither >= 0.5 && DitherInError >= 0.5)
			{
				quantized[0] = QuantizeSample(DitherSample(*out1));
				quantized[1] = QuantizeSample(DitherSample(*out2));
			}
			else
			{
				quantized[0] = QuantizeSample(*out1);
				quantized[1] = QuantizeSample(*out2);
			}
			error[0] = quantized[0] - *out1;
			error[1] = quantized[1] - *out2;
			if (OnlyError >= 0.5)
			{
				*out1 = error[0];
				*out2 = error[1];
			}
			else
			{
				*out1 = quantized[0];
				*out2 = quantized[1];
			}
		}
		*out1 = *out1 * OutGain;
		*out2 = *out2 * OutGain;
		*in1++;
		*in2++;
		*out1++;
		*out2++;
	}
}
