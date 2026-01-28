#pragma once

#include <vector>
#include "MappedSegment.hpp"

using namespace std;

namespace Lights
{
	/// @brief Represents a set of discretely numbered pixels
	class DiscreteSegment : public MappedSegment
	{
	public:
		DiscreteSegment( LedStrip* target, vector<uint16_t> pixels );

	private:
	};
}