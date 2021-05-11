#include "C1Bitcrusher.h"

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
		return (rand() / (float)RAND_MAX) * 2;
	}
}

float C1Bitcrusher::TPDF()
{
	float s1;
	float s2;
	float out;
	if (MersenneTwister >= 0.5)
	{
		s1 = MT_generator();
		s2 = MT_generator();
	}
	else
	{
		s1 = rand() / (float)RAND_MAX;
		s2 = rand() / (float)RAND_MAX;
	}
	out = s1 + s2;
	return out * 2;
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
	float noise = DitherNoise() / NumAmplitudes;
	if (InvertDither >= 0.5)
	{
		noise = noise * -1;
	}
	if (noise > 1)
	{
		noise = 1;
	}
	else if (noise < -1)
	{
		noise = -1;
	}
	return sample + (noise * DitherGain);
}

float C1Bitcrusher::NoiseShapeSample(float sample, float noise)
{
	if (sample == 0 && AutoBlank >= 0.5)
	{
		return 0;
	}
	if (noise > 1)
	{
		noise = 1;
	}
	else if (noise < -1)
	{
		noise = -1;
	}
	if (NoiseShapingFocus >= 0.5)
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
	float DC = DCBias / (NumAmplitudes / 2);
	if (DC > 1)
	{
		DC = 1;
	}
	else if (DC < -1)
	{
		DC = -1;
	}
	return sample + DC;
}

float C1Bitcrusher::ClipSample(float sample)
{
	if (sample > ClipThreshold)
	{
		sample = ClipThreshold;
	}
	else if (sample < ClipThreshold * -1)
	{
		sample = ClipThreshold * -1;
	}
	return sample;
}

float C1Bitcrusher::QuantizeSample(float sample)
{
	float scale = NumAmplitudes / 2;
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
	if (Clip0dB >= 0.5)
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
	int i;
	if (Disable >= 0.5)
	{
		for (i=0; i<sampleFrames; i++)
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
	for (i=0; i<sampleFrames; i++)
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
		if (Clip >= 0.5)
		{
			*out1 = ClipSample(*out1);
			*out2 = ClipSample(*out2);
		}
		if (Quantize >= 0.5)
		{
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
