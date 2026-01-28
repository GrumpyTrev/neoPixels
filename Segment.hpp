#pragma once

#include <string>
#include "LedStrip.hpp"
#include "BaseDefinedObject.hpp"

namespace Lights
{
	/// @brief Represents a part of an LedStrip and allows pixel operations to be performed
	/// on that part independent of the rest of the strip.
	class Segment : public BaseDefinedObject
	{
	public:
		inline Segment( LedStrip* target, uint16_t start, uint16_t ledCount )
			: targetStrip(target), startIndex(start), numLeds(ledCount) {}
		inline Segment(LedStrip *target) : Segment(target, 0, target->NumLeds()) {}
		virtual inline Colour PixelColour( uint16_t index ) { return targetStrip->PixelColour( index + startIndex ); };
		virtual inline void SetPixelColour( uint16_t index, Colour colour ) { targetStrip->SetPixelColour( index + startIndex, colour ); };
		inline void Show() { targetStrip->Show(); };
		void Fill( Colour colour, uint16_t first, uint16_t count );
		inline void Fill(Colour colour) { Fill(colour, 0, numLeds); };
		inline uint16_t NumLeds() { return numLeds; };
		inline LedStrip *Target() { return targetStrip; };
		void Shift();

	protected:
		uint16_t numLeds;
		LedStrip *targetStrip;

	private:
		uint16_t startIndex;
	};
}