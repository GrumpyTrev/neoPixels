#pragma once
#include "NumberProvider.hpp"

namespace Lights
{
    /// @brief The NumberIntervalProvider provides basic 'counter' functionality with min, max and step values.
    class NumberIntervalProvider : public NumberProvider
    {
    public:
        inline NumberIntervalProvider( NumberProvider* min, NumberProvider* max, int32_t step, bool reverse ) :
            startProvider( min ), endProvider( max ), initialStep( step ), reverseAtEnd( reverse ),
            NumberProvider( 0 ) {
        };

        /// @brief Supply the next number
        inline virtual void Next()
        {
            // If this is the first time this has been run, or after a reset, start from the start again and cache
            // the NumberProvider start and end values
            if ( firstRun == true )
            {
                start = startProvider->Value();
                end = endProvider->Value();
                currentStep = initialStep;
                SetValue( start );

                // If start > end, then reverse the cached limits and make sure the step is negative
                if ( start > end )
                {
                    int32_t temp = start;
                    start = end;
                    end = temp;

                    if ( currentStep > 0 )
                    {
                        currentStep = -currentStep;
                    }
                }

                firstRun = false;
            }
            else
            {
                int32_t nextValue = GetValue() + currentStep;

                // If the maximum has been reached, then either start again at the minimum or reverse the counter
                if ( nextValue > end )
                {
                    if ( reverseAtEnd == false )
                    {
                        nextValue = start;
                    }
                    else
                    {
                        nextValue = end;
                        currentStep = -currentStep;
                    }
                }
                else if ( nextValue < start )
                {
                    // If the minimum has been reached then either start again at the end or reverse the counter
                    if ( reverseAtEnd == false )
                    {
                        nextValue = end;
                    }
                    else
                    {
                        nextValue = start;
                        currentStep = -currentStep;
                    }
                }

                SetValue( nextValue );
            }
        }

        /// @brief Reset the provider
        inline virtual void Reset() { firstRun = true; }

    protected:
        /// @brief Start number provider
        NumberProvider* startProvider;

        /// @brief End number provider
        NumberProvider* endProvider;

        /// @brief The limits provided by the providers
        int32_t start;
        int32_t end;

        /// @brief The step between supplied numbers provided by the constructor
        int32_t initialStep;

        /// @brief The step currently being used, this may be reversed if the reverse flag is set
        int32_t currentStep;

        /// @brief Flag used to detect when this provider is first run
        bool firstRun = true;

        /// @brief Controls whether the sequence reverses at the end
        bool reverseAtEnd = false;
    };
}