#pragma once
#include "SimpleSequenceProvider.hpp"
#include "SegmentProvider.hpp"

namespace Lights
{
	class SegmentSequenceProvider : public SegmentProvider
	{
	public:
		inline SegmentSequenceProvider() { providerImplementation = new SimpleSequenceProvider<Segment *>(nullptr); }
	};
}