#pragma once
#include "NumberProvider.hpp"

namespace Lights
{
    /// @brief The NumberIntervalProvider provides basic 'counter' functionality with min, max and step values.
    ///        It can be self triggering, i.e. it can provide the next value automatically rather than being triggered
    class NumberIntervalProvider : public NumberProvider
    {
    public:
        inline NumberIntervalProvider( NumberProvider* min, NumberProvider* max, uint16_t step, bool reverse ) :
            minimumProvider( min ), maximumProvider( max ), interval( step ), reverseAtEnd( reverse ),
            NumberProvider( 0 ) {
        };

        /// @brief Supply the next number
        inline virtual void Next()
        {
            // If this is the first time this has been run, or after a reset, start from the minimum again and cache
            // the NumberProvider min and max value
            if ( firstRun == true )
            {
                minimum = minimumProvider->Value();
                maximum = maximumProvider->Value();
                reversing = false;
                SetValue( minimum );

                firstRun = false;
            }
            else
            {
                // Use 32 bit here in case maximum is 65535
                uint32_t nextValue = 0;

                if ( reversing == false )
                {
                    nextValue = GetValue() + interval;
                    if ( nextValue > maximum )
                    {
                        // If reverseAtEnd is false then start again at the minimum.
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
                    // Will the next step go negative
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

                SetValue( (uint16_t)nextValue );
            }
        }

        /// @brief Reset the provider
        inline virtual void Reset() { firstRun = true; }

    protected:
        /// @brief Minimum number provider
        NumberProvider* minimumProvider;

        /// @brief Maximum number provider
        NumberProvider* maximumProvider;

        /// @brief The limits provided by the providers
        uint16_t minimum;
        uint16_t maximum;

        /// @brief The interval between supplied numbers
        uint16_t interval;

        /// @brief Flag used to detect when this provider is first run
        bool firstRun = true;

        /// @brief Controls whether the sequence reverses at the end
        bool reverseAtEnd = false;

        /// @brief Is the sequence currently reversing
        bool reversing = false;
    };
}