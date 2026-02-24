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
		// Get the start LED number, number of LEDs and reverse flag
		int32_t startLed = GetStoredNumber( 0, storage.Led );
		int32_t numLeds = GetStoredNumber( commandStrip->NumLeds(), storage.Count );
		bool reverse = GetStoredBoolean( false, storage.ReverseFlag );

		// Make sure the numbers make sense
		if ( ( startLed < 0 ) || ( numLeds <= 0 ) || ( ( startLed + numLeds ) > commandStrip->NumLeds() ) )
		{
			ReportError( "Start LED number or number of LEDs invalid for segment %", storage.Name );
		}
		else
		{
			Segment* segment = ( reverse == true )
				? new IntervalSegment( commandStrip, startLed, 1, startLed + numLeds - 1, true )
				: new Segment( commandStrip, startLed, numLeds );

			StoreObject( new SegmentProvider( segment ), "Segment" );
		}
	}

	/// @brief Make an IntervalSegment from the ParameterStorage
	/// @return
	void ObjectParsers::MakeIntervalSegment()
	{
		// Get the start, interval, number of Leds and the reverse flag
		int32_t startLed = GetStoredNumber( 0, storage.Led );
		int32_t interval = GetStoredNumber( 0, storage.Interval );
		int32_t maxLed = GetStoredNumber( commandStrip->NumLeds() - 1, storage.Count );
		bool reverse = GetStoredBoolean( false, storage.ReverseFlag );

		// Make sure the numbers make sense
		if ( ( startLed < 0 ) || ( maxLed < 0 ) || ( ( startLed + maxLed ) > commandStrip->NumLeds() ) ||
			( maxLed < startLed ) || ( interval < 1 ) )
		{
			ReportError( "Start LED number, interval or maximum LED invalid for segment %", storage.Name );
		}
		else
		{
			StoreObject( new SegmentProvider( new IntervalSegment( commandStrip, startLed, interval, maxLed, reverse ) ),
				"IntervalSegment" );
		}
	}

	/// @brief Make an DiscreteSegment from the ParameterStorage
	/// @return
	void ObjectParsers::MakeDiscreteSegment()
	{
		// There should be at least one LED number
		if ( storage.CheckObjectsType<NumberProvider>() == true )
		{
			vector<uint16_t> leds;
			for ( BaseDefinedObject* provider : storage.Objects )
			{
				leds.push_back( static_cast<NumberProvider*>( provider )->Value() );
			}
			StoreObject( new SegmentProvider( new DiscreteSegment( commandStrip, leds ) ), "DiscreteSegment" );
		}
		else
		{
			ReportError( "No LEDs specified for segment %", storage.Name );
		}
	}
}