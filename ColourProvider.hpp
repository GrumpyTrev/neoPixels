#pragma once
#include "Colour.hpp"
#include "ProviderBase.hpp"
#include "NumberProvider.hpp"
#include <iostream>

namespace Lights
{
	/// @brief A provider to supply a single fixed colour
	class ColourProvider : public ProviderBase
	{
	public:
		inline ColourProvider( Colour initial ) : providedValue( initial ) {};
		inline virtual Colour Value()
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
				cout << "Provider " << Name() << " " << providedValue.value << "\n";
			}

			return providedValue;
		}

		inline virtual Colour GetValue() { return providedValue; }
		inline virtual void SetValue( Colour value ) { providedValue = value; }

		/// @brief Set the optional indexer
		/// @param indexer 
		inline void Indexer( NumberProvider* indexer ) { indexProvider = indexer; }

	protected:
		Colour providedValue = Colour::InvalidColour;

		/// @brief Optional indexer used for direct access
		NumberProvider* indexProvider = nullptr;
	};
}