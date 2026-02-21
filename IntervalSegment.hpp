#pragma once

#include "MappedSegment.hpp"

namespace Lights
{
	/// @brief Represents a non-contiguous subset of the strip consisting of pixels selected at intervals
	/// from the strip
	class IntervalSegment : public MappedSegment
	{
	public:
		inline IntervalSegment( LedStrip* target, uint16_t start, uint16_t interval, uint16_t maxLedNumber,
			bool reverse ) : MappedSegment( target )
		{
			// How many LEDs are there
			numLeds = ( maxLedNumber - start + interval ) / interval;

			// Initialise the map
			pixelMap = new uint16_t[ numLeds ];
			uint16_t pixelIndex = start;
			uint16_t mapIndex = ( reverse == false ) ? 0 : numLeds;
			while ( pixelIndex <= maxLedNumber )
			{
				if ( reverse == false )
				{
					pixelMap[ mapIndex++ ] = pixelIndex;
				}
				else
				{
					pixelMap[ --mapIndex ] = pixelIndex;
				}
				pixelIndex += interval;
			}
		};

		inline IntervalSegment( LedStrip* target, uint16_t start, uint16_t interval )
			: IntervalSegment(target, start, interval, target->NumLeds() - 1, false) {};
	};
}