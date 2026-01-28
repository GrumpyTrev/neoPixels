#pragma once
#include "Colour.hpp"
#include "ProviderBase.hpp"

namespace Lights
{
	/// @brief A provider to supply a single fixed colour
	class ColourProvider : public ProviderBase
	{
	public:
		inline ColourProvider( Colour initial ) : providedValue( initial ) {};
		inline virtual Colour Value()
		{
			Colour retVal = providedValue;

			if ( selfIncrement == true )
			{
				Next();
			}

			return retVal;
		}
		inline virtual Colour GetValue() { return providedValue; }
		inline virtual void SetValue( Colour value ) { providedValue = value; }

	protected:
		Colour providedValue = false;
	};
}