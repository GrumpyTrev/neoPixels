#include "Action.hpp"
#include <pico/time.h>

#include "Trace.hpp"
#include <iostream>

namespace Lights
{
	/// @brief Copy constructor
	/// @param rhs
	Action::Action(const Action &rhs) : ExecutableItem(rhs), BaseDefinedObject(rhs)
	{
		actionColourProvider = rhs.actionColourProvider;
	}

	/// @brief This is called prior to execution of the Action
	void Action::InitialiseItem()
	{
		// If there is a post-operation delay then reset the operation counter
		if (postDelayer != nullptr)
		{
			operationCount = 0;
		}
	}

	/// @brief Execute the operation associated with this Action
	/// @param count
	/// @return
	bool Action::Execute(uint count)
	{
		// Assume no delay, or not run
		uint postDelayValue = 0;

		// Save the step count so it can be accessed by derived classes
		stepCount = count;

		// Determine the possibly dynamic execution count
		uint countLimit = (counter != nullptr) ? counter->Value() : 1;

		// If there is no post-operation delay, then just perform the operation the required number
		// of times
		if (postDelayer == nullptr)
		{
			for (uint executionCount = 0; executionCount < countLimit; ++executionCount)
			{
				Execute();
			}
		}
		else
		{
			// If there is an execution timer and it has not expired then perform the operation
			if (executionTimer != nullptr)
			{
				if (time_reached(timeLimit) == false)
				{
					Execute();
					postDelayValue = postDelayer->Value();
				}
			}
			else
			{
				// If the operationCount has not reached its limit then perform the operation
				if (operationCount++ < countLimit)
				{
					Execute();
					postDelayValue = postDelayer->Value();
				}
			}
		}

		// Set the next time this Action should be executed
		if (postDelayValue > 0)
		{
			delayTime = make_timeout_time_ms(postDelayValue);
		}

		if (Trace::TraceOn() == true)
		{
			cout << TimeDisplay() << "Action " << Name() << " Op count " << operationCount << " post delay " << postDelayValue << "\n";
		}
		return postDelayValue > 0;
	}
}