#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef __AWGNDither__
#include "AWGNDither.h"
#endif

AWGNDither::AWGNDither (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 0, kNumParams)
{
	// init
	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	BitDepth = 16;
	Dither = 1;
	Quantize = 0;
}

void AWGNDither::setParameter (VstInt32 index, float value)
{
	if (index == kBitDepth)
	{
		BitDepth = value*32;
		if (BitDepth > 32) BitDepth = 32;
		else if (BitDepth < 1) BitDepth = 1;
	}
	else if (index == kDither)
	{
		Dither = value;
	}
	else if (index == kQuantize)
	{
		Quantize = value;
	}
}

void AWGNDither::setParameterAutomated (VstInt32 index, float value)
{
	if (index == kBitDepth)
	{
		BitDepth = value*32;
		if (BitDepth > 32) BitDepth = 32;
		else if (BitDepth < 1) BitDepth = 1;
	}
	else if (index == kDither)
	{
		Dither = value;
	}
	else if (index == kQuantize)
	{
		Quantize = value;
	}
}

float AWGNDither::getParameter (VstInt32 index)
{
	if (index == kBitDepth)
	{
		return BitDepth/32;
	}
	else if (index == kDither)
	{
		return Dither;
	}
	else if (index == kQuantize)
	{
		return Quantize;
	}
	return 0;
}

void AWGNDither::getParameterDisplay (VstInt32 index, char* text)
{
	if (index == kBitDepth)
	{
		float2string (BitDepth, text, kVstMaxParamStrLen);
	}
	else if (index == kDither)
	{
		if (Dither >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kQuantize)
	{
		if (Quantize >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
}

void AWGNDither::getParameterLabel (VstInt32 index, char* label)
{
	if (index == kBitDepth)
	{
		strcpy (label, "Bits");
	}
}

void AWGNDither::getParameterName (VstInt32 index, char* text)
{
	if (index == kBitDepth)
	{
		strcpy (text, "BitDepth");
	}
	else if (index == kDither)
	{
		strcpy (text, "Dither");
	}
	else if (index == kQuantize)
	{
		strcpy (text, "Quantize");
	}
}

bool AWGNDither::getEffectName (char* name)
{
	strcpy (name, "AWGNDither");
	return true;
}

bool AWGNDither::getProductString (char* text)
{
	strcpy (text, "AWGNDither");
	return true;
}

bool AWGNDither::getVendorString (char* text)
{
	strcpy (text, "Datajake");
	return true;
}

#define PI 3.1415926536

float AWGNDither::AWGN_generator()
{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

	float temp1;
	float temp2;
	float result;
	int p;

	p = 1;

	while( p > 0 )
	{
		temp2 = ( rand() / ( (float)RAND_MAX ) ); /*  rand() function generates an
													integer between 0 and  RAND_MAX,
													which is defined in stdlib.h.
												*/

		if ( temp2 == 0 )
		{// temp2 is >= (RAND_MAX / 2)
			p = 1;
		}// end if
		else
		{// temp2 is < (RAND_MAX / 2)
			p = -1;
		}// end else

	}// end while()

	temp1 = cosf( ( 2.0f * (float)PI ) * rand() / ( (float)RAND_MAX ) );
	result = sqrtf( -2.0f * logf( temp2 ) ) * temp1;

	return result;	// return the generated random sample to the caller

}// end AWGN_generator()

void AWGNDither::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	int i;
	for (i=0; i<sampleFrames; i++)
	{
		*out1 = *in1;
		*out2 = *in2;
		if (Dither >= 0.5)
		{
			float noise[2];
			noise[0] = AWGN_generator() / powf(2, BitDepth);
			*out1 = *in1 + noise[0];
			noise[1] = AWGN_generator() / powf(2, BitDepth);
			*out2 = *in2 + noise[1];
		}
		if (Quantize >= 0.5)
		{
			float quantized[2];
			if (*out1 > 1.0)
			{
				*out1 = 1.0;
			}
			else if (*out1 < -1.0)
			{
				*out1 = -1.0;
			}
			quantized[0] = *out1 * (powf(2, BitDepth) / 2);
			quantized[0] = floorf(quantized[0]);
			quantized[0] = quantized[0] / (powf(2, BitDepth) / 2);
			*out1 = quantized[0];
			if (*out2 > 1.0)
			{
				*out2 = 1.0;
			}
			else if (*out2 < -1.0)
			{
				*out2 = -1.0;
			}
			quantized[1] = *out2 * (powf(2, BitDepth) / 2);
			quantized[1] = floorf(quantized[1]);
			quantized[1] = quantized[1] / (powf(2, BitDepth) / 2);
			*out2 = quantized[1];
		}
		*in1++;
		*in2++;
		*out1++;
		*out2++;
	}
}
