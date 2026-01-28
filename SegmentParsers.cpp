#include "ObjectParsers.hpp"
#include "IntervalSegment.hpp"
#include "DiscreteSegment.hpp"

using namespace std;

namespace Lights
{
	/// @brief Make a Segment from the ParameterStorage
	/// @return
	void ObjectParsers::MakeSegment()
	{
		// Make sure that if a start led has been specified then a count has also been specified
		if ((storage.StartLedProvider != nullptr) && (storage.CountProvider == nullptr))
		{
			errorStream << "Invalid number of Leds for segment " << storage.Name;
		}
		else
		{
			// Get the start led, count and reverse flag
			uint16_t startIndex = GetStoredNumber( 0, storage.StartLedProvider );
			uint16_t numLeds = GetStoredNumber( commandStrip->NumLeds(), storage.CountProvider );
			bool reverse = GetStoredBoolean(false, storage.ReverseProvider);

			Segment *sequence = nullptr;
			if (reverse == true)
			{
				sequence = new IntervalSegment(commandStrip, startIndex, 1, startIndex + numLeds - 1, true);
			}
			else
			{
				sequence = new Segment(commandStrip, startIndex, numLeds);
			}

			StoreObject(new SegmentProvider(sequence), "Segment");
		}
	}

	/// @brief Make an IntervalSegment from the ParameterStorage
	/// @return
	void ObjectParsers::MakeIntervalSegment()
	{
		// Make sure the start and interval have been specified.
		if ((storage.StartLedProvider != nullptr) && (storage.IntervalProvider != nullptr))
		{
			// Get the start, interval, number of Leds and the reverse flag
			uint16_t startIndex = GetStoredNumber( 0, storage.StartLedProvider );
			uint16_t interval = GetStoredNumber( 0, storage.IntervalProvider );
			uint16_t maxLed = GetStoredNumber( commandStrip->NumLeds() - 1, storage.CountProvider );
			bool reverse = GetStoredBoolean(false, storage.ReverseProvider);

			StoreObject(new SegmentProvider(
							new IntervalSegment(commandStrip, startIndex, interval, maxLed, reverse)),
						"IntervalSegment");
		}
		else
		{
			errorStream << "Start offset or interval not specified for segment " << storage.Name;
		}
	}

	/// @brief Make an DiscreteSegment from the ParameterStorage
	/// @return
	void ObjectParsers::MakeDiscreteSegment()
	{
		// There should be at least one pixel number
		if (storage.Numbers.size() > 0)
		{
			StoreObject(new SegmentProvider(new DiscreteSegment(commandStrip, storage.Numbers)),
						"DiscreteSegment");
		}
		else
		{
			errorStream << "No pixels specified for segment " << storage.Name;
		}
	}
}