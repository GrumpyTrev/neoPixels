#include "ExecutableItem.hpp"
#include <map>
#include <pico/time.h>

using namespace std;

namespace Lights
{
	/// @brief Mapping from SynchType name to type
	map<string, ExecutableItem::SynchType> ExecutableItem::parallelMap{
		{"parallel", parallel},
		{"sequential", sequential},
		{"parallelStart", parallelStart},
		{"parallelEnd", parallelEnd},
		{"parallelBoth", parallelBoth}};

	/// @brief Copy constructor
	/// @param rhs
	ExecutableItem::ExecutableItem(const ExecutableItem &rhs)
	{
		postDelayer = rhs.postDelayer;
		executionTimer = rhs.executionTimer;
		counter = rhs.counter;
		itemSynch = rhs.itemSynch;
		itemSegment = rhs.itemSegment;
		defaultSegment = rhs.defaultSegment;
	}

	/// @brief Initialise the Item prior to execution
	void ExecutableItem::Initialise(SegmentProvider *sequenceToApply)
	{
		if (initialised == false)
		{
			OneOffInitialisation();
			initialised = true;
		}

		// If there is a duration then determine the time that this Action can run until
		if (executionTimer != nullptr)
		{
			timeLimit = delayed_by_ms(get_absolute_time(), executionTimer->Value());
		}

		// If a segment has been supplied by the containing block then save it as the segment for this item to use
		// Otherwise if a segment has been configured for this item use it, otherwise use the default segment
		itemSegment = ( sequenceToApply != nullptr ) ? sequenceToApply :
			( itemSegment != nullptr ) ? itemSegment : defaultSegment;

		// Allow derived classes to initialise themselves
		InitialiseItem();
	}

	/// @brief Return a SynchType given a string value
	/// @param fromString
	/// @param type
	/// @return
	bool ExecutableItem::TypeFromString(string fromString, SynchType &type)
	{
		map<string, ExecutableItem::SynchType>::iterator it = parallelMap.find(fromString);

		if (it != parallelMap.end())
		{
			type = it->second;
		}

		return (it != parallelMap.end());
	}

	string TimeDisplay()
	{
		uint timeMs = to_ms_since_boot(get_absolute_time());
		return to_string(timeMs / 1000) + ':' + to_string(timeMs - (timeMs / 1000) * 1000) + ' ';
	}
}