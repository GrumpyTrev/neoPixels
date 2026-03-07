#pragma once
#include "NumberProvider.hpp"

namespace Lights
{
    class ExpressionProvider : public NumberProvider
    {
    public:

        /// @brief Constructor. Copy the supplied number and operand providers locally
        /// @param objects 
        inline ExpressionProvider( vector<BaseDefinedObject*> const& objects ) : NumberProvider( 0 )
        {
            copy( objects.begin(), objects.end(), back_inserter( expressionVector ) );
        }

        int32_t Value();

    private:

        /// @brief The set of numbers and operands that are evaluated to produce this providers value
        vector<BaseDefinedObject*> expressionVector;
    };
}