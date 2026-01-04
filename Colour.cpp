#include "Colour.hpp"
#include <map>

using namespace std;

namespace Lights
{
	const Colour Colour::InvalidColour = Colour(0xffffffff);
	const Colour Colour::White = Colour(0xff, 0xff, 0xff);
	const Colour Colour::Black = Colour(0, 0, 0);

	map<string, Colour> colourMap{
		{"Black", Colour(0, 0, 0)},
		{"White", Colour(0xff, 0xff, 0xff)},
		{"Red", Colour(0xff, 0, 0)},
		{"Green", Colour(0, 0xff, 0)},
		{"Blue", Colour(0, 0, 0xff)},
		{"RubyRed", Colour(24, 17, 95)},
		{"Ochre", Colour(0xff4e20)},
		{"MagentaPink", Colour(0xc70075)},
		{"WarmFluorescent", Colour(0xFFF4E5)} /* 0 K, 255, 244, 229 */
	};

	/// @brief Get a predefined colour from the colour name
	/// @param colourName
	/// @return
	Colour Colour::GetColour(string colourName)
	{
		Colour colourSelected = Colour::InvalidColour;

		map<string, Colour>::iterator it = colourMap.find(colourName);
		if (it != colourMap.end())
		{
			colourSelected = it->second;
		}

		return colourSelected;
	}

	/// @brief Add a names colour to the map. Overwrite any existing colour
	/// @param colourName
	/// @param colourToAdd
	void Colour::AddColour(string colourName, Colour colourToAdd)
	{
		map<string, Colour>::iterator it = colourMap.find(colourName);
		if (it != colourMap.end())
		{
			colourMap.erase(it);
		}

		colourMap[colourName] = colourToAdd;
	}

	// Create Colour from component HSV values
	// Copied from AdaFruit_NeoPixel
	Colour Colour::ColourHSV(uint16_t hue, uint8_t sat, uint8_t val)
	{
		uint8_t r, g, b;

		// Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
		// 0 is not the start of pure red, but the midpoint...a few values above
		// zero and a few below 65536 all yield pure red (similarly, 32768 is the
		// midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
		// each for red, green, blue) really only allows for 1530 distinct hues
		// (not 1536, more on that below), but the full unsigned 16-bit type was
		// chosen for hue so that one's code can easily handle a contiguous color
		// wheel by allowing hue to roll over in either direction.
		hue = (hue * 1530L + 32768) / 65536;
		// Because red is centered on the rollover point (the +32768 above,
		// essentially a fixed-point +0.5), the above actually yields 0 to 1530,
		// where 0 and 1530 would yield the same thing. Rather than apply a
		// costly modulo operator, 1530 is handled as a special case below.

		// So you'd think that the color "hexcone" (the thing that ramps from
		// pure red, to pure yellow, to pure green and so forth back to red,
		// yielding six slices), and with each color component having 256
		// possible values (0-255), might have 1536 possible items (6*256),
		// but in reality there's 1530. This is because the last element in
		// each 256-element slice is equal to the first element of the next
		// slice, and keeping those in there this would create small
		// discontinuities in the color wheel. So the last element of each
		// slice is dropped...we regard only elements 0-254, with item 255
		// being picked up as element 0 of the next slice. Like this:
		// Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
		// Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
		// Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
		// and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
		// the constants below are not the multiples of 256 you might expect.

		// Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
		if (hue < 510)
		{
			// Red to Green-1
			b = 0;
			if (hue < 255)
			{
				// Red to Yellow-1
				r = 255;
				g = hue; //     g = 0 to 254
			}
			else
			{
				// Yellow to Green-1
				r = 510 - hue; //     r = 255 to 1
				g = 255;
			}
		}
		else if (hue < 1020)
		{
			// Green to Blue-1
			r = 0;
			if (hue < 765)
			{
				// Green to Cyan-1
				g = 255;
				b = hue - 510; //     b = 0 to 254
			}
			else
			{
				//   Cyan to Blue-1
				g = 1020 - hue; //     g = 255 to 1
				b = 255;
			}
		}
		else if (hue < 1530)
		{
			// Blue to Red-1
			g = 0;
			if (hue < 1275)
			{
				// Blue to Magenta-1
				r = hue - 1020; //     r = 0 to 254
				b = 255;
			}
			else
			{
				// Magenta to Red-1
				r = 255;
				b = 1530 - hue; //     b = 255 to 1
			}
		}
		else
		{
			// Last 0.5 Red (quicker than % operator)
			r = 255;
			g = b = 0;
		}

		// Apply saturation and value to R,G,B, pack into 32-bit result:
		uint32_t v1 = 1 + val;	// 1 to 256; allows >>8 instead of /255
		uint16_t s1 = 1 + sat;	// 1 to 256; same reason
		uint8_t s2 = 255 - sat; // 255 to 0
		return Colour(((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
					  (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
					  (((((b * s1) >> 8) + s2) * v1) >> 8));
	}

	// A 32-bit variant of gamma8() that applies the same function
	// to all components of a packed RGB or WRGB value.
	uint32_t Colour::Gamma32(uint32_t rgbValue)
	{
		uint8_t *y = (uint8_t *)&rgbValue;
		// All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
		// to avoid a bunch of shifting and masking that would be necessary for
		// properly handling different endianisms (and each byte is a fairly
		// trivial operation, so it might not even be wasting cycles vs a check
		// and branch for the RGB case). In theory this might cause trouble *if*
		// someone's storing information in the unused most significant byte
		// of an RGB value, but this seems exceedingly rare and if it's
		// encountered in reality they can mask values going in or coming out.
		for (uint8_t i = 0; i < 4; i++)
		{
			y[i] = Gamma8(y[i]);
		}

		return rgbValue;
	}

	/// @brief Blend two colours together
	/// @param source
	/// @param destination
	/// @param amount How much of the destination colour is in the blend ( 0 - none, 255 -all )
	/// @return
	Colour Colour::Blend(Colour source, Colour destination, uint8_t amount)
	{
		Colour blendedColour;

		if (amount == 0)
		{
			blendedColour = source;
		}
		else if (amount == 255)
		{
			blendedColour = destination;
		}
		else
		{
			blendedColour.component.red = amount * (destination.component.red - source.component.red) / 256 +
										  source.component.red;
			blendedColour.component.green = amount * (destination.component.green - source.component.green) / 256 +
											source.component.green;
			blendedColour.component.blue = amount * (destination.component.blue - source.component.blue) / 256 +
										   source.component.blue;
		}

		return blendedColour;
	}

	const uint8_t constexpr Colour::GammaTable[256] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
      2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
      6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
      11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
      19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
      29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
      40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
      55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
      71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
      90, 91, 93, 94, 95, 96, 98, 99, 100, 102, 103, 104, 106, 107, 109, 110,
      111, 113, 114, 116, 117, 119, 120, 121, 123, 124, 126, 128, 129, 131, 132, 134,
      135, 137, 138, 140, 142, 143, 145, 146, 148, 150, 151, 153, 155, 157, 158, 160,
      162, 163, 165, 167, 169, 170, 172, 174, 176, 178, 179, 181, 183, 185, 187, 189,
      191, 193, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220,
      222, 224, 227, 229, 231, 233, 235, 237, 239, 241, 244, 246, 248, 250, 252, 255};

}