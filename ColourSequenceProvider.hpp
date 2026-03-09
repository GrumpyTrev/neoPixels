#pragma once
#include "ColourProvider.hpp"
#include "NumberProvider.hpp"
#include "ostream"

namespace Lights
{
	class ColourSequenceProvider : public ColourProvider
	{
	public:
		inline ColourSequenceProvider() : ColourProvider( Colour::InvalidColour ) {}
		inline void Next()
		{
			providedValue = ( indexProvider == nullptr ) ? sequence.Next() : sequence[ indexProvider->Value() ];

			if ( TraceOn() == true )
			{
				cout << "ColourSequenceProvider " << Name() << " value " << providedValue.value << "\n";
			}
		}
		inline void Initialise() { sequence.Reset(); Next(); }
		inline void AddValue( Colour valueToAdd ) { sequence.Add( valueToAdd ); }

	private:
		ProviderSequenceHelper<Colour> sequence;
	};
}