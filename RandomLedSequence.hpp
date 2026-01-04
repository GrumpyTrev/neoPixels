#pragma once
#include "MappedLedSequence.hpp"
#include "SimpleProvider.hpp"

namespace Lights
{
	class RandomLedSequence : public MappedSegment, SimpleProvider<uint>
	{
	public:
		inline RandomLedSequence(LedStrip *target, Segment *baseSequence, uint count)
			: MappedSegment(target), SimpleProvider(0), base(baseSequence), pixelCount(count)
		{
			pixelMap = new uint[pixelCount];
		};

	private:
		Segment *base;
		uint pixelCount;
	};
}