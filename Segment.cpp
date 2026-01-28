#include "Segment.hpp"

namespace Lights
{
	void Segment::Fill( Colour colour, uint16_t first, uint16_t count )
	{
		uint16_t last = ( first + count );
		if (last > numLeds)
		{
			last = numLeds;
		}
		for ( uint16_t i = first; i < last; i++ )
		{
			SetPixelColour(i, colour);
		}
	}

	void Segment::Shift()
	{
		for ( uint16_t index = numLeds - 1; index > 0; index-- )
		{
			SetPixelColour(index, PixelColour(index - 1));
		}
	}
}