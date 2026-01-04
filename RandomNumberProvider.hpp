#pragma once
#include "NumberProvider.hpp"
#include "AnimationHelper.hpp"

// TESTING
#include <iostream>

namespace Lights
{
	class RandomNumberProvider : public NumberProvider
	{
	public:
		inline RandomNumberProvider(uint min, uint max) : minimum(min), maximum(max), NumberProvider(0) {};

		/// @brief Supply the next colour
		inline virtual void Next()
		{
			SetValue(AnimationHelper::Random8(minimum, maximum));
			// cout << "Setting random number to " << Value() << "\n";
		}

	protected:
		/// @brief Minimum number
		uint minimum;

		/// @brief Maximum number
		uint maximum;
	};
}