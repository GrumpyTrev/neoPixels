#include <iostream>
#include "ObjectParsers.hpp"
#include "ObjectStore.hpp"
#include "ColourSequenceProvider.hpp"
#include "NumberProvider.hpp"
#include "ColourWheelProvider.hpp"
#include "ColourFadeProvider.hpp"
#include "NumberSequenceProvider.hpp"
#include "RandomNumberProvider.hpp"
#include "RandomNumberSetProvider.hpp"
#include "SegmentSequenceProvider.hpp"
#include "NumberIntervalProvider.hpp"
#include "ColourHSVProvider.hpp"
#include "NumberSineProvider.hpp"
#include "TriggerAction.hpp"
#include "StringFormatter.hpp"

using namespace std;

namespace Lights
{
	/// @brief Make a ColourSequenceProvider with one or more Colours
	/// @return
	void ObjectParsers::MakeColourSequenceProvider()
	{
		// Make sure that all the stored objects are ColourProviders, and there is at least one of them
		if ((storage.CheckObjectsType<ColourProvider>() == true) && (storage.Objects.size() > 0))
		{
			ColourSequenceProvider *definedProvider = new ColourSequenceProvider();
			for (BaseDefinedObject *object : storage.Objects)
			{
				definedProvider->AddValue(((ColourProvider *)(object))->Value());
			}

			StoreObject( ApplyCommonProviderParameters( definedProvider ), "ColourSequenceProvider" );
		}
		else
		{
			ReportError( "Invalid colours for ColourSequenceProvider %", storage.Name );
		}
	}

	/// @brief Create a ColourWheelProvider with a specified name
	/// @return
	void ObjectParsers::MakeColourWheelProvider()
	{
		// Just the provider name is required. This has already been checked
		StoreObject( ApplyCommonProviderParameters( new ColourWheelProvider() ), "ColourWheelProvider" );
	}

	/// @brief Create a ColourFadeProvider based upon two colour references and a step count
	/// @param tokens
	/// @return
	void ObjectParsers::MakeColourFadeProvider()
	{
		// There should be two colour references and a step count.
		if ( ( storage.StartColour == nullptr ) || ( storage.EndColour == nullptr ) ||
			( storage.Interval == nullptr ) )
		{
			ReportError( "Parameter missing for ColourFadeProvider %", storage.Name );
		}
		else
		{
			StoreObject( ApplyCommonProviderParameters(
				new ColourFadeProvider( (ColourProvider*)( storage.StartColour ),
					(ColourProvider*)( storage.EndColour ),
					GetStoredNumber( 0, storage.Interval ) ) ),
				"ColourFadeProvider" );
		}
	};

	/// @brief Make a NumberProvider with the specified name and value(s)
	void ObjectParsers::MakeNumberProvider()
	{
		// This is going to be a NumberProvider or a NumberSequenceProvider depending on the
		// number of numbers provided
		if (storage.Numbers.size() > 0)
		{
			if (storage.Numbers.size() == 1)
			{
				NumberProvider* provider = new NumberProvider( storage.Numbers.at( 0 ) );
				StoreObject( ApplyCommonProviderParameters( provider ), "NumberProvider" );
			}
			else
			{
				NumberSequenceProvider* provider = new NumberSequenceProvider();
				for ( uint16_t number : storage.Numbers )
				{
					provider->AddValue(number);
				}
				StoreObject( ApplyCommonProviderParameters( provider ), "NumberSequenceProvider" );
			}
		}
		else
		{
			ReportError( "Not enough numbers for NumberProvider %", storage.Name );
		}
	}

	/// @brief Create and store a RandomNumberProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeRandomNumberProvider()
	{
		StoreObject( ApplyCommonProviderParameters( new RandomNumberProvider( GetStoredNumber( 0, storage.Min ),
			GetStoredNumber( (uint16_t)65535, storage.Max ) ) ),
			"RandomNumberProvider" );
	}

	/// @brief Create and store a RandomNumberSetProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeRandomNumberSetProvider()
	{
		StoreObject( ApplyCommonProviderParameters( new RandomNumberSetProvider( GetStoredNumber( 0, storage.Min ),
			GetStoredNumber( (uint16_t)65535, storage.Max ) ) ),
			"RandomNumberSetProvider" );
	}
	
	/// @brief Create and store a NumberIntervalProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberIntervalProvider()
	{
		// If the minimum or maximum are not specified then make number providers for them
		NumberProvider* minProvider = ( storage.Min != nullptr ) ?
			(NumberProvider*)storage.Min : new NumberProvider( 0 );
		NumberProvider* maxProvider = ( storage.Max != nullptr ) ?
			(NumberProvider*)storage.Max : new NumberProvider( uint16_t( 65535 ) );

		// The step is optional
		uint16_t step = GetStoredNumber( 1, storage.Interval );

		// The reverse is optional
		bool reverse = GetStoredBoolean( false, storage.ReverseFlag );

		StoreObject( ApplyCommonProviderParameters(
			new NumberIntervalProvider( minProvider, maxProvider, step, reverse ) ), "NumberIntervalProvider" );
	}

	/// @brief Create and store a ColourHSVProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeColourHSVProvider()
	{
		if ( ( storage.Hue == nullptr ) || ( storage.Sat == nullptr ) || ( storage.Value == nullptr ) )
		{
			ReportError( "Missing hue, sat or val provider for %", storage.Name );
		}
		else
		{
			StoreObject( ApplyCommonProviderParameters(
				new ColourHSVProvider( (NumberProvider*)storage.Hue, (NumberProvider*)storage.Sat,
					(NumberProvider*)storage.Value, GetStoredNumber( 0, storage.Interval ) ) ),
				"ColourHSVProvider" );
		}
	}

	/// @brief Create and store a NumberSineProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberSineProvider()
	{
		StoreObject( ApplyCommonProviderParameters(
			new NumberSineProvider( GetStoredNumber( 1, storage.Interval ), (NumberProvider*)storage.Init ) ),
			"NumberSineProvider" );
	}

	/// @brief Make a SegmentSequenceProvider from the ParameterStorage
	/// @param tokens
	/// @return
	void ObjectParsers::MakeSegmentSequence()
	{
		// There should be at least one segment provider and they should all be SegmentProviders
		if ( ( storage.Objects.size() > 0 ) && ( storage.CheckObjectsType<SegmentProvider>() == true ) )
		{
			SegmentSequenceProvider *definedObject = new SegmentSequenceProvider();

			for (BaseDefinedObject *object : storage.Objects)
			{
				definedObject->AddValue(((SegmentProvider *)(object))->Value());
			}

			StoreObject( ApplyCommonProviderParameters( definedObject ), "SegmentSequenceProvider" );
		}
		else
		{
			ReportError( "Either no providers or invalid providers for SegmentProvider %", storage.Name );
		}
	}

	/// @brief Apply the extracted parameters to a Provider
	/// @param error
	/// @return
	ProviderBase* ObjectParsers::ApplyCommonProviderParameters( ProviderBase* provider )
	{
		if ( storage.NextTrigger != nullptr )
		{
			( (TriggerAction*)storage.NextTrigger )->AddCallback(
				new Callback<ProviderBase>( provider, &ProviderBase::Next ) );

			// If there is a Next trigger then clear SelfIncrement
			provider->SelfIncrement( false );
		}

		if ( storage.ResetTrigger != nullptr )
		{
			( (TriggerAction*)storage.ResetTrigger )->AddCallback(
				new Callback<ProviderBase>( provider, &ProviderBase::Reset ) );
		}

		return provider;
	}
}