#pragma once
#include "Segment.hpp"

namespace Lights
{
	/// @brief The MappedSegment add pixel mapping capability to the Segment class
	class MappedSegment : public Segment
	{
	public:
		inline MappedSegment(LedStrip *target) : Segment(target) {};

		/// @brief Delete the mapping memory
		inline ~MappedSegment() { delete pixelMap; };

		/// @brief Set a pixel colour through the mapping
		/// @param index
		/// @param colour
		inline void SetPixelColour(uint index, Colour colour)
		{
			if (index < numLeds)
			{
				targetStrip->SetPixelColour(pixelMap[index], colour);
			}
		}

		/// @brief Get a pixel colour through the mapping
		/// @param index
		/// @return
		inline Colour PixelColour(uint index)
		{
			Colour pixelColour = Colour::InvalidColour;
			if (index < numLeds)
			{
				pixelColour = targetStrip->PixelColour(pixelMap[index]);
			}

			return pixelColour;
		}

	protected:
		// Mapping between the segment pixel number and the underlying strip
		uint *pixelMap;
	};
}