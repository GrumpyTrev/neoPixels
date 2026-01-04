#include "ColourWheelProvider.hpp"
#include "AnimationHelper.hpp"

// TESTING
#include <iostream>

namespace Lights
{
	/// @brief Supply the next colour.
	void ColourWheelProvider::Next()
	{
		// Initialise the colourIndex if not done yet
		if (GetValue().value == Colour::InvalidColour.value)
		{
			colourIndex = AnimationHelper::Random8();
		}

		// Set hte index to a random value
		colourIndex = AnimationHelper::RandomColourWheelIndex(colourIndex);

		// Get the value from the colour wheel
		SetValue(AnimationHelper::ColourWheel(colourIndex));

		cout << "ColourWheelProvider Index " << uint(colourIndex) << " Value " << GetValue().value << "\n";
	}

	/// @brief Return the current colour
	/// @return
	Colour ColourWheelProvider::Value()
	{
		// Make sure to get a colour if this provider has not been initialised yet
		if (GetValue().value == Colour::InvalidColour.value)
		{
			Next();
		}

		return ColourProvider::Value();
	};
}