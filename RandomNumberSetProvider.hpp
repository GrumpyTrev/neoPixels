#pragma once
#include "RandomNumberProvider.hpp"
#include <vector>

using namespace std;

namespace Lights
{
	class RandomNumberSetProvider : public RandomNumberProvider
	{
	public:
		inline RandomNumberSetProvider(uint min, uint max) : RandomNumberProvider(min, max) {};

		void Next();

	private:
		vector<uint> numberset;
	};
}