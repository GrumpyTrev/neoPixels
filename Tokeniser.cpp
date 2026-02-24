#include "Tokeniser.hpp"
#include <sstream>

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
}