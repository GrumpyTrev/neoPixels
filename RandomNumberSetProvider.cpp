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
			// Fill the set with the sequence of numbers from 'minimum' to 'maximum' - 1
			for ( uint16_t index = minimum; index < maximum; ++index )
			{
				numberset.push_back(index);
			}
		}

		// Select one of the set at random. This will be in the range 0 to numberset.size() - 1
		uint16_t numberIndex = AnimationHelper::Random16( numberset.size() );
		SetValue(numberset.at(numberIndex));

		// Remove from the set of numbers
		numberset.erase(numberset.begin() + numberIndex);
	}
}