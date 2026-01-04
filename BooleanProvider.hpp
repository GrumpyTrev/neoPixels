#pragma once
#include "SimpleProvider.hpp"

namespace Lights
{
	/// @brief Class used to wrap up a boolean in a provider
	class BooleanProvider : public BaseProvider<bool>
	{
	public:
		inline BooleanProvider(bool value)
		{
			providerImplementation = new SimpleProvider<bool>(value);
		}

	protected:
		inline BooleanProvider() {};
	};
}