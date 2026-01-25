#include <stdlib.h>
#include <algorithm>
#include "AnimationHelper.hpp"

namespace Lights
{
	// Returns a new, random wheel index with a minimum distance of 42 from seed.
	// Stolen from  kitesurfer1404/WS2812FX
	uint8_t AnimationHelper::RandomColourWheelIndex(uint8_t seed)
	{
		uint8_t r = 0;
		uint8_t x = 0;
		uint8_t y = 0;
		uint8_t d = 0;

		while (d < 42)
		{
			r = Random8();
			x = abs(seed - r);
			y = 255 - x;
			d = std::min(x, y);
		}

		return r;
	}

	// Put a value 0 to 255 in to get a color value.
	// The colours are a transition r -> g -> b -> back to r
	// Inspired by the Adafruit examples.
	Colour AnimationHelper::ColourWheel(uint8_t wheelPos)
	{
		Colour colourValue;
		wheelPos = 255 - wheelPos;
		if (wheelPos < 85)
		{
			colourValue = Colour(255 - wheelPos * 3, 0, wheelPos * 3);
		}
		else if (wheelPos < 170)
		{
			wheelPos -= 85;
			colourValue = Colour(0, wheelPos * 3, 255 - wheelPos * 3);
		}
		else
		{
			wheelPos -= 170;
			colourValue = Colour(wheelPos * 3, 255 - wheelPos * 3, 0);
		}

		return colourValue;
	}

	// Fast 8-bit random number generator shamelessly borrowed from FastLED
	uint8_t AnimationHelper::Random8()
	{
		randomSeed16 = (randomSeed16 * 2053) + 13849;
		return (uint8_t)((randomSeed16 + (randomSeed16 >> 8)) & 0xFF);
	}

	/// Generate an 8-bit random number between 0 and lim
	/// @param lim the upper bound for the result, exclusive
	uint8_t AnimationHelper::Random8(uint8_t lim)
	{
		return (Random8() * lim) >> 8;
	}

	/// Generate an 8-bit random number in the given range
	/// @param min the lower bound for the random number, inclusive
	/// @param lim the upper bound for the random number, exclusive
	uint8_t AnimationHelper::Random8(uint8_t min, uint8_t lim)
	{
		return Random8(lim - min) + min;
	}

	/// Generate a 16 bit randowm number
	/// @return
	uint16_t AnimationHelper::Random16()
	{
		randomSeed16 = (randomSeed16 * 2053) + 13849;
		return randomSeed16;
	}

	/// Generate an 8-bit random number between 0 and lim
	/// @param lim the upper bound for the result, exclusive
	uint16_t AnimationHelper::Random16(uint16_t lim)
	{
		return (Random16() * lim) >> 16;
	}

	/// Generate an 16-bit random number in the given range
	/// @param min the lower bound for the random number, inclusive
	/// @param lim the upper bound for the random number, exclusive
	uint16_t AnimationHelper::Random16(uint16_t min, uint16_t lim)
	{
		return Random16(lim - min) + min;
	}

	void AnimationHelper::Random16AddEntropy(uint16_t entropy)
	{
		randomSeed16 += entropy;
	}

	/// Add one byte to another, saturating at 0xFF
	/// @param i first byte to add
	/// @param j second byte to add
	/// @returns the sum of i + j, capped at 0xFF
	uint8_t AnimationHelper::QAdd8(uint8_t i, uint8_t j)
	{
		uint16_t t = i + j;
		return t > 255 ? 255 : t;
	}

	/// Subtract one byte from another, saturating at 0x00
	/// @param i byte to subtract from
	/// @param j byte to subtract
	/// @returns i - j with a floor of 0
	uint8_t AnimationHelper::QSub8(uint8_t i, uint8_t j)
	{
		uint16_t t = i - j;
		return t < 0 ? 0 : t;
	}
	// Use Mark Krigsman's orignal "HeatColor" code if you want to
	// get different colors at differet "temperatures"
	Colour AnimationHelper::HeatColor(uint8_t temperature)
	{
		Colour heatcolor;
		// Scale 'heat' down from 0-255 to 0-191,
		// which can then be easily divided into three
		// equal 'thirds' of 64 units each.
		uint8_t t192 = Scale8Video(temperature, 192);
		// calculate a value that ramps up from
		// zero to 255 in each 'third' of the scale.
		uint8_t heatramp = t192 & 0x3F; // 0..63
		heatramp <<= 2;					// scale up to 0..252
		// now figure out which third of the spectrum we're in:
		if (t192 & 0x80)
		{
			// we're in the hottest third
			heatcolor.component.red = 255;		 // full red
			heatcolor.component.green = 255;	 // full green
			heatcolor.component.blue = heatramp; // ramp up blue
		}
		else if (t192 & 0x40)
		{
			// we're in the middle third
			heatcolor.component.red = 255;		  // full red
			heatcolor.component.green = heatramp; // ramp up green
			heatcolor.component.blue = 0;		  // no blue
		}
		else
		{
			// we're in the coolest third
			heatcolor.component.red = heatramp; // ramp up red
			heatcolor.component.green = 0;		// no green
			heatcolor.component.blue = 0;		// no blue
		}
		return heatcolor;
	}

	/// The "video" version of scale8() guarantees that the output will
	/// be only be zero if one or both of the inputs are zero.
	/// If both inputs are non-zero, the output is guaranteed to be non-zero.
	/// This makes for better "video"/LED dimming, at the cost of
	/// several additional cycles.
	/// @param i input value to scale
	/// @param scale scale factor, in n/256 units
	/// @returns scaled value
	/// @see scale8()
	uint8_t AnimationHelper::Scale8Video(uint8_t i, uint8_t scale)
	{
		return (((int)i * (int)scale) >> 8) + ((i && scale) ? 1 : 0);
	}

	uint16_t AnimationHelper::randomSeed16 = 0;
}