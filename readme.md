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

* Disable: disables the effect. This is useful if the host application doesn’t have a way of bypassing the plug-in.
* BitDepth: The new bit depth.
* DCBias: applies a DC bias to the input.
* Dither: Enable or disable dithering.
* DitherType: The type of noise that is used for dithering.
* InvertDither: Inverts the dither noise.
* DitherInError: Treats dithering as part of the quantization error.
* NoiseShaping: Enable or disable noise shaping.
* NoiseShapingFocus: Controls whether the noise shaping focuses on low or high frequencies.
* Quantize: Enable or disable the actual bit reduction.
* QuantizationMode: The type of math that is used for quantization.
* OnlyError: Only outputs the quantization error.
* AutoBlank: If this is enabled, silent audio isn't dithered or noise shaped.
* MersenneTwister: Switches to the Mersenne Twister pseudorandom number generator.
* MersenneGenerator: The Mersenne Twister generator function to use.
* Seed: The value used to reset the pseudorandom number generator.
* SeedWithTime: Resets the pseudorandom number generator with the current time. When this is enabled, the value set by Seed is ignored.
* Clip: Clips the audio before quantizing.
* ClipThreshold: Sets the threshold for Clip.
* Clip0dB: Clips audio that is over 0 dB so it can fit in a fixed point format.
* InGain: Input gain of the plug-in.
* OutGain: Output gain of the plug-in.
* DitherGain: controls the over all affect of dithering.
* NoiseShapingGain: controls the over all affect of noise shaping.

# Extra notes

* The seed parameters take effect when the plug-in is next reset.
* This is only a VST2 compatible plug-in.
* This plug-in only supports 32-bit floating point processing.
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
