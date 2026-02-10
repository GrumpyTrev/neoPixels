#pragma once
#include <sstream>

/// @brief Replace any %% by a single %. Any single % will already have been processed before getting
/// here.
/// @param format 
/// @return 
inline string StringFormat( const char* format )
{
    string replacementString = "";

    char currentCharacter = *format++;
    while ( currentCharacter != 0 )
    {
        if ( currentCharacter == '%' )
        {
            // The next char must be a '%' so just step the pointer on
            format++;
        }

        replacementString += currentCharacter;
        currentCharacter = *format++;
    }

    return replacementString;
}

/// @brief Output the first of a variable number of arguments to the output string
/// and recursively process the reset of the arguements
/// @tparam T The type of the first arguemnt
/// @tparam ...Args The type of the rest of the arguements
/// @param format The format string containing % characters where the arguments should be inserted
/// @param value The first of the variable arguments
/// @param ...args The rest of the arguments
/// @return 
template<typename T, typename... Args>
inline string StringFormat( const char* format, T value, Args... args )
{
    std::stringstream buffer;

    bool stringFormatted = false;
    char currentCharacter = *format++;
    while ( ( currentCharacter != 0 ) && ( stringFormatted == false ))
    {
        if ( currentCharacter == '%' )
        {
            // If the next char is also a % then just step on
            if ( *format == '%' )
            {
                format++;
            }
            else
            {
                // Put the first of the variable number of args into the stream and recursively process the
                // other arguments
                buffer << value << StringFormat( format, args... );
                stringFormatted = true;
            }
        }

        if ( stringFormatted == false )
        {
            buffer << currentCharacter;
            currentCharacter = *format++;
        }
    }

    return buffer.str();
}

