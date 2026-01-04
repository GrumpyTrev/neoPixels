#pragma once
#include "NumberProvider.hpp"

// TESTING
#include <iostream>

namespace Lights
{
	class NumberIntervalProvider : public NumberProvider
	{
	public:
		inline NumberIntervalProvider(uint min, uint max, uint step, bool reverse) : 
            minimum(min), maximum(max), interval(step), reverseAtEnd( reverse ), NumberProvider(min) {};

		/// @brief Supply the next number
		inline virtual void Next()
		{
            if (firstRun == true )
            {
                SetValue( minimum );
                firstRun = false;
            }
            else
            {
                uint nextValue = 0;

                if ( reversing == false )
                {
                    nextValue = GetValue() + interval;
                    if ( nextValue > maximum ) 
                    {
                        // If reverseAtEnd is false then start agsain at the minimum.
                        // Otherwise start at the maximum and reverse direction
                        if ( reverseAtEnd == false )
                        {
                            nextValue = minimum;
                        }
                        else
                        {
                            reversing = true;
                            nextValue = maximum;
                        }
                    }
                }
                else
                {
                    if ( GetValue() < ( minimum + interval ) ) 
                    {
                        // Start reached. Reverse direction
                        reversing = false;
                        nextValue = minimum;
                    }
                    else
                    {
                        nextValue = GetValue() - interval;
                    }
                }
 
                SetValue( nextValue );
            }
			// cout << "NumberIntervalProvider " << Value() << "\n";
		}

	protected:
		/// @brief Minimum number
		uint minimum;

		/// @brief Maximum number
		uint maximum;

        /// @brief The interval between supplied numbers
        uint interval;

        /// @brief Flag used to detect when this provider is first run
        bool firstRun = true;

        /// @brief Controls whether the sequence reverses at the end
        bool reverseAtEnd = false;

        /// @brief Is the sequence currently reversing
        bool reversing = false;
	};
}