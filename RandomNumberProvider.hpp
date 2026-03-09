#pragma once
#include "NumberProvider.hpp"
#include <iostream>

namespace Lights
{
	class RandomNumberProvider : public NumberProvider
	{
	public:
		inline RandomNumberProvider( uint16_t min, uint16_t max ) : minimum( min ), maximum( max ), NumberProvider( 0 )
		{
		}

		/// @brief Supply the next number
		inline virtual void Next()
		{
			// Set the initialise flag in case this is called before a value is obtained
			initialised = true;

			SetValue( Random16( minimum, maximum ) );

			if ( TraceOn() == true )
			{
				cout << "Random " << Name() << " set to " << GetValue() << "\n";
			}
		}

		/// @brief Initialise the provider
		inline virtual void Initialise() { Next(); }

		static inline uint16_t Random16()
		{
			randomSeed16 = ( randomSeed16 * 2053 ) + 13849;
			return randomSeed16;
		}

		static inline uint16_t Random16( uint16_t lim ) { return ( Random16() * lim ) >> 16; }

		static inline uint16_t Random16( uint16_t min, uint16_t lim ) { return Random16( lim - min ) + min; }

	protected:
		/// @brief Minimum number
		uint16_t minimum;

		/// @brief Maximum number
		uint16_t maximum;

	private:
		// 16 bit random number seed
		inline static uint16_t randomSeed16 = 0;
	};
}