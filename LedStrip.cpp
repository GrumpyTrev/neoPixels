#include "LedStrip.hpp"

namespace Lights
{
	LedStrip::LedStrip(uint numLeds) : numLeds(numLeds)
	{
		pixelData = new Colour[numLeds];
	}

	LedStrip::~LedStrip()
	{
		delete pixelData;
	}

	Colour LedStrip::PixelColour(uint index)
	{
		Colour pixelColor;
		if (index < numLeds)
		{
			pixelColor = pixelData[index];
		}

		return pixelColor;
	}

	void LedStrip::SetPixelColour(uint index, Colour colour)
	{
		if (index < numLeds)
		{
			pixelData[index] = colour;
		}
	}

	uint32_t LedStrip::ScalePixelData(uint32_t data, uint16_t scale)
	{
		if (applyGamma == true)
		{
			data = Colour::Gamma32(data);
		}
		uint32_t dataA = (data & 0x00FF00FF) * scale;
		uint32_t dataB = ((data >> 8) & 0x00FF00FF) * scale;
		return ((dataA >> 8) & 0x00FF00FF | (dataB & 0xFF00FF00));
	}
}