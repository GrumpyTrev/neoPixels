#include "RandomLedSequence.hpp"
#include <set>

using namespace std;

namespace Lights
{
	/// @brief Select the next set of random pixels from the target sequence and save in the map
	void RandomLedSequence::Next()
	{
		// Keep track of which pixels have already been selected
		set<uint> pixelsUsed;

		for (uint index = 0; index < pixelCount; ++index)
		{
		}
	}
}