#pragma once
#include "Action.hpp"
#include <iostream>

namespace Lights
{
	/// @brief Action to set a pixel to a specified colour
	class SetAction : public Action
	{
	public:
		/// @brief Explicit default constructor
		inline SetAction() {}

		/// @brief Copy constructor
		/// @param rhs
		inline SetAction(const SetAction &rhs) : Action(rhs)
		{
			ledNumberProvider = rhs.ledNumberProvider;
			whileProvider = rhs.whileProvider;
			fillWithSingleColour = rhs.fillWithSingleColour;
		};

		/// @brief Clone the item.
		/// @return
		inline SetAction *Clone() const { return new SetAction(*this); }

		/// @brief Allow the LED NumberProvider to be specified
		/// @param provider
		inline void LedProvider( NumberProvider* provider ) { ledNumberProvider = provider; }

		/// @brief Allow the While NumberProvider to be specified
		/// @param provider

		/// @brief Set the fillWithSingleColour flag
		/// @param value 
		inline void FillWithSingleColour( bool value ) { fillWithSingleColour = value; }

	protected:
		/// @brief Set the pixel to the colour given by the provider
		/// @param count
		inline void Execute() 
		{
			// If there is no numberProvider then fill all the pixels in the sequence.
			if ( ledNumberProvider == nullptr )
			{
				Lights::Segment* segment = itemSegment->Value();

				// Should the same colour be used for all the LEDs
				if ( fillWithSingleColour == true )
				{
					if ( ExecutionAllowed() == true )
					{
						segment->Fill( actionColourProvider->Value() );
					}
				}
				else
				{
					// Don't use Fill here as we want to call the colour provider for each pixel
					for ( uint16_t index = 0; index < segment->NumLeds(); index++ )
					{
						if ( ExecutionAllowed() == true )
						{
							segment->SetPixelColour( index, actionColourProvider->Value() );
						}
					}
				}
			}
			else
			{
				if ( ExecutionAllowed() == true )
				{
					int32_t ledNumber = ledNumberProvider->Value();
					Colour ledColour = actionColourProvider->Value();
					itemSegment->Value()->SetPixelColour( ledNumber, ledColour );

					if ( TraceOn() == true )
					{
						HSVColour traceColour = ledColour.ToHSV();

						cout << "Set " << Name() << " " << ledNumber << " hue " << traceColour.hue << " val " << (uint16_t)traceColour.value << "\n";
					}
				}
			}
		}

	private:



		/// @brief A NumberProvider used to provide the Led number
		NumberProvider* ledNumberProvider = nullptr;

		/// @brief A flag used to determine whether or not a single colour should be used when setting the 
		/// entire segment
		bool fillWithSingleColour = false;
	};
}