#include "ws2812Strip.hpp"
#include "ws2812.pio.h"
#include <pico/time.h>

namespace Lights
{
	/// @brief Constructor with PIO block and state machine number specified in call.
	///	Initialise the PIO
	/// @param pioBlock
	/// @param stateMachine
	/// @param dataPin
	/// @param numLeds
	ws2812Strip::ws2812Strip(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds)
		: LedStrip(numLeds), pioBlock(pioBlock), stateMachine(stateMachine), dataPin(dataPin)
	{
		InitialiseStrip();
	}

	/// @brief Constructor with no PIO block or state machine number specified in call.
	///	Find an available PIO/state machine pair and initialise the PIO
	/// @param dataPin
	/// @param numLeds
	ws2812Strip::ws2812Strip(uint dataPin, uint numLeds) : LedStrip(numLeds), dataPin(dataPin)
	{
		// Try getting an unused SM from first PIO 0 and then PIO 1
		pioBlock = pio0;
		int unusedStateMachine = pio_claim_unused_sm(pioBlock, false);
		if (unusedStateMachine == -1)
		{
			pioBlock = pio1;
			unusedStateMachine = pio_claim_unused_sm(pioBlock, true);
		}

		stateMachine = unusedStateMachine;
		InitialiseStrip();
	}

	/// @brief Add the WS2812B PIO program to the specified PIO/sm and set it going
	void ws2812Strip::InitialiseStrip()
	{
		uint offset = pio_add_program(pioBlock, &ws2812_program);

		ws2812_program_init(pioBlock, stateMachine, offset, dataPin, Frequency, BitsPerPixel);

		// Always allow the first write
		nextWriteAllowedTime = get_absolute_time();
	}

	/// @brief Write out the colour values to all the LEDs
	/// Scale the individual LED values using the current brightness value
	void ws2812Strip::Show()
	{
		// Has enough time elapsed since the last write
		int64_t timeUntilWriteAllowed = absolute_time_diff_us(get_absolute_time(), nextWriteAllowedTime);
		if (timeUntilWriteAllowed > 0)
		{
			// Not enough time has elapsed yet. Sleep until nextWriteAllowedTime
			sleep_us(timeUntilWriteAllowed);
		}

		// Can write now.
		for (uint i = 0; i < numLeds; i++)
		{
			pio_sm_put_blocking(pioBlock, stateMachine,
								ScalePixelData(pixelData[i].value, brightness) << 8);
		}

		// Update the next time that a write is allowed
		nextWriteAllowedTime = delayed_by_us(get_absolute_time(), LedLatchTimeUs);
	}
}