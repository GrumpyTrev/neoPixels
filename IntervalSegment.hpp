#pragma once

#include "MappedSegment.hpp"

namespace Lights
{
	/// @brief Represents a non-contiguous subset of the strip consisting of pixels selected at intervals
	/// from the strip
	class IntervalSegment : public MappedSegment
	{
	public:
		IntervalSegment( LedStrip* target, uint16_t start, uint16_t interval, uint16_t maxLedNumber, bool reverse );
		inline IntervalSegment( LedStrip* target, uint16_t start, uint16_t interval )
			: IntervalSegment(target, start, interval, target->NumLeds() - 1, false) {};
	};
}