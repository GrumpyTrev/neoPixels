#pragma once
#include "ProviderBase.hpp"

namespace Lights
{
	/// @brief The NumberProvider supplies a fixed uint16 number
	class NumberProvider : public ProviderBase
	{
	public:
		inline NumberProvider( uint16_t value ) : providedValue( value ) {};

		/// @brief Return the number held by this provider
		///		   Although NumberProvider is fixed, it is the base class for other number providers
		///		   that may support more complex behaviour.
		///        Check for a self triggering.
		/// @return 
		inline virtual uint16_t Value()
		{
			uint16_t retVal = providedValue;

			// Is this is self triggering then get the next value
			if ( selfIncrement == true )
			{
				Next();
			}

			return retVal;
		}

		/// @brief Return the current value without going through Value()
		/// @return 
		inline virtual uint16_t GetValue() { return providedValue; }

		/// @brief Directly set the uncerlyinmg value
		/// @param value 
		inline virtual void SetValue( uint16_t value ) { providedValue = value; }

	protected:
		uint16_t providedValue = 0;
	};
}