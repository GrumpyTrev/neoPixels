#include "Commander.hpp"
#include "ObjectParsers.hpp"
#include "ObjectStore.hpp"
#include "Scripting.hpp"
#include <pico/error.h>
#include <pico/stdio.h>
#include <iostream>

using namespace std;

namespace Lights
{
	/// @brief Run the specified display. If no display is specified then just run a loop getting a command
	/// @param defaultDisplay
	void Commander::Execute()
	{
		command = NoCommand;

		while (true)
		{
			if (command == ExecuteBlockCommand)
			{
				command = NoCommand;
				ExecuteBlock();
			}
			while (command == NoCommand)
			{
				GetCommand();
			}
		}
	}

	/// @brief Execute the specified Block
	void Commander::ExecuteBlock()
	{
		uint executionCount = 0;

		// Make sure that the Block runs straight away
		absolute_time_t delayTime = get_absolute_time();

		// Continue execution until another Block has been choosen to execute
		while (CommandAvailable() == false)
		{
			// Is it time to run the Block
			if (time_reached(delayTime) == true)
			{
				// It is time to run the Block. If this is the first time then initialise it first
				if (executionCount == 0)
				{
					commandBlock->Initialise(nullptr);
				}

				// Run the Block. The Block will return a post operation delay
				if (commandBlock->Execute(executionCount++) > 0)
				{
					// Update the delay time
					delayTime = commandBlock->DelayTime();

					// Refresh the LEDs
					commandStrip->Show();
				}
				else
				{
					// A zero post-operation delay is interpreted as a re-initialise request
					executionCount = 0;
					delayTime = get_absolute_time();

					// To prevent this locking out the input system, check for a command
					GetCommand();
				}
			}
			else
			{
				// Check for a command
				GetCommand();
			}
		}
	}

	/// @brief Get character either from the Scripting class or the Uart
	void Commander::GetCommand()
	{
		if (Scripting::IsActive() == true)
		{
			GetScriptCommand();
		}
		else
		{
			GetUartCommand();
		}
	}

	/// @brief Get a character from the Uart and store it in the Tokeniser.
	/// If is a carriage return then process the command.
	void Commander::GetUartCommand()
	{
		int input = PICO_ERROR_TIMEOUT;
		do
		{
			// Get a single char from the serial port
			input = getchar_timeout_us(0);

			if (input != PICO_ERROR_TIMEOUT)
			{
				// Add this character to the command buffer. If it is a CR then attempt to parse the command
				if (input == '\r')
				{
//					putchar('\n');
					ParseCommand();
				}
				else
				{
					// Add received char to the command buffer
					tokens->AddChar((char)input);
//					putchar(input);
				}
			}

		} while (input != PICO_ERROR_TIMEOUT);
	}

	/// @brief Get a line from the Scipting class. Echo it and process it.
	void Commander::GetScriptCommand()
	{
		string line = Scripting::GetLine();
		if (line.size() > 0)
		{
			puts(line.c_str());
			putchar('\n');

			tokens->Line(line);
			ParseCommand();
		}
	}

	/// @brief Parse the current command buffer into a command object
	void Commander::ParseCommand()
	{
		command = NoCommand;

		ostringstream error;

		tokens->Tokenise();

		// Make sure there's at least one token
		if (tokens->TokensLeft() > 0)
		{
			string commandToken = tokens->Next();

			// Ignore any comment lines
			if (commandToken[0] != '#')
			{
				// See if any classes have registered for the command
				if (parsers->ParseDefinition(commandToken) == true)
				{
					error << parsers->ErrorString();
				}
				else
				{
					error << "No such command: " << commandToken;
				}
			}
		}

		if (error.tellp() > 0)
		{
			cout << error.str() << '\n';
		}
	}
}