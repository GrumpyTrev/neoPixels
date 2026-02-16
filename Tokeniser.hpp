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
		bool NextUint( uint16_t& value );

		/// @brief The current token being processed
		/// @return
		inline string Current() { return tokenIndex > 0 ? tokens[tokenIndex - 1] : ""; }

		/// @brief Flag set to true if an error has been found during the processing of the command line
		inline bool Error() { return error; };

	private:
		/// @brief The actual command line
		string inputLine;

		/// @brief The command line split into seperate string
		vector<string> tokens;

		/// @brief The index of the next token to be returned
		uint16_t tokenIndex;

		/// @brief Flag set to true if an error has been found during the processing of the command line
		bool error = false;
	};
}