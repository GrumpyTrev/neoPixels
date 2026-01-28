#pragma once
#include "NumberProvider.hpp"
#include "AnimationHelper.hpp"

namespace Lights
{
	class RandomNumberProvider : public NumberProvider
	{
	public:
		inline RandomNumberProvider( uint16_t min, uint16_t max ) : minimum( min ), maximum( max ), NumberProvider( 0 ) {};

		/// @brief Supply the next colour
		inline virtual void Next()
		{
			SetValue( AnimationHelper::Random16( minimum, maximum ) );
		}

	protected:
		/// @brief Minimum number
		uint16_t minimum;

		/// @brief Maximum number
		uint16_t maximum;
	};
}