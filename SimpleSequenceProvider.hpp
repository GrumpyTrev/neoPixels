#pragma once
#include "SimpleProvider.hpp"
#include <vector>

namespace Lights
{
	template <class T>
	class SimpleSequenceProvider : public SimpleProvider<T>
	{
	public:
		inline SimpleSequenceProvider(T initial) : SimpleProvider<T>(initial) {};
		void Next()
		{
			this->providedValue = sequence[index];

			if (++index == sequence.size())
			{
				index = 0;
			}
		};
		inline void AddValue(T valueToAdd) { sequence.push_back(valueToAdd); };

	private:
		std::vector<T> sequence;
		uint index = 0;
	};
}