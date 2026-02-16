#include "ColourFadeProvider.hpp"

namespace Lights
{
	/// @brief Get the next value for the fade
	void ColourFadeProvider::Next()
	{
		// Set the value as determined from the fadeAmount
		SetValue( Colour::Blend( sourceProvider->Value(), destinationProvider->Value(), fadeAmount ) );

		// Update the fadeAmount for next time
		if ( ++fadeCount < fadeSteps )
		{
			if ( reverse == false )
			{
				fadeAmount += fadeInterval;
			}
			else
			{
				fadeAmount -= fadeInterval;
			}
		}
		else
		{
			// Reverse the blend
			fadeCount = 0;
			reverse = !reverse;
		}
	}

	/// @brief Return the current value for the fade
	/// @return
	Colour ColourFadeProvider::Value()
	{
		if (initialised == false)
		{
			InitialiseFade();
			Next();
		}

		return ColourProvider::Value();
	}

	/// @brief Initialise the fade operation from it's colour providers
	void ColourFadeProvider::InitialiseFade()
	{
		// If the source or destination is Black then reduce the range of fade values
		// If the source is black then start with more of the destination in the mix
		if ( ( sourceProvider->Value().value == Colour::Black.value ) ||
			( destinationProvider->Value().value == Colour::Black.value ) )
		{
			fadeInterval = 192 / fadeSteps;

			fadeAmount = ( sourceProvider->Value().value == Colour::Black.value ) ? 64 : 0;
		}
		else
		{
			fadeInterval = 256 / fadeSteps;
			fadeAmount = 0;
		}

		fadeCount = 0;
		reverse = false;
		initialised = true;
	}
}