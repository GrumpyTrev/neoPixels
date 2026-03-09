#pragma once
#include "ProviderBase.hpp"
#include "Segment.hpp"

namespace Lights
{
	/// @brief Class used to wrap up a Segment in a provider
	class SegmentProvider : public ProviderBase
	{
	public:
		inline SegmentProvider( Segment* initial ) : providedValue( initial ) {};
		inline virtual Segment* Value()
		{
			// Does this provider require initialising
			if ( InitialisationRequired() == false )
			{
				// Is this is self triggering then get the next value
				if ( selfIncrement == true )
				{
					Next();
				}
			}

			if ( TraceOn() == true )
			{
				cout << "Provider " << Name() << " " << providedValue->Name() << "\n";
			}

			return providedValue;
		}
		inline virtual Segment* GetValue() { return providedValue; }
		inline virtual void SetValue( Segment* value ) { providedValue = value; }

		/// @brief Set the optional indexer
		/// @param indexer 
		inline void Indexer( NumberProvider* indexer ) { indexProvider = indexer; }

	protected:

		Segment* providedValue = nullptr;

		/// @brief Optional indexer used for direct access
		NumberProvider* indexProvider = nullptr;
	};
}