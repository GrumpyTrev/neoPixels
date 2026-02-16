#include "ColourWheelProvider.hpp"
#include "AnimationHelper.hpp"

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

		// Set the index to a random value
		colourIndex = AnimationHelper::RandomColourWheelIndex(colourIndex);

		// Get the value from the colour wheel
		SetValue( AnimationHelper::ColourWheel( colourIndex ) );
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