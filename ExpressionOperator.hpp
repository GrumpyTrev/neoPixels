#pragma once
#include "BaseDefinedObject.hpp"

namespace Lights
{
    class ExpressionOperator : public BaseDefinedObject
    {
    public:

        /// @brief The possible operator types
        enum OperatorType
        {
            /// @brief This is an addition operator
            add,
            /// @brief This is a subtraction
            subtract,
            /// @brief This is a division operator
            divide,
            /// @brief This is a multiplication operator
            multiply,
        };

        /// @brief Create an ExpressionOperator with the specfied type
        /// @param type 
        inline ExpressionOperator( OperatorType type ) : type( type ) {}

        /// @brief Access the type of this operator
        /// @return 
        inline OperatorType Type() { return type; }

    private:

        /// @brief The type of this operator
        OperatorType type;
    };
}