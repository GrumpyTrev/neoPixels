#include "Scripting.hpp"

using namespace std;

namespace Lights
{
	vector<string> Scripting::builtInScript = {"segment fullSegment", "#t on"};
	vector<string> Scripting::defaultScript =
	{
		"trigger cycleTrigger",
		"randomNumber ledSelector max=50 next=cycleTrigger",
		"randomNumber hueSelector next=cycleTrigger",
		"randomNumber valueSelector min=192 max=256 next=cycleTrigger",
		"randomNumber twinkleNumber min=1 max=4 next=cycleTrigger",
		"colourHSV hsvProvider hue=hueSelector sat=255 value=valueSelector next=cycleTrigger",
		"fade fadeBy10 fadeBy=90",
		"set setLed colour=hsvProvider startLed=ledSelector",
		"block setter count=twinkleNumber cycleTrigger setLed",
		"block effect5 delay=50 fadeBy10 setter",
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