#include "Scripting.hpp"

using namespace std;

namespace Lights
{
	vector<string> Scripting::builtInScript = {"segment fullSegment", "#t on"};
	vector<string> Scripting::defaultScript =
		{
			"colourSequence provider White Green",
			"fill fillColour type=sequential delay=1000 colour=provider",
			"fill fillBlack type=sequential delay=1000 colour=Black",
			"cycle cycler type=parallel provider",
			"block fillBlock1 count=5 segment=fullSegment cycler fillColour fillBlack",
			"x fillBlock1",
	};

	deque<string> Scripting::activeScript;

	bool Scripting::scriptsRead = false;

	string Scripting::GetLine()
	{
		string line = "";

		if (scriptsRead == false)
		{
			if (activeScript.size() == 0)
			{
				// Load the activeScript with the combined builtInScript and defaultScript
				activeScript.assign(builtInScript.begin(), builtInScript.end());
				activeScript.insert(activeScript.end(), defaultScript.begin(), defaultScript.end());
			}

			if (activeScript.size() > 0)
			{
				line = activeScript.front();
				activeScript.pop_front();
			}

			scriptsRead = (activeScript.size() == 0);
		}

		return line;
	}
}