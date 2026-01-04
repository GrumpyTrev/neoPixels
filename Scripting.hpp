#pragma once

#include <string>
#include <deque>
#include <vector>

using namespace std;

namespace Lights
{
	/// @brief The Scripting contains scripts providing built in definitions and optionally a default
	/// block to execute
	class Scripting
	{
	public:
		/// @brief Get a line from the script
		/// @return
		static string GetLine();

		static bool IsActive() { return scriptsRead == false; }

	private:
		/// @brief Built in definitions
		static vector<string> builtInScript;

		/// @brief Default block
		static vector<string> defaultScript;

		/// @brief The combimned script being read from
		static deque<string> activeScript;

		/// @brief Has the script been read
		static bool scriptsRead;
	};
}