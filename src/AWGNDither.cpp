#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef __AWGNDither__
#include "AWGNDither.h"
#endif

AWGNDither::AWGNDither (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 0, 0)
{
	// init
	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
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
		*out1 = *in1 + AWGN_generator() / 65536;
		*out2 = *in2 + AWGN_generator() / 65536;
		*in1++;
		*in2++;
		*out1++;
		*out2++;
	}
}
