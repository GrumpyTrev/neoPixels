#pragma once
#include "NumberProvider.hpp"
#include <iostream>

namespace Lights
{
	class NumberSequenceProvider : public NumberProvider
	{
	public:
		inline NumberSequenceProvider() : NumberProvider( 0 ) {}
		inline virtual void Next()
		{
			providedValue = sequence.Next();

			if ( TraceOn() == true )
			{
				cout << "NumberSequenceProvider " << Name() << " value " << providedValue << "\n";
			}
		}

		inline virtual void Initialise() { sequence.Reset(); Next(); }
		inline void AddValue( int32_t valueToAdd ) { sequence.Add( valueToAdd ); }

	private:
		ProviderSequenceHelper<int32_t> sequence;
	};
}