#include "IntervalSegment.hpp"

namespace Lights
{
	IntervalSegment::IntervalSegment(LedStrip *target, uint start, uint interval,
									 uint maxLedNumber, bool reverse) : MappedSegment(target)
	{
		// Make sure that the start index and maxLedNumber are valid
		if ((start < target->NumLeds()) && (maxLedNumber >= start) && (maxLedNumber < target->NumLeds()))
		{
			// How many LEDs are there
			numLeds = (maxLedNumber - start + interval) / interval;

			// Initialise the map
			pixelMap = new uint[numLeds];
			uint pixelIndex = start;
			uint mapIndex = (reverse == false) ? 0 : numLeds;
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