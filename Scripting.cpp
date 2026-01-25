#include "Scripting.hpp"

using namespace std;

namespace Lights
{
	vector<string> Scripting::builtInScript = {"segment fullSegment", "#t on"};
	vector<string> Scripting::defaultScript =
	{
		"fade fadeBy10 fadeBy=90",
		"randomNumber ledSelector minimum=0 maximum=50",
		"randomNumber hueSelector minimum=0 maximum=65535",
		"randomNumber valueSelector minimum=192 maximum=256",
		"colourHSV hsvProvider hue=hueSelector sat=255 value=valueSelector",
		"set setLed colour=hsvProvider startLed=ledSelector",
		"cycle colourCycler hueSelector valueSelector hsvProvider ledSelector",
		"block fader fadeBy10",
		"block setter count=2 colourCycler setLed",
		"block effect5 delay=50 fader setter",
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