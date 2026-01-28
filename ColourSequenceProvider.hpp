#pragma once
#include "ColourProvider.hpp"

namespace Lights
{
	class ColourSequenceProvider : public ColourProvider
	{
	public:
		inline ColourSequenceProvider() : ColourProvider( Colour::InvalidColour ) {}
		inline void Next() { providedValue = sequence.Next(); }
		inline void AddValue( Colour valueToAdd ) { sequence.Add( valueToAdd ); }

	private:
		ProviderSequenceHelper<Colour> sequence;
	};
}