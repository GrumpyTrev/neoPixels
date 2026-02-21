#pragma once
#include "ProviderBase.hpp"

namespace Lights
{
	/// @brief The NumberProvider supplies a fixed uint16 number
	class NumberProvider : public ProviderBase
	{
	public:
		inline NumberProvider( int32_t value ) : providedValue( value ) {};

		/// @brief Return the number held by this provider
		///		   Although NumberProvider is fixed, it is the base class for other number providers
		///		   that may support more complex behaviour.
		///        Check for a self triggering.
		/// @return 
		inline virtual int32_t Value()
		{
			int32_t retVal = providedValue;

			// Is this is self triggering then get the next value
			if ( selfIncrement == true )
			{
				Next();
			}

			return retVal;
		}

		/// @brief Return the current value without going through Value()
		/// @return 
		inline virtual int32_t GetValue() { return providedValue; }

		/// @brief Directly set the underlyinmg value
		/// @param value 
		inline virtual void SetValue( int32_t value ) { providedValue = value; }

	protected:
		int32_t providedValue = 0;
	};
}