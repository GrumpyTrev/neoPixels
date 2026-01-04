#pragma once
#include <string>
#include "LedStrip.hpp"
#include "Tokeniser.hpp"
#include "ObjectParsers.hpp"
#include "Block.hpp"

using namespace std;

namespace Lights
{
	class Commander
	{
	public:
		/// @brief Construct a Commander to drive the specified Led strip
		/// @param strip
		inline Commander(LedStrip *strip) : commandStrip(strip)
		{
			tokens = new Tokeniser();
			parsers = new ObjectParsers(tokens, commandStrip, this);
		}

		/// @brief Start executing built in scripts and command inputs
		void Execute();

		/// @brief Get a command from one of the command input streams
		void GetCommand();

		/// @brief What type of command has been entered
		/// @return
		inline bool CommandAvailable() { return command != NoCommand; }

		/// @brief Schedule the execution of a block
		/// @param executeBlock
		inline void BlockToExecute(Block *executeBlock)
		{
			commandBlock = executeBlock;
			command = ExecuteBlockCommand;
		}

	private:
		/// @brief What type of command has been entered
		enum CommandType
		{
			NoCommand,
			ExecuteBlockCommand
		};

		/// @brief Execute a block until a new block is specified
		void ExecuteBlock();

		/// @brief Parse the command buffer into a command object
		void ParseCommand();

		/// @brief Get a command from the serial stream
		void GetUartCommand();

		/// @brief Get a command from the script
		void GetScriptCommand();

		// The command that has been entered
		CommandType command = NoCommand;

		// The Block to execute
		Block *commandBlock = nullptr;

		/// @brief The Led strip used by this Commander
		LedStrip *commandStrip = nullptr;

		/// @brief The Tokeniser used to break up the command line
		Tokeniser *tokens = nullptr;

		/// @brief The ObjectParsers used to create objects and process commands
		ObjectParsers *parsers = nullptr;
	};
}