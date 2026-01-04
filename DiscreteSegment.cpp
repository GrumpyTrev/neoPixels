#include "DiscreteSegment.hpp"

namespace Lights
{
	DiscreteSegment::DiscreteSegment(LedStrip *target, vector<uint> pixels) : MappedSegment(target)
	{
		numLeds = pixels.size();

		// Initialise the map
		pixelMap = new uint[numLeds];

		uint mapIndex = 0;
		for (auto &pixel : pixels)
		{
			pixelMap[mapIndex++] = pixel;
		}
	}
}