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
	bool Tokeniser::NextInt( int32_t& value )
	{
		bool intFound = false;
		if (TokensLeft() >= 1)
		{
			try
			{
				value = (int32_t)stoi( Next(), nullptr, 0 );
				intFound = true;
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

		return intFound;
	}
}