#pragma once
#include "ProviderBase.hpp"
#include "iostream"

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
		/// @return 
		inline virtual int32_t Value()
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

			return providedValue;
		}

		/// @brief Return the current value without going through Value()
		/// @return 
		inline virtual int32_t GetValue() { return providedValue; }

		/// @brief Directly set the underlying value
		/// @param value 
		inline virtual void SetValue( int32_t value ) { providedValue = value; }

		/// @brief Set the optional indexer
		/// @param indexer 
		inline void Indexer( NumberProvider* indexer ) { indexProvider = indexer; }

	protected:

		/// @brief The value to be returned by this provider
		int32_t providedValue = 0;

		/// @brief Optional indexer used for direct access
		NumberProvider* indexProvider = nullptr;
	};
}