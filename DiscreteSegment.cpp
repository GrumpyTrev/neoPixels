#include "DiscreteSegment.hpp"

namespace Lights
{
	DiscreteSegment::DiscreteSegment( LedStrip* target, vector<uint16_t> pixels ) : MappedSegment( target )
	{
		numLeds = pixels.size();

		// Initialise the map
		pixelMap = new uint16_t[ numLeds ];

		uint16_t mapIndex = 0;
		for (auto &pixel : pixels)
		{
			pixelMap[mapIndex++] = pixel;
		}
	}
}