#include "RandomNumberSetProvider.hpp"

using namespace std;

namespace Lights
{
	/// @brief Provide a random number from the set of numbers not already provided
	/// If all the numbers have been provided (or at startup) refill the set with all the numbers
	void RandomNumberSetProvider::Next()
	{
		if (numberset.size() == 0)
		{
			// Fill the set with the sequence of numbers from 'minimum' to 'maximum'
			for ( uint16_t index = minimum; index <= maximum; ++index )
			{
				numberset.push_back(index);
			}
		}

		// Select one of the set at random
		uint8_t numberIndex = AnimationHelper::Random8( 0, numberset.size() - 1 );
		SetValue(numberset.at(numberIndex));

		// Remove from the set of numbers
		numberset.erase(numberset.begin() + numberIndex);
	}
}