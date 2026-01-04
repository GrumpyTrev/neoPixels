#pragma once
#include "ColourProvider.hpp"

namespace Lights
{
	class ColourFadeProvider : public ColourProvider
	{
	public:
		inline ColourFadeProvider(ColourProvider *source, ColourProvider *destination, uint steps)
			: ColourProvider(Colour::InvalidColour), fadeSteps(steps), sourceProvider(source),
			  destinationProvider(destination) {};
		void Next();
		Colour Value();

	private:
		uint fadeSteps = 0;
		uint fadeInterval = 0;
		uint fadeCount = 0;
		uint fadeAmount = 0;
		bool reverse = false;
		bool initialised = false;

		ColourProvider *sourceProvider;
		ColourProvider *destinationProvider;
	};
}