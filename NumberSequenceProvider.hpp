#pragma once
#include "NumberProvider.hpp"

namespace Lights
{
	class NumberSequenceProvider : public NumberProvider
	{
	public:
		inline NumberSequenceProvider() : NumberProvider( 0 ) {}
		inline void Next() { providedValue = sequence.Next(); }
		inline void Reset() { sequence.Reset(); }
		inline void AddValue( int32_t valueToAdd ) { sequence.Add( valueToAdd ); }

	private:
		ProviderSequenceHelper<int32_t> sequence;
	};
}