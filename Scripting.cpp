#include "Scripting.hpp"

namespace Lights
{
	vector<string> Scripting::builtInScript = { "segment fullSegment" };
	vector<string> Scripting::defaultScript =
	{
		"number fadeAmount 10",
		"number twinkleProbability 20",
		"number delay 50",
		"randomNumber twinkleSelector max=twinkleProbability",
		"randomNumber hueSelector",
		"randomNumber valueSelector min=192 max=256",
		"colourHSV hsvProvider hue=hueSelector sat=255 value=valueSelector",
		"fade fader fade=fadeAmount",
		"set setLed colour=hsvProvider whenNot=twinkleSelector",
		"block effect5 delay=delay fader setLed",
		"x effect5",
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