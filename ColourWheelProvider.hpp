#pragma once
#include "ColourProvider.hpp"

namespace Lights
{
	/// @brief A provider to supply colours from the colour wheel.
	class ColourWheelProvider : public ColourProvider
	{
	public:
		inline ColourWheelProvider() : ColourProvider(Colour::InvalidColour){};

		/// @brief Supply the next colour
		void Next();

		/// @brief Return the current colour
		/// @return
		Colour Value();

	private:
		/// @brief The index into the colour wheel previously used.
		/// This is used as a seed for the next colour
		uint8_t colourIndex = 0;
	};
}