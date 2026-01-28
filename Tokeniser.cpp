#include "Tokeniser.hpp"
#include <sstream>

using namespace std;

namespace Lights
{
	/// @brief Separate the command line into space delimited strings and put onto the token stack
	void Tokeniser::Tokenise()
	{
		// Clear the tokens
		tokens.clear();
		tokenIndex = 0;
		error = false;

		// Separate into strings using a stream
		stringstream commandStream(inputLine);
		string token;

		while (getline(commandStream, token, ' '))
		{
			tokens.push_back(token);
		}

		// Clear the inputLine for next time
		inputLine.clear();
	}

	/// @brief Return the next string from the command line. Set the error flag if no more strings available
	/// @return
	string Tokeniser::Next()
	{
		string token = "";
		if (tokenIndex < tokens.size())
		{
			token = tokens[tokenIndex++];
		}
		else
		{
			error = true;
		}

		return token;
	}

	/// @brief Get the next unsigned integer from the command line.
	/// Set the error flag if there are no more tokens or if the token is not a valid integer
	/// @param value Reference to uint that value should be returned in
	/// @return Returns true if a valid integer was returned in the reference parameter
	bool Tokeniser::NextUint( uint16_t& value )
	{
		bool uintFound = false;
		if (TokensLeft() >= 1)
		{
			try
			{
				value = (uint16_t)stoi( Next(), nullptr, 0 );
				uintFound = true;
			}
			catch (invalid_argument const &ex)
			{
				error = true;
			}
		}
		else
		{
			error = true;
		}

		return uintFound;
	}

	/// @brief Get the next unsigned integer from the command line.
	/// Set the error flag if there are no more tokens or if the token is not a valid integer
	/// @return Returns the integer, or 0 if no integer read
	uint16_t Tokeniser::NextUint()
	{
		uint16_t value = 0;
		return (NextUint(value) == true) ? value : 0;
	}

	/// @brief Attempt to get the next unsigned integer from the command line.
	/// @param value Reference to uint that value should be returned in
	/// @return Returns true if a valid integer was returned in the reference parameter
	bool Tokeniser::TryNextUint( uint16_t& value )
	{
		bool uintFound = false;
		if (TokensLeft() >= 1)
		{
			try
			{
				value = (uint16_t)stoi( Next(), nullptr, 0 );
				uintFound = true;
			}
			catch (invalid_argument const &ex)
			{
				tokenIndex--;
			}
		}

		return uintFound;
	}

	/// @brief Attempt to get a uint parameter from the command line
	/// The parameter is expected to be of the form <parameterI><uint>, i.e "count=10"
	/// @param parameterId The parameter identity prefix
	/// @param value The returned uint
	/// @return True if the prefix and integer were found
	bool Tokeniser::TryUintParameter( string parameterId, uint16_t& value )
	{
		bool uintParameterFound = false;

		string valueString;
		if (MatchString(parameterId, valueString) == true)
		{
			try
			{
				value = (uint16_t)stoi( valueString, nullptr, 0 );
				uintParameterFound = true;
			}
			catch (invalid_argument const &ex)
			{
				--tokenIndex;
			}
		}

		return uintParameterFound;
	}

	/// @brief Attempt to get a boolean parameter from the command line
	/// The parameter is expected to be of the form <parameterI><bool string>, i.e "count=true"
	/// @param parameterId The parameter identity prefix
	/// @param value The returned bool
	/// @return True if the prefix and boolean string were found
	bool Tokeniser::TryBoolParameter(string parameterId, bool &value)
	{
		bool boolParameterFound = false;

		string valueString;
		if (MatchString(parameterId, valueString) == true)
		{
			value = valueString == "true" ? true : false;
			boolParameterFound = true;
		}

		return boolParameterFound;
	}

	/// @brief Attempt to get a string parameter from the command line
	/// The parameter is expected to be of the form <parameterI><string>, i.e "sequence=s2"
	/// @param parameterId The parameter identity prefix
	/// @param value The returned string
	/// @return True if the prefix and string were found
	bool Tokeniser::TryStringParameter(string parameterId, string &value)
	{
		return MatchString(parameterId, value);
	}

	/// @brief Match a string against the start of the next token. If the string is found then
	/// Return the rest of the string in the remainingString parameter
	/// @param stringToMatch The string to match
	/// @param remainingString The rest of the token once the matching string is removed
	/// @return True if the string was matched
	bool Tokeniser::MatchString(string stringToMatch, string &remainingString)
	{
		bool matched = false;

		if (TokensLeft() >= 1)
		{
			string stringToMatchAgainst = Next();
			if (stringToMatchAgainst.find(stringToMatch) == 0)
			{
				remainingString = stringToMatchAgainst.substr(stringToMatch.length());
				matched = true;
			}
			else
			{
				tokenIndex--;
			}
		}
		return matched;
	}
}