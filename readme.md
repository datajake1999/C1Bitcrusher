# What is this?
This is a bitcrusher plug-in with an emphasis on realism. It processes audio in the same way as creating an audio file with a lower bit depth. To prove this, you can do the following test in Audacity.

* Open preferences and disable dithering.
* Record or generate some audio.
* Apply the effect with the default settings.
* Export a 16 bit WAV file.
* Invert the track.
* Import your previously exported audio file.
* Mix and render the project. You should notice the audio is pure silence.

# Parameters and their descriptions

* BitDepth: The new bit depth.
* Dither: Enable or disable dithering.
* DitherType: The type of noise that is used for dithering.
* NoiseShaping: Enable or disable noise shaping.
* Quantize: Enable or disable the actual bit reduction.
* QuantizationMode: The type of math that is used for quantization.
* OnlyError: Only outputs the quantization noise.
* AutoDither: If this is enabled, silent audio isn't dithered.
* ClipPreQuantization: Clips the audio before quantizing.
* ClipValue: The maximum sample value allowed in the audio before quantizing.
* InGain: Input gain of the plug-in.
* OutGain: Output gain of the plug-in.
* DitherGain: controls the over all affect of dithering.
* NoiseShapingGain: controls the over all affect of noise shaping.

# License
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
