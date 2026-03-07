#include "ExpressionProvider.hpp"
#include <stack>
#include "ExpressionOperator.hpp"

// TESTING
#include <iostream>

using namespace std;

namespace Lights
{
    /// @brief Calculate the value using the numbers and operands in the expressionVector
    int32_t ExpressionProvider::Value()
    {
        std::stack<int32_t> operandStack;

        bool runtimeError = false;
        int32_t expressionIndex = 0;

        // Default the provided value to 0 in case there is a runtime error
        providedValue = 0;

        while ( ( runtimeError == false ) && ( expressionIndex < expressionVector.size() ) )
        {
            // Process the next item in the expressionVector
            BaseDefinedObject* item = expressionVector[ expressionIndex++ ];

            // If this is a NumberProvicer then just push the number on the stack
            NumberProvider* itemNumber = dynamic_cast<NumberProvider*>( item );
            if ( itemNumber != nullptr )
            {
                int32_t value = itemNumber->Value();
                operandStack.push( value );
                if ( TraceOn() == true )
                {
                    cout << "Calculated expression provider " << Name() << " pushing " << value << "\n";
                }
            }
            else
            {
                // It should be a ExpressionOperator, but check anyway
                ExpressionOperator* itemOperator = dynamic_cast<ExpressionOperator*>( item );
                if ( itemOperator != nullptr )
                {
                    // Pop the top two numbers off the stack and apply the operator
                    if ( operandStack.size() >= 2 )
                    {
                        int32_t operand2 = operandStack.top();
                        operandStack.pop();
                        int32_t operand1 = operandStack.top();
                        operandStack.pop();

                        int32_t result = 0;
                        switch ( itemOperator->Type() )
                        {
                        case ExpressionOperator::add:
                            result = operand1 + operand2;
                            break;

                        case ExpressionOperator::subtract:
                            result = operand1 - operand2;
                            break;

                        case ExpressionOperator::divide:
                            if ( operand2 != 0 )
                            {
                                result = operand1 / operand2;
                            }
                            else
                            {
                                runtimeError = true;
                            }
                            break;

                        case ExpressionOperator::multiply:
                            result = operand1 * operand2;
                            break;
                        }

                        if ( runtimeError == false )
                        {
                            // Push the result back on the stack
                            operandStack.push( result );
                        }
                    }
                    else
                    {
                        runtimeError = true;
                    }
                }
                else
                {
                    runtimeError = true;
                }
            }
        }

        // If there was a runtime error then set the provided value to 0, otherwise pop the result off the stack
        if ( ( runtimeError == false ) && ( operandStack.size() == 1 ) )
        {
            providedValue = operandStack.top();

            if ( TraceOn() == true )
            {
                cout << "Calculated expression provider " << Name() << " value " << providedValue << "\n";
            }
        }

        return providedValue;
    }
}