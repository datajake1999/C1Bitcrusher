# What is this?
This is a bitcrusher VST plug-in with an emphasis on realism. It processes audio in the same way as creating an audio file with a lower bit depth. To prove this, you can do the following test in Audacity.

* Install the plug-in.
* Open Audacity.
* Enable the effect.
* Open preferences and disable dithering.
* Record or generate some audio.
* Apply the effect with the default settings.
* Export a 16 bit WAV file.
* Invert the track.
* Import your previously exported audio file.
* Mix and render the project. You should notice the audio is pure silence.

# Parameters and their descriptions

* Disable: Bypasses the effect. This is useful if the host application doesn’t have a way of bypassing the plug-in.
* InGain: Input gain of the effect.
* OutGain: Output gain of the effect.
* BitDepth: The target bit depth.
* DCBias: applies a DC bias to the input.
* Dither: Enable or disable dithering.
* DitherType: The probability density function that is used to generate dither noise.
* InvertDither: Flips the polarity of the dither noise.
* HighpassDither: Applies a highpass filter to the dither noise.
* DitherGain: controls the over all affect of dithering.
* MersenneTwister: Switches to using the Mersenne Twister pseudorandom number generator instead of the PRNG provided by the standard library.
* MersenneGenerator: The Mersenne Twister generator function to use.
* Seed: The value used to reset the pseudorandom number generators.
* SeedWithTime: Resets the pseudorandom number generators with the current time. When this is enabled, the value set by Seed is ignored.
* NoiseShaping: Enable or disable noise shaping.
* NoiseShapingFocus: Controls whether the noise shaping focuses on low or high frequencies.
* NoiseShapingOrder: The noise shaping filter style.
* NoiseShapingGain: controls the over all affect of noise shaping.
* AutoBlank: If this is enabled, silent audio isn't dithered or noise shaped.
* Clip: Clips the audio before quantizing.
* ClipThreshold: Sets the threshold for Clip.
* Quantize: Enable or disable the actual bit reduction.
* QuantizationMode: The type of math that is used for quantization.
* Clip0dB: Clips audio that is over 0 dB as part of the quantization process, so the quantized audio can fit in a fixed point format.
* DitherInError: Treats dithering as part of the quantization process.
* OnlyError: Sends the quantization error to the output instead of the quantized signal.

# Extra notes

* The seed parameters take effect when the plug-in is next reset.
* This is only a VST2 compatible plug-in.
* This plug-in has no User Interface of Its own, so the host application should provide a method of interacting with effect parameters.
* This project is developed directly with the VST SDK. As a result, this plug-in has a small footprint.

# License
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
