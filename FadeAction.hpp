#pragma once
#include "Action.hpp"

namespace Lights
{
	/// @brief Reduce or increase the brightness of all LEDs by the specified amount
	class FadeAction : public Action
	{
	public:
		/// @brief Explicit default constructor
		inline FadeAction() {}

		/// @brief Copy constructor
		/// @param rhs
		inline FadeAction(const FadeAction &rhs) : Action(rhs) 
		{
			fadePercentage = rhs.fadePercentage;
		};

		/// @brief Clone the item.
		/// @return
		inline FadeAction *Clone() const { return new FadeAction(*this); }

		/// @brief Set the amount to increase or decrease the brightness
		/// @param percentage 
		inline void Percentage( uint16_t percentage ) { fadePercentage = percentage; }

	protected:
		
		/// @brief Called to change the brighness of all the LEDs in the current segment
		/// For each LED get the colour as an HSV tuple. Change the brightness (V) and write it back
		inline void Execute() 
		{ 
			Lights::Segment* segment = itemSegment->Value();

			for ( uint16_t index = 0; index < segment->NumLeds(); index++ )
			{
				HSVColour hsv = segment->PixelColour( index ).ToHSV();
				hsv.value = ( hsv.value * fadePercentage ) / 100;
				segment->SetPixelColour( index, Colour::ColourHSV( hsv.hue, hsv.saturation, hsv.value ));
			}
		}

	private:
		
		/// @brief How much to reduce or increase the brightness of all the LEDS
		uint16_t fadePercentage = 0;
	};
}