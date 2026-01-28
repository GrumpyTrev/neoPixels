#pragma once
#include "NumberProvider.hpp"

namespace Lights
{
	class NumberSequenceProvider : public NumberProvider
	{
	public:
		inline NumberSequenceProvider() : NumberProvider( 0 ) {}
		inline void Next() { providedValue = sequence.Next(); }
		inline void AddValue( uint16_t valueToAdd ) { sequence.Add( valueToAdd ); }

	private:
		ProviderSequenceHelper<uint16_t> sequence;
	};
}