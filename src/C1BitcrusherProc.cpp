#include "C1Bitcrusher.h"

void C1Bitcrusher::Reset()
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
	memset(quantized, 0, sizeof(quantized));
	memset(error, 0, sizeof(error));
}

double C1Bitcrusher::MT_generator()
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

double C1Bitcrusher::RPDF()
{
	double s;
	if (MersenneTwister >= 0.5)
	{
		s = MT_generator();
	}
	else
	{
		s = rand() / (double)RAND_MAX;
	}
	s = (s - 0.5) * 2;
	return s;
}

double C1Bitcrusher::TPDF()
{
	double s1;
	double s2;
	double out;
	if (MersenneTwister >= 0.5)
	{
		s1 = MT_generator();
		s2 = MT_generator();
	}
	else
	{
		s1 = rand() / (double)RAND_MAX;
		s2 = rand() / (double)RAND_MAX;
	}
	s1 = (s1 - 0.5) * 2;
	s2 = (s2 - 0.5) * 2;
	out = s1 + s2;
	return out;
}

#define PI 3.1415926536

double C1Bitcrusher::AWGN_generator()
{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

	double temp1;
	double temp2;
	double result;
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
			temp2 = ( rand() / ( (double)RAND_MAX ) ); /*  rand() function generates an
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
		temp1 = cos( ( 2.0 * (double)PI ) * MT_generator() );
	}
	else
	{
		temp1 = cos( ( 2.0 * (double)PI ) * rand() / ( (double)RAND_MAX ) );
	}
	result = sqrt( -2.0 * log( temp2 ) ) * temp1;

	return result;	// return the generated random sample to the caller

}// end AWGN_generator()

double C1Bitcrusher::DitherNoise()
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

double C1Bitcrusher::DitherSample(double sample, double *lastNoise)
{
	if (sample == 0 && AutoBlank >= 0.5)
	{
		return 0;
	}
	double noise = DitherNoise() / scale;
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
	if (HighpassDither >= 0.5)
	{
		double noise2 = 0;
		if (lastNoise)
		{
			noise2 = *lastNoise;
			*lastNoise = noise;
		}
		noise = noise - noise2;
	}
	return sample + (noise * DitherGain);
}

double C1Bitcrusher::NoiseShapeSampleFirstOrder(double sample, double noise)
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

double C1Bitcrusher::NoiseShapeSampleSecondOrder(double sample, double noise1, double noise2)
{
	double noiseSum;
	if (sample == 0 && AutoBlank >= 0.5)
	{
		return 0;
	}
	if (noise1 > 1)
	{
		noise1 = 1;
	}
	else if (noise1 < -1)
	{
		noise1 = -1;
	}
	if (noise2 > 1)
	{
		noise2 = 1;
	}
	else if (noise2 < -1)
	{
		noise2 = -1;
	}
	noiseSum = noise1 * 2;
	if (NoiseShapingFocus >= 0.5)
	{
		noiseSum = noiseSum - noise2;
		return sample - (noiseSum * NoiseShapingGain);
	}
	else
	{
		noiseSum = noiseSum + noise2;
		return sample + (noiseSum * NoiseShapingGain);
	}
}

double C1Bitcrusher::DCSample(double sample)
{
	double DC = DCBias / scale;
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

double C1Bitcrusher::ClipSample(double sample)
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

double C1Bitcrusher::QuantizeSample(double sample)
{
	sample = sample * scale;
	if (QuantizationMode >= 0.0 && QuantizationMode < 0.25)
	{
		sample = floor(sample);
	}
	else if (QuantizationMode >= 0.25 && QuantizationMode < 0.5)
	{
		sample = ceil(sample);
	}
	else if (QuantizationMode >= 0.5 && QuantizationMode < 0.75)
	{
		if (sample >= 0)
		{
			sample = floor(sample);
		}
		else
		{
			sample = ceil(sample);
		}
	}
	else
	{
		if (sample >= 0)
		{
			sample = floor(sample + 0.5);
		}
		else
		{
			sample = ceil(sample - 0.5);
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

double C1Bitcrusher::ProcessSample(double sample, int channel)
{
	if (channel > 1)
	{
		channel = 1;
	}
	else if (channel < 0)
	{
		channel = 0;
	}
	if (Disable >= 0.5)
	{
		return sample;
	}
	sample = sample * InGain;
	sample = DCSample(sample);
	if (Dither >= 0.5 && DitherInError < 0.5)
	{
		sample = DitherSample(sample, &LastDither[channel]);
	}
	if (Clip >= 0.5)
	{
		sample = ClipSample(sample);
	}
	if (Quantize >= 0.5)
	{
		if (NoiseShaping >= 0.5)
		{
			if (NoiseShapingOrder >= 0.5)
			{
				sample = NoiseShapeSampleSecondOrder(sample, error[0][channel], error[1][channel]);
			}
			else
			{
				sample = NoiseShapeSampleFirstOrder(sample, error[0][channel]);
			}
		}
		if (Dither >= 0.5 && DitherInError >= 0.5)
		{
			quantized[channel] = QuantizeSample(DitherSample(sample, &LastDither[channel]));
		}
		else
		{
			quantized[channel] = QuantizeSample(sample);
		}
		error[1][channel] = error[0][channel];
		error[0][channel] = quantized[channel] - sample;
		if (OnlyError >= 0.5)
		{
			sample = error[0][channel];
		}
		else
		{
			sample = quantized[channel];
		}
	}
	sample = sample * OutGain;
	return sample;
}
