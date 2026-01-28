#pragma once
#include "pico/types.h"
#include "Colour.hpp"

namespace Lights
{
	// Collection of miscellaneous functions used by the animation classes
	class AnimationHelper
	{
	public:
		static uint8_t RandomColourWheelIndex(uint8_t seed);
		static Colour ColourWheel(uint8_t pos);
		static uint8_t Random8();
		static uint8_t Random8(uint8_t lim);
		static uint8_t Random8(uint8_t min, uint8_t lim);
		static uint16_t Random16();
		static uint16_t Random16(uint16_t lim);
		static uint16_t Random16( uint16_t min, uint16_t lim );

	private:
		// 16 bit random number seed
		static uint16_t randomSeed16;
	};
}