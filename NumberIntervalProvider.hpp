#pragma once
#include "NumberProvider.hpp"
#include <iostream>

namespace Lights
{
    /// @brief The NumberIntervalProvider provides basic 'counter' functionality with min, max and step values.
    class NumberIntervalProvider : public NumberProvider
    {
    public:
        inline NumberIntervalProvider( NumberProvider* min, NumberProvider* max, NumberProvider* step, bool reverse ) :
            startProvider( min ), endProvider( max ), initialStep( step ), reverseAtEnd( reverse ),
            NumberProvider( 0 ) {
        }

        /// @brief Supply the next number
        inline virtual void Next()
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

        /// @brief Initialise this provider,
        inline void Initialise()
        {
            // Cache the NumberProvider start and end values. Start from the start again 
            start = startProvider->Value();
            end = endProvider->Value();
            currentStep = initialStep->Value();

            if ( TraceOn() == true )
            {
                cout << "Counter " << Name() << " Initialised " << start << ":" << end << ":" << currentStep << "\n";
            }

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
        }

    protected:
        /// @brief Start number provider
        NumberProvider* startProvider;

        /// @brief End number provider
        NumberProvider* endProvider;

        /// @brief The limits provided by the providers
        int32_t start;
        int32_t end;

        /// @brief The step between supplied numbers provided by the constructor
        NumberProvider* initialStep;

        /// @brief The step currently being used, this may be reversed if the reverse flag is set
        int32_t currentStep;

        /// @brief Controls whether the sequence reverses at the end
        bool reverseAtEnd = false;
    };
}