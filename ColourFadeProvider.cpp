#include "ColourFadeProvider.hpp"

namespace Lights
{
	/// @brief Start a fade operation by determining the fade interval
	void ColourFadeProvider::Next()
	{
		// If the source or destination is Black then reduce the range of fade values
		// If the source is black then start with more of the destination in the mix
		if ((sourceProvider->Value().value == Colour::Black.value) ||
			(destinationProvider->Value().value == Colour::Black.value))
		{
			fadeInterval = 192 / fadeSteps;

			fadeAmount = (sourceProvider->Value().value == Colour::Black.value) ? 64 : 0;
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

	/// @brief Return a blend of the source and destination colours
	/// @return
	Colour ColourFadeProvider::Value()
	{
		if (initialised == false)
		{
			Next();
		}

		Colour fadeColour = Colour::Blend(sourceProvider->Value(), destinationProvider->Value(), fadeAmount);
		if (++fadeCount < fadeSteps)
		{
			if (reverse == false)
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

		SetValue(fadeColour);

		return ColourProvider::Value();
	}
}