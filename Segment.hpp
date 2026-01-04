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
		inline Segment(LedStrip *target, uint start, uint ledCount)
			: targetStrip(target), startIndex(start), numLeds(ledCount) {}
		inline Segment(LedStrip *target) : Segment(target, 0, target->NumLeds()) {}
		virtual inline Colour PixelColour(uint index) { return targetStrip->PixelColour(index + startIndex); };
		virtual inline void SetPixelColour(uint index, Colour colour) { targetStrip->SetPixelColour(index + startIndex, colour); };
		inline void Show() { targetStrip->Show(); };
		void Fill(Colour colour, uint first, uint count);
		inline void Fill(Colour colour) { Fill(colour, 0, numLeds); };
		inline uint NumLeds() { return numLeds; };
		inline LedStrip *Target() { return targetStrip; };
		void Shift();

	protected:
		uint numLeds;
		LedStrip *targetStrip;

	private:
		uint startIndex;
	};
}