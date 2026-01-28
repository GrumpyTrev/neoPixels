#include "IntervalSegment.hpp"

namespace Lights
{
	IntervalSegment::IntervalSegment( LedStrip* target, uint16_t start, uint16_t interval,
		uint16_t maxLedNumber, bool reverse ) : MappedSegment( target )
	{
		// Make sure that the start index and maxLedNumber are valid
		if ((start < target->NumLeds()) && (maxLedNumber >= start) && (maxLedNumber < target->NumLeds()))
		{
			// How many LEDs are there
			numLeds = (maxLedNumber - start + interval) / interval;

			// Initialise the map
			pixelMap = new uint16_t[ numLeds ];
			uint16_t pixelIndex = start;
			uint16_t mapIndex = ( reverse == false ) ? 0 : numLeds;
			while (pixelIndex <= maxLedNumber)
			{
				if (reverse == false)
				{
					pixelMap[mapIndex++] = pixelIndex;
				}
				else
				{
					pixelMap[--mapIndex] = pixelIndex;
				}
				pixelIndex += interval;
			}
		}
	}
}