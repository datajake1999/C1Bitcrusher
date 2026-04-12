#ifndef C1_H
#define C1_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	kRectangular = 0,
	kTriangular,
	kGaussian,
	kNumDitherTypes
};

enum
{
	kGenerator1 = 0,
	kGenerator2,
	kGenerator3,
	kNumMersenneGenerators
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
	int MersenneTwister;
	int MersenneGenerator;
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
}C1Settings;

typedef struct
{
	C1Settings settings;
	double scale;
	double coeffs[9];
	int num_coeffs;
}C1State;

typedef struct
{
	double LastDither;
	double error[9];
}C1ChannelState;

void C1Init(C1State *state);
void C1LoadSettings(C1State *state, C1Settings *settings);
void C1GetSettings(C1State *state, C1Settings *settings);
void C1ResetPRNG(C1State *state);
void C1ResetChannel(C1ChannelState *cs);
double C1ProcessSample(C1State *state, C1ChannelState *cs, double sample);

#ifdef __cplusplus
}
#endif

#endif
