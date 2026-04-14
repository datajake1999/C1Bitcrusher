# Introduction
This is a bitcrusher VST plug-in with an emphasis on accuracy. It processes audio in a similar way as creating an audio file with a lower bit depth.

# Parameters and Their Descriptions

* Disable: Bypasses the effect. This is useful if the host application doesn't have a way of bypassing the plug-in.
* InGain: Input gain of the effect.
* OutGain: Output gain of the effect.
* BitDepth: The target bit depth.
* DCBias: Applies DC offset to the input.
* Dither: Enables or disables dithering.
* DitherType: The probability density function that is used to generate dither noise. The available options are Rectangular, Triangular, and Gaussian.
* InvertDither: Flips the polarity of the dither noise.
* HighpassDither: Applies a highpass filter to the dither noise.
* HighpassGain: controls the over all effect of the aforementioned highpass filter.
* DitherGain: controls the over all effect of dithering.
* MersenneTwister: Switches to using the Mersenne Twister pseudorandom number generator instead of the PRNG provided by the standard library.
* MersenneGenerator: The Mersenne Twister generator function to use.
* Seed: The value that is used to reset the pseudorandom number generators.
* SeedWithTime: Resets the pseudorandom number generators with the current time. When this is enabled, the value set by Seed is ignored.
* NoiseShaping: Enables or disables noise shaping.
* NoiseShapingFilter: The noise shaping filter that is used. The available options are First Order, Second Order, and Psychoacoustic.
* PsychoacousticCurve: The EQ curve that is used in the psychoacoustic noise shaping filter. The available options are Wannamaker 3-tap, Lipshitz, and Wannamaker 9-tap.
* NoiseShapingGain: controls the over all effect of noise shaping.
* AutoBlank: If this is enabled, silence isn't dithered or noise shaped.
* Clip: Clips the audio before quantizing.
* ClipThreshold: Sets the threshold for Clip.
* Quantize: Enables or disables bit reduction.
* QuantizationMode: The type of math that is used for quantization. The available options are Floor, Ceiling, Truncate, and Round.
* Clip0dB: Clips audio that is over 0 dB after the quantization process. If this is enabled, the quantized output can fit perfectly in a fixed point format if the output gain isn't changed.
* DitherInError: Treats dithering as part of the quantization process. This makes combined dithering and noise shaping more effective.
* OnlyError: Sends the quantization error to the output instead of the quantized signal.
* NormalizeError: If OnlyError is enabled, this option normalizes quantization error to -30 dBFS.

# Extra Notes

* The seed parameters take effect when the plug-in is next reset.
* This is only a VST2 compatible plug-in.
* This plug-in has no user interface of Its own, so the host application should provide a method of interacting with plug-in parameters.
* This project is developed directly with the VST 2.4 SDK. As a result, this plug-in has a small footprint.

# License
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
