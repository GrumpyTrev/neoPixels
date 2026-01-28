#include "AnimationHelper.hpp"

namespace Lights
{
	// Returns a new, random wheel index with a minimum distance of 42 from seed.
	// Stolen from  kitesurfer1404/WS2812FX
	uint8_t AnimationHelper::RandomColourWheelIndex( uint8_t seed )
	{
		uint8_t r = 0;
		uint8_t x = 0;
		uint8_t y = 0;
		uint8_t d = 0;

		while ( d < 42 )
		{
			r = Random8();
			x = abs( seed - r );
			y = 255 - x;
			d = std::min( x, y );
		}

		return r;
	}

	// Put a value 0 to 255 in to get a color value.
	// The colours are a transition r -> g -> b -> back to r
	// Inspired by the Adafruit examples.
	Colour AnimationHelper::ColourWheel( uint8_t wheelPos )
	{
		Colour colourValue;
		wheelPos = 255 - wheelPos;
		if ( wheelPos < 85 )
		{
			colourValue = Colour( 255 - wheelPos * 3, 0, wheelPos * 3 );
		}
		else if ( wheelPos < 170 )
		{
			wheelPos -= 85;
			colourValue = Colour( 0, wheelPos * 3, 255 - wheelPos * 3 );
		}
		else
		{
			wheelPos -= 170;
			colourValue = Colour( wheelPos * 3, 255 - wheelPos * 3, 0 );
		}

		return colourValue;
	}

	// Fast 8-bit random number generator shamelessly borrowed from FastLED
	uint8_t AnimationHelper::Random8()
	{
		randomSeed16 = ( randomSeed16 * 2053 ) + 13849;
		return (uint8_t)( ( randomSeed16 + ( randomSeed16 >> 8 ) ) & 0xFF );
	}

	/// Generate an 8-bit random number between 0 and lim
	/// @param lim the upper bound for the result, exclusive
	uint8_t AnimationHelper::Random8( uint8_t lim )
	{
		return ( Random8() * lim ) >> 8;
	}

	/// Generate an 8-bit random number in the given range
	/// @param min the lower bound for the random number, inclusive
	/// @param lim the upper bound for the random number, exclusive
	uint8_t AnimationHelper::Random8( uint8_t min, uint8_t lim )
	{
		return Random8( lim - min ) + min;
	}

	/// Generate a 16 bit randowm number
	/// @return
	uint16_t AnimationHelper::Random16()
	{
		randomSeed16 = ( randomSeed16 * 2053 ) + 13849;
		return randomSeed16;
	}

	/// Generate an 8-bit random number between 0 and lim
	/// @param lim the upper bound for the result, exclusive
	uint16_t AnimationHelper::Random16( uint16_t lim )
	{
		return ( Random16() * lim ) >> 16;
	}

	/// Generate an 16-bit random number in the given range
	/// @param min the lower bound for the random number, inclusive
	/// @param lim the upper bound for the random number, exclusive
	uint16_t AnimationHelper::Random16( uint16_t min, uint16_t lim )
	{
		return Random16( lim - min ) + min;
	}

	uint16_t AnimationHelper::randomSeed16 = 0;
}