#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "mt19937ar.h"
#include "C1.h"

static const double psycho3[3] =
{
	1.623,
	-0.982,
	0.109
};

static const double psycho5[5] =
{
	2.033,
	-2.165,
	1.959,
	-1.590,
	0.6149
};

static const double psycho9[9] =
{
	2.412,
	-3.370,
	3.937,
	-4.174,
	3.353,
	-2.205,
	1.281,
	-0.569,
	0.0847
};

void C1Init(C1State *state)
{
	C1Settings settings;
	if (!state)
	{
		return;
	}
	memset(state, 0, sizeof(C1State));
	memset(&settings, 0, sizeof(settings));
	settings.Disable = 0;
	settings.InGain = 1;
	settings.OutGain = 1;
	settings.BitDepth = 16;
	settings.DCBias = 0;
	settings.Dither = 1;
	settings.DitherType = kTriangular;
	settings.InvertDither = 0;
	settings.HighpassDither = 0;
	settings.HighpassGain = 1;
	settings.DitherGain = 1;
	settings.MersenneTwister = 1;
	settings.MersenneGenerator = kGenerator3;
	settings.Seed = 0;
	settings.SeedWithTime = 0;
	settings.NoiseShaping = 0;
	settings.NoiseShapingFilter = kSecondOrder;
	settings.PsychoacousticCurve = kLipshitz;
	settings.NoiseShapingGain = 1;
	settings.AutoBlank = 0;
	settings.Clip = 0;
	settings.ClipThreshold = 1;
	settings.Quantize = 1;
	settings.QuantizationMode = kRound;
	settings.Clip0dB = 1;
	settings.DitherInError = 1;
	settings.OnlyError = 0;
	C1LoadSettings(state, &settings);
	C1ResetPRNG(state);
}

void C1LoadSettings(C1State *state, C1Settings *settings)
{
	if (!state || !settings)
	{
		return;
	}
	state->settings.Disable = settings->Disable;
	state->settings.InGain = settings->InGain;
	state->settings.OutGain = settings->OutGain;
	state->settings.BitDepth = settings->BitDepth;
	if (state->settings.BitDepth > 32)
	{
		state->settings.BitDepth = 32;
	}
	else if (state->settings.BitDepth < 1)
	{
		state->settings.BitDepth = 1;
	}
	state->scale = pow(2.0, state->settings.BitDepth) / 2;
	state->settings.DCBias = settings->DCBias;
	if (state->settings.DCBias > 2)
	{
		state->settings.DCBias = 2;
	}
	else if (state->settings.DCBias < -2)
	{
		state->settings.DCBias = -2;
	}
	state->settings.Dither = settings->Dither;
	state->settings.DitherType = settings->DitherType%kNumDitherTypes;
	state->settings.InvertDither = settings->InvertDither;
	state->settings.HighpassDither = settings->HighpassDither;
	state->settings.HighpassGain = settings->HighpassGain;
	state->settings.DitherGain = settings->DitherGain;
	state->settings.MersenneTwister = settings->MersenneTwister;
	state->settings.MersenneGenerator = settings->MersenneGenerator%kNumMersenneGenerators;
	state->settings.Seed = settings->Seed;
	if (state->settings.Seed < 0)
	{
		state->settings.Seed = 0;
	}
	state->settings.SeedWithTime = settings->SeedWithTime;
	state->settings.NoiseShaping = settings->NoiseShaping;
	state->settings.NoiseShapingFilter = settings->NoiseShapingFilter%kNumNoiseShapingFilters;
	state->settings.PsychoacousticCurve = settings->PsychoacousticCurve%kNumPsychoacousticCurves;
	switch(state->settings.PsychoacousticCurve)
	{
	case kWannamaker3Tap:
		state->num_coeffs = 3;
		memcpy(state->coeffs, psycho3, sizeof(psycho3));
		break;
	case kLipshitz:
		state->num_coeffs = 5;
		memcpy(state->coeffs, psycho5, sizeof(psycho5));
		break;
	case kWannamaker9Tap:
		state->num_coeffs = 9;
		memcpy(state->coeffs, psycho9, sizeof(psycho9));
		break;
	}
	state->settings.NoiseShapingGain = settings->NoiseShapingGain;
	state->settings.AutoBlank = settings->AutoBlank;
	state->settings.Clip = settings->Clip;
	state->settings.ClipThreshold = settings->ClipThreshold;
	if (state->settings.ClipThreshold < 0)
	{
		state->settings.ClipThreshold = 0;
	}
	state->settings.Quantize = settings->Quantize;
	state->settings.QuantizationMode = settings->QuantizationMode%kNumQuantizationModes;
	state->settings.Clip0dB = settings->Clip0dB;
	state->settings.DitherInError = settings->DitherInError;
	state->settings.OnlyError = settings->OnlyError;
}

void C1GetSettings(C1State *state, C1Settings *settings)
{
	if (!state || !settings)
	{
		return;
	}
	memcpy(settings, &state->settings, sizeof(C1Settings));
}

void C1ResetPRNG(C1State *state)
{
	if (!state)
	{
		return;
	}
	if (state->settings.SeedWithTime )
	{
		srand(time(NULL));
		init_genrand(time(NULL));
	}
	else
	{
		srand(state->settings.Seed);
		init_genrand(state->settings.Seed);
	}
}

void C1ResetChannel(C1ChannelState *cs)
{
	if (!cs)
	{
		return;
	}
	memset(cs, 0, sizeof(C1ChannelState));
}

static double PRNG(C1State *state)
{
	if (!state)
	{
		return 0;
	}
	if (state->settings.MersenneTwister)
	{
		switch(state->settings.MersenneGenerator)
		{
		case kGenerator1:
			return genrand_real1();
		case kGenerator2:
			return genrand_real2();
		case kGenerator3:
			return genrand_real3();
		}
	}
	else
	{
		return rand() / (double)RAND_MAX;
	}
	return 0;
}

static double RPDF(C1State *state)
{
	double s;
	if (!state)
	{
		return 0;
	}
	s = PRNG(state);
	s = (s - 0.5) * 2;
	return s;
}

static double TPDF(C1State *state)
{
	double s1;
	double s2;
	double out;
	if (!state)
	{
		return 0;
	}
	s1 = PRNG(state);
	s2 = PRNG(state);
	s1 = (s1 - 0.5) * 2;
	s2 = (s2 - 0.5) * 2;
	out = s1 + s2;
	return out;
}

#define PI 3.1415926536

static double AWGN_generator(C1State *state)
{
	double temp1;
	double temp2;
	double result;
	int p;
	if (!state)
	{
		return 0;
	}
	p = 1;
	while (p > 0)
	{
		temp2 = PRNG(state);
		if (temp2 == 0)
		{
			p = 1;
		}
		else
		{
			p = -1;
		}
	}
	temp1 = cos((2.0 * (double)PI) * PRNG(state));
	result = sqrt(-2.0 * log(temp2)) * temp1;
	return result;
}

static double DitherSample(C1State *state, C1ChannelState *cs, double sample)
{
	double noise;
	if (!state)
	{
		return sample;
	}
	if (sample == 0 && state->settings.AutoBlank)
	{
		return 0;
	}
	switch(state->settings.DitherType)
	{
	case kRectangular:
		noise = RPDF(state);
		break;
	case kTriangular:
		noise = TPDF(state);
		break;
	case kGaussian:
		noise = AWGN_generator(state);
		break;
	}
	noise = noise / state->scale;
	if (state->settings.InvertDither)
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
	if (state->settings.HighpassDither)
	{
		double noise2 = 0;
		if (cs)
		{
			noise2 = cs->LastDither;
			cs->LastDither = noise;
		}
		noise = noise - (noise2 * state->settings.HighpassGain);
	}
	sample = sample + (noise * state->settings.DitherGain);
	return sample;
}

static double NoiseShapeSampleFirstOrder(C1State *state, C1ChannelState *cs, double sample)
{
	if (!state || !cs)
	{
		return sample;
	}
	if (sample == 0 && state->settings.AutoBlank)
	{
		return 0;
	}
	sample = sample - (cs->error[0] * state->settings.NoiseShapingGain);
	return sample;
}

static double NoiseShapeSampleSecondOrder(C1State *state, C1ChannelState *cs, double sample)
{
	if (!state || !cs)
	{
		return sample;
	}
	if (sample == 0 && state->settings.AutoBlank)
	{
		return 0;
	}
	sample = sample - (((cs->error[0] * 2) - cs->error[1]) * state->settings.NoiseShapingGain);
	return sample;
}

static double NoiseShapeSamplePsycho(C1State *state, C1ChannelState *cs, double sample)
{
	int i;
	if (!state || !cs)
	{
		return sample;
	}
	if (sample == 0 && state->settings.AutoBlank)
	{
		return 0;
	}
	for (i = 0; i < state->num_coeffs; i++)
	{
		sample = sample - (cs->error[i] * (state->coeffs[i] * state->settings.NoiseShapingGain));
	}
	return sample;
}

static double DCSample(C1State *state, double sample)
{
	double DC;
	if (!state)
	{
		return sample;
	}
	DC = state->settings.DCBias / state->scale;
	if (DC > 1)
	{
		DC = 1;
	}
	else if (DC < -1)
	{
		DC = -1;
	}
	sample = sample + DC;
	return sample;
}

static double ClipSample(C1State *state, double sample)
{
	if (!state)
	{
		return sample;
	}
	if (sample > state->settings.ClipThreshold)
	{
		sample = state->settings.ClipThreshold;
	}
	else if (sample < state->settings.ClipThreshold * -1)
	{
		sample = state->settings.ClipThreshold * -1;
	}
	return sample;
}

static double QuantizeSample(C1State *state, double sample)
{
	if (!state)
	{
		return sample;
	}
	sample = sample * state->scale;
	switch(state->settings.QuantizationMode)
	{
	case kFloor:
		sample = floor(sample);
		break;
	case kCeiling:
		sample = ceil(sample);
		break;
	case kTruncate:
		if (sample >= 0)
		{
			sample = floor(sample);
		}
		else
		{
			sample = ceil(sample);
		}
		break;
	case kRound:
		if (sample >= 0)
		{
			sample = floor(sample + 0.5);
		}
		else
		{
			sample = ceil(sample - 0.5);
		}
		break;
	}
	if (state->settings.Clip0dB)
	{
		if (sample > state->scale-1)
		{
			sample = state->scale-1;
		}
		else if (sample < state->scale * -1)
		{
			sample = state->scale * -1;
		}
	}
	sample = sample / state->scale;
	return sample;
}

double C1ProcessSample(C1State *state, C1ChannelState *cs, double sample)
{
	int i;
	if (!state || state->settings.Disable)
	{
		return sample;
	}
	sample = sample * state->settings.InGain;
	sample = DCSample(state, sample);
	if (state->settings.Dither && !state->settings.DitherInError)
	{
		sample = DitherSample(state, cs, sample);
	}
	if (state->settings.Clip)
	{
		sample = ClipSample(state, sample);
	}
	if (state->settings.Quantize)
	{
		double quantized;
		double error;
		if (state->settings.NoiseShaping)
		{
			switch(state->settings.NoiseShapingFilter)
			{
			case kFirstOrder:
				sample = NoiseShapeSampleFirstOrder(state, cs, sample);
				break;
			case kSecondOrder:
				sample = NoiseShapeSampleSecondOrder(state, cs, sample);
				break;
			case kPsychoacoustic:
				sample = NoiseShapeSamplePsycho(state, cs, sample);
				break;
			}
		}
		if (state->settings.Dither && state->settings.DitherInError)
		{
			quantized = QuantizeSample(state, DitherSample(state, cs, sample));
		}
		else
		{
			quantized = QuantizeSample(state, sample);
		}
		error = quantized - sample;
		if (state->settings.NoiseShaping)
		{
			switch(state->settings.NoiseShapingFilter)
			{
			case kSecondOrder:
				cs->error[1] = cs->error[0];
				break;
			case kPsychoacoustic:
				for (i = state->num_coeffs-1; i >= 1; i--)
				{
					cs->error[i] = cs->error[i-1];
				}
				break;
			}
			cs->error[0] = error;
		}
		if (state->settings.OnlyError)
		{
			sample = error;
		}
		else
		{
			sample = quantized;
		}
	}
	sample = sample * state->settings.OutGain;
	return sample;
}
