#pragma once
#include "SegmentProvider.hpp"

namespace Lights
{
	class SegmentSequenceProvider : public SegmentProvider
	{
	public:
		inline SegmentSequenceProvider() : SegmentProvider( nullptr ) {}
		inline void Next() { providedValue = sequence.Next(); }
		inline void AddValue( Segment* valueToAdd ) { sequence.Add( valueToAdd ); }

	private:
		ProviderSequenceHelper<Segment*> sequence;
	};
}