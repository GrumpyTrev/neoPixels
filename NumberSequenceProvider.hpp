#pragma once
#include "SimpleSequenceProvider.hpp"
#include "NumberProvider.hpp"

namespace Lights
{
	class NumberSequenceProvider : public NumberProvider
	{
	public:
		inline NumberSequenceProvider() { providerImplementation = new SimpleSequenceProvider<uint>(0); }
	};
}