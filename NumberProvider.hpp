#pragma once
#include "SimpleProvider.hpp"

namespace Lights
{
	class NumberProvider : public BaseProvider<uint>
	{
	public:
		inline NumberProvider(uint count) { providerImplementation = new SimpleProvider<uint>(count); };

	protected:
		inline NumberProvider() {};
	};
}