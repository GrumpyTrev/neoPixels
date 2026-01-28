#pragma once
#include "Action.hpp"
#include "NumberProvider.hpp"

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
			numberProvider = rhs.numberProvider;
		};

		/// @brief Clone the item.
		/// @return
		inline SetAction *Clone() const { return new SetAction(*this); }

		/// @brief Allow the pixel NumberProvider to be specified
		/// @param provider
		inline void PixelProvider(NumberProvider *provider) { numberProvider = provider; }

	protected:
		/// @brief Set the pixel to the colour given by the provider
		/// @param count
		inline void Execute() 
		{ 
			// If there is no numberProvider then fill all the pixels in the sequence.
			if ( numberProvider == nullptr )
			{
				// Don't use Fill here as we want to call the colour provider for each pixel
				Lights::Segment* segment = itemSegment->Value();
				for ( uint16_t index = 0; index < segment->NumLeds(); index++ )
				{
					segment->SetPixelColour( index, actionColourProvider->Value() );
				}
			}
			else
			{
				itemSegment->Value()->SetPixelColour( numberProvider->Value(), actionColourProvider->Value() );
			}
		}

	private:
		/// @brief A NumberProvider used to provide the pixel number
		NumberProvider *numberProvider;
	};
}