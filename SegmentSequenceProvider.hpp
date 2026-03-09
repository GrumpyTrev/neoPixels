#pragma once
#include "SegmentProvider.hpp"

namespace Lights
{
	class SegmentSequenceProvider : public SegmentProvider
	{
	public:
		inline SegmentSequenceProvider() : SegmentProvider( nullptr ) {}
		inline void Next()
		{
			providedValue = ( indexProvider == nullptr ) ? sequence.Next() : sequence[ indexProvider->Value() ];

			if ( TraceOn() == true )
			{
				cout << "SegmentSequenceProvider " << Name() << " " << providedValue->Name() << "\n";
			}
		}
		inline void Initialise() { sequence.Reset(); Next(); }
		inline void AddValue( Segment* valueToAdd ) { sequence.Add( valueToAdd ); }

	private:
		ProviderSequenceHelper<Segment*> sequence;
	};
}