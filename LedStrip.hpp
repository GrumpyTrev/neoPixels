#pragma once

#include <pico/types.h>
#include "Colour.hpp"

namespace Lights
{
	/// @brief The LedStrip class represents a string of LEDs together with methods to
	/// set the colour of each individual and to show the entire string
	class LedStrip
	{
	public:
		LedStrip(uint numLeds);
		~LedStrip();

		inline uint NumLeds() { return numLeds; };
		inline uint8_t Brightness() { return brightness; };
		inline void SetBrightness(uint8_t brightnessValue) { brightness = brightnessValue; };
		Colour PixelColour(uint index);
		void SetPixelColour(uint index, Colour colour);
		virtual void Show() = 0;
		inline void ApplyGamma(bool gammaValue) { applyGamma = gammaValue; };
		inline bool Gamma() { return applyGamma; };

	protected:
		uint32_t ScalePixelData(uint32_t data, uint16_t scale);

		uint numLeds;
		uint8_t brightness = 255;
		bool applyGamma;

		// The Colour data for the entire string of LEDs
		Colour *pixelData;
	};
};
