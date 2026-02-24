#pragma once
#include "Action.hpp"

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
			whenProvider = rhs.whenProvider;
			whenNotProvider = rhs.whenNotProvider;
			fillWithSingleColour = rhs.fillWithSingleColour;
		};

		/// @brief Clone the item.
		/// @return
		inline SetAction *Clone() const { return new SetAction(*this); }

		/// @brief Allow the LED NumberProvider to be specified
		/// @param provider
		inline void LedProvider( NumberProvider* provider ) { ledNumberProvider = provider; }

		/// @brief Allow the When NumberProvider to be specified
		/// @param provider
		inline void WhenProvider( NumberProvider* provider ) { whenProvider = provider; }

		/// @brief Allow the WhenNot NumberProvider to be specified
		/// @param provider
		inline void WhenNotProvider( NumberProvider* provider ) { whenNotProvider = provider; }

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
					if ( CanSet() == true )
					{
						segment->Fill( actionColourProvider->Value() );
					}
				}
				else
				{
					// Don't use Fill here as we want to call the colour provider for each pixel
					for ( uint16_t index = 0; index < segment->NumLeds(); index++ )
					{
						if ( CanSet() == true )
						{
							segment->SetPixelColour( index, actionColourProvider->Value() );
						}
					}
				}
			}
			else
			{
				if ( CanSet() == true )
				{
					itemSegment->Value()->SetPixelColour( ledNumberProvider->Value(), actionColourProvider->Value() );
				}
			}
		}

	private:

		/// @brief Determine whether or not the set operation is currently enabled
		/// @return 
		bool inline CanSet()
		{
			// The operation is allowed if there are no conditions, or if either of the conditions are true
			return ( ( whenProvider == nullptr ) && ( whenNotProvider == nullptr ) ) ||
				( ( whenProvider != nullptr ) && ( whenProvider->Value() > 0 ) ) ||
				( ( whenNotProvider != nullptr ) && ( whenNotProvider->Value() == 0 ) );
		}

		/// @brief A NumberProvider used to provide the Led number
		NumberProvider* ledNumberProvider = nullptr;

		/// @brief Providers determining whether or not the set action should be carried out
		NumberProvider* whenProvider = nullptr;
		NumberProvider* whenNotProvider = nullptr;

		/// @brief A flag used to determine whether or not a single colour should be used when setting the 
		/// entire segment
		bool fillWithSingleColour = false;
	};
}