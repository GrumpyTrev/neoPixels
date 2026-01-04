#include "Segment.hpp"

namespace Lights
{
	void Segment::Fill(Colour colour, uint first, uint count)
	{
		uint last = (first + count);
		if (last > numLeds)
		{
			last = numLeds;
		}
		for (uint i = first; i < last; i++)
		{
			SetPixelColour(i, colour);
		}
	}

	void Segment::Shift()
	{
		for (uint index = numLeds - 1; index > 0; index--)
		{
			SetPixelColour(index, PixelColour(index - 1));
		}
	}
}