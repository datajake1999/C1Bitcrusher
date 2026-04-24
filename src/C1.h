#ifndef C1_H
#define C1_H

#include "rnd.h"

enum
{
	kRectangular = 0,
	kTriangular,
	kGaussian,
	kNumDitherTypes
};

enum
{
	kPCG = 0,
	kWELL,
	kGameRand,
	kXorShift,
	kNumRandomGenerators
};

enum
{
	kFirstOrder = 0,
	kSecondOrder,
	kPsychoacoustic,
	kNumNoiseShapingFilters
};

enum
{
	kWannamaker3Tap = 0,
	kLipshitz,
	kWannamaker9Tap,
	kNumPsychoacousticCurves
};

enum
{
	kFloor = 0,
	kCeiling,
	kTruncate,
	kRound,
	kNumQuantizationModes
};

typedef struct
{
	int Disable;
	double InGain;
	double OutGain;
	double BitDepth;
	double DCBias;
	int Dither;
	int DitherType;
	int InvertDither;
	int HighpassDither;
	double HighpassGain;
	double DitherGain;
	int RandomGenerator;
	int Seed;
	int SeedWithTime;
	int NoiseShaping;
	int NoiseShapingFilter;
	int PsychoacousticCurve;
	double NoiseShapingGain;
	int AutoBlank;
	int Clip;
	double ClipThreshold;
	int Quantize;
	int QuantizationMode;
	int Clip0dB;
	int DitherInError;
	int OnlyError;
	int NormalizeError;
}C1Settings;

typedef struct
{
	C1Settings settings;
	double scale;
	double error_norm_gain;
	double dc;
	double coeffs[9];
	int num_coeffs;
	rnd_pcg_t pcg_state;
	rnd_well_t well_state;
	rnd_gamerand_t gamerand_state;
	rnd_xorshift_t xorshift_state;
}C1State;

typedef struct
{
	double LastDither;
	double error[9];
}C1ChannelState;

#ifdef __cplusplus
extern "C" {
#endif

void C1Init(C1State *state, C1Settings *defaults);
void C1LoadSettings(C1State *state, C1Settings *settings);
void C1GetSettings(C1State *state, C1Settings *settings);
void C1ResetPRNG(C1State *state);
void C1ResetChannel(C1ChannelState *cs);
double C1ProcessSample(C1State *state, C1ChannelState *cs, double sample);

#ifdef __cplusplus
}
#endif

#endif
