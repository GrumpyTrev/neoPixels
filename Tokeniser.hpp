#pragma once

#include "pico/stdlib.h"
#include <string>
#include <vector>

using namespace std;

namespace Lights
{
	/// @brief Simple command line tokeniser
	class Tokeniser
	{
	public:
		/// @brief Add a character to the command line
		/// @param inputChar
		inline void AddChar(char inputChar) { inputLine += inputChar; }

		/// @brief Assign the inputline to a string
		/// @param inputChar
		inline void Line(string input) { inputLine = input; }

		/// @brief Separate the command line into space delimited strings and put onto the token stack
		void Tokenise();

		/// @brief Return the next string from the command line. Set the error flag if no more strings available
		/// @return
		string Next();

		/// @brief How many tokens are left in the command line
		/// @return
		inline uint TokensLeft() { return tokens.size() - tokenIndex; }

		/// @brief Get the next unsigned integer from the command line.
		/// Set the error flag if there are no more tokens or if the token is not a valid integer
		/// @param value Reference to uint that value should be returned in
		/// @return Returns true if a valid integer was returned in the reference parameter
		bool NextUint(uint &value);

		/// @brief Get the next unsigned integer from the command line.
		/// Set the error flag if there are no more tokens or if the token is not a valid integer
		/// @return Returns the integer, or 0 if no integer read
		uint NextUint();

		/// @brief Attempt to get the next unsigned integer from the command line.
		/// @param value Reference to uint that value should be returned in
		/// @return Returns true if a valid integer was returned in the reference parameter
		bool TryNextUint(uint &value);

		/// @brief Attempt to get a uint parameter from the command line
		/// The parameter is expected to be of the form <parameterI><uint>, i.e "count=10"
		/// @param parameterId The parameter identity prefix
		/// @param value The returned uint
		/// @return True if the prefix and integer were found
		bool TryUintParameter(string parameterId, uint &value);

		/// @brief Attempt to get a boolean parameter from the command line
		/// The parameter is expected to be of the form <parameterI><bool string>, i.e "count=true"
		/// @param parameterId The parameter identity prefix
		/// @param value The returned bool
		/// @return True if the prefix and boolean string were found
		bool TryBoolParameter(string parameterId, bool &value);

		/// @brief Attempt to get a string parameter from the command line
		/// The parameter is expected to be of the form <parameterI><string>, i.e "sequence=s2"
		/// @param parameterId The parameter identity prefix
		/// @param value The returned string
		/// @return True if the prefix and string were found
		bool TryStringParameter(string parameterId, string &value);

		/// @brief The current token being processed
		/// @return
		inline string Current() { return tokenIndex > 0 ? tokens[tokenIndex - 1] : ""; }

		/// @brief Flag set to true if an error has been found during the processing of the command line
		inline bool Error() { return error; };

		/// @brief Go back one token if valid
		inline void PushBack()
		{
			if (tokenIndex > 0)
			{
				tokenIndex--;
			}
		}

	private:
		/// @brief Match a string against the start of the next token. If the string is found then
		/// Return the rest of the string in the remainingString parameter
		/// @param stringToMatch The string to match
		/// @param remainingString The rest of the token once the matching string is removed
		/// @return True if the string was matched
		bool MatchString(string stringToMatch, string &remainingString);

		/// @brief The actual command line
		string inputLine;

		/// @brief The command line split into seperate string
		vector<string> tokens;

		/// @brief The index of the next token to be returned
		uint tokenIndex;

		/// @brief Flag set to true if an error has been found during the processing of the command line
		bool error = false;
	};
}