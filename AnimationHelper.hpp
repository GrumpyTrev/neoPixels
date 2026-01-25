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
		static uint16_t Random16(uint16_t min, uint16_t lim);
		static void Random16AddEntropy(uint16_t entropy);
		static uint8_t QAdd8(uint8_t i, uint8_t j);
		static uint8_t QSub8(uint8_t i, uint8_t j);
		static Colour HeatColor(uint8_t temperature);
		static uint8_t Scale8Video(uint8_t i, uint8_t scale);
		static inline void ResetSeed(uint16_t seed) { randomSeed16 = seed; };

	private:
		// 16 bit random number seed
		static uint16_t randomSeed16;
	};
}