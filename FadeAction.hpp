#pragma once
#include "Action.hpp"
#include <math.h>

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
			ledNumberProvider = rhs.ledNumberProvider;
			fadePercentage = rhs.fadePercentage;
		};

		/// @brief Clone the item.
		/// @return
		inline FadeAction* Clone() const { return new FadeAction( *this ); }

		/// @brief Set the amount to increase or decrease the brightness
		/// @param percentage 
		inline void Percentage( uint16_t percentage ) { fadePercentage = percentage; }

		/// @brief Allow the LED NumberProvider to be specified. If not specified then all the segment is faded
		/// @param provider
		inline void LedProvider( NumberProvider* provider ) { ledNumberProvider = provider; }

	protected:
		
		/// @brief Called to change the brighness of all the LEDs in the current segment
		/// For each LED get the colour as an HSV tuple. Change the brightness (V) and write it back
		inline void Execute()
		{ 
			// If an ledNumberProvider has been specified then only update the specified LED
			if ( ledNumberProvider != nullptr )
			{
				UpdateLed( ledNumberProvider->Value() );
			}
			else
			{
				for ( uint16_t index = 0; index < itemSegment->Value()->NumLeds(); index++ )
				{
					UpdateLed( index );
				}
			}
		}

	private:
		void inline UpdateLed( uint16_t ledNunber )
		{
			Lights::Segment* segment = itemSegment->Value();

			HSVColour hsv = segment->PixelColour( ledNunber ).ToHSV();

			if ( fadePercentage < 100 )
			{
				hsv.value = ( hsv.value * ( 100 - fadePercentage ) ) / 100;
			}
			else
			{
				hsv.value = min( 255, ( hsv.value * fadePercentage ) / 100 );
			}

			segment->SetPixelColour( ledNunber, Colour::ColourHSV( hsv.hue, hsv.saturation, hsv.value ) );
		}

		/// @brief A NumberProvider used to provide the Led number
		NumberProvider* ledNumberProvider = nullptr;

		/// @brief How much to reduce or increase the brightness of all the LEDS
		uint16_t fadePercentage = 0;
	};
}