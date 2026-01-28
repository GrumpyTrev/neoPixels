#pragma once
#include "ColourProvider.hpp"

namespace Lights
{
	class ColourFadeProvider : public ColourProvider
	{
	public:
		inline ColourFadeProvider( ColourProvider* source, ColourProvider* destination, uint16_t steps )
			: ColourProvider(Colour::InvalidColour), fadeSteps(steps), sourceProvider(source),
			  destinationProvider(destination) {};
		void Next();
		Colour Value();

	private:
		uint16_t fadeSteps = 0;
		uint16_t fadeInterval = 0;
		uint16_t fadeCount = 0;
		uint16_t fadeAmount = 0;
		bool reverse = false;
		bool initialised = false;

		ColourProvider *sourceProvider;
		ColourProvider *destinationProvider;
	};
}