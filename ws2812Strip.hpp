#pragma once
#include "hardware/pio.h"
#include "LedStrip.hpp"

namespace Lights
{
	class ws2812Strip : public LedStrip
	{
	public:
		// Fixed frequency for Leds being used
		const float Frequency = 800000;

		// Leds are RGB i.e. 24 bits
		const uint BitsPerPixel = 24;

		ws2812Strip(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds);
		ws2812Strip(uint dataPin, uint numLeds);
		void Show();

	private:
		void InitialiseStrip();
		PIO pioBlock;
		uint stateMachine;
		uint dataPin;
		absolute_time_t nextWriteAllowedTime;

		/// @brief The number of microseconds to allow the last set of data to be latched by the
		/// LEDs before sending some more
		const uint LedLatchTimeUs = 600;
	};
}