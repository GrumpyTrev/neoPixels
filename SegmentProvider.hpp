#pragma once
#include "SimpleProvider.hpp"
#include "Segment.hpp"

namespace Lights
{
	/// @brief Class used to wrap up a Segment in a provider
	class SegmentProvider : public BaseProvider<Segment *>
	{
	public:
		inline SegmentProvider(Segment *segment)
		{
			providerImplementation = new SimpleProvider<Segment *>(segment);
		};

	protected:
		inline SegmentProvider() {};
	};
}