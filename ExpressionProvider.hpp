#pragma once
#include "NumberProvider.hpp"

namespace Lights
{
    class ExpressionProvider : public NumberProvider
    {
    public:

        /// @brief Constructor. Copy the supplied number and operand providers locally
        /// @param objects 
        inline ExpressionProvider( NumberProvider* init, vector<BaseDefinedObject*> const& objects ) :
            initProvider( init ), NumberProvider( 0 )
        {
            copy( objects.begin(), objects.end(), back_inserter( expressionVector ) );
        }

        /// @brief Evaluate the expression
        virtual void Next();

        /// @brief Initialise the expression, to either an initial value, or the first calculated value
        inline virtual void Initialise()
        {
            if ( initProvider != nullptr )
            {
                providedValue = initProvider->Value();
            }
            else
            {
                Next();
            }
        }

    private:
        /// @brief The set of numbers and operands that are evaluated to produce this providers value
        vector<BaseDefinedObject*> expressionVector;

        /// @brief Optional initial value for the expression
        NumberProvider* initProvider = nullptr;
    };
}