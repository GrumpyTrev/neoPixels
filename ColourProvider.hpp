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
			// Does this provider require initialising
			if ( initialised == false )
			{
				Initialise();
				initialised = true;
			}
			// Is this is self triggering then get the next value
			else if ( selfIncrement == true )
			{
				Next();
			}

			return providedValue;
		}

		inline virtual Colour GetValue() { return providedValue; }
		inline virtual void SetValue( Colour value ) { providedValue = value; }

	protected:
		Colour providedValue = Colour::InvalidColour;
	};
}