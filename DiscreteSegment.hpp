#pragma once

#include "MappedSegment.hpp"

using namespace std;

namespace Lights
{
	/// @brief Represents a set of discretely numbered pixels
	class DiscreteSegment : public MappedSegment
	{
	public:
		inline DiscreteSegment( LedStrip* target, vector<uint16_t> const& pixels ) : MappedSegment( target )
		{
			numLeds = pixels.size();

			// Initialise the map
			pixelMap = new uint16_t[ numLeds ];
			copy( pixels.begin(), pixels.end(), pixelMap );
		}

	private:
	};
}