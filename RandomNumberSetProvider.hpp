#pragma once
#include "RandomNumberProvider.hpp"
#include <vector>

namespace Lights
{
	class RandomNumberSetProvider : public RandomNumberProvider
	{
	public:
		inline RandomNumberSetProvider( uint16_t min, uint16_t max ) : RandomNumberProvider( min, max ) {};

		void Next();

		inline void Reset() { numberset.clear(); Next(); }

	private:
		vector<uint16_t> numberset;
	};
}