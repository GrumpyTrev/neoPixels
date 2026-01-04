#pragma once
#include "SimpleSequenceProvider.hpp"
#include "ColourProvider.hpp"

namespace Lights
{
	class ColourSequenceProvider : public ColourProvider
	{
	public:
		inline ColourSequenceProvider() { providerImplementation = new SimpleSequenceProvider<Colour>(Colour::InvalidColour); };
	};
}