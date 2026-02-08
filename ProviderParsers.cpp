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
			errorStream << "Invalid colours for ColourSequenceProvider " << storage.Name;
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
		if ((storage.FadeStartColourProvider == nullptr) || (storage.FadeEndColourProvider == nullptr) ||
			(storage.IntervalProvider == nullptr))
		{
			errorStream << "Parameter missing for ColourFadeProvider " << storage.Name;
		}
		else
		{
			StoreObject( ApplyCommonProviderParameters(
				new ColourFadeProvider( (ColourProvider*)( storage.FadeStartColourProvider ),
					(ColourProvider*)( storage.FadeEndColourProvider ),
					GetStoredNumber( 0, storage.IntervalProvider ) ) ),
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
			errorStream << "Not enough numbers for NumberProvider " << storage.Name;
		}
	}

	/// @brief Create and store a RandomNumberProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeRandomNumberProvider()
	{
		StoreObject( ApplyCommonProviderParameters( new RandomNumberProvider( GetStoredNumber( 0, storage.MinRangeProvider ),
			GetStoredNumber( (uint16_t)65535, storage.MaxRangeProvider ) ) ),
			"RandomNumberProvider" );
	}

	/// @brief Create and store a RandomNumberSetProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeRandomNumberSetProvider()
	{
		StoreObject( ApplyCommonProviderParameters( new RandomNumberSetProvider( GetStoredNumber( 0, storage.MinRangeProvider ),
			GetStoredNumber( (uint16_t)65535, storage.MaxRangeProvider ) ) ),
			"RandomNumberSetProvider" );
	}
	
	/// @brief Create and store a NumberIntervalProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberIntervalProvider()
	{
		// If the minimum or maximum are not specified then make number providers for them
		NumberProvider* minProvider = ( storage.MinRangeProvider != nullptr ) ?
			(NumberProvider*)storage.MinRangeProvider : new NumberProvider( 0 );
		NumberProvider* maxProvider = ( storage.MaxRangeProvider != nullptr ) ?
			(NumberProvider*)storage.MaxRangeProvider : new NumberProvider( uint16_t( 65535 ) );

		// The step is optional
		uint16_t step = GetStoredNumber( 1, storage.IntervalProvider );

		// The reverse is optional
		bool reverse = GetStoredBoolean( false, storage.ReverseProvider );

		StoreObject( ApplyCommonProviderParameters(
			new NumberIntervalProvider( minProvider, maxProvider, step, reverse ) ), "NumberIntervalProvider" );
	}

	/// @brief Create and store a ColourHSVProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeColourHSVProvider()
	{
		if ((storage.HueProvider == nullptr) || (storage.SatProvider == nullptr) || ( storage.ValueProvider == nullptr))
		{
			errorStream << "Missing hue, sat or val provider for " << storage.Name;
		}
		else
		{
			StoreObject( ApplyCommonProviderParameters(
				new ColourHSVProvider( (NumberProvider*)storage.HueProvider, (NumberProvider*)storage.SatProvider,
					(NumberProvider*)storage.ValueProvider, GetStoredNumber( 0, storage.IntervalProvider ) ) ),
				"ColourHSVProvider" );
		}
	}

	/// @brief Create and store a NumberSineProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberSineProvider()
	{
		// The step is optional
		uint16_t step = GetStoredNumber( 1, storage.IntervalProvider );

		StoreObject( ApplyCommonProviderParameters( new NumberSineProvider( step ) ), "NumberSineProvider" );
	}

	/// @brief Make a SegmentSequenceProvider from the ParameterStorage
	/// @param tokens
	/// @return
	void ObjectParsers::MakeSegmentSequence()
	{
		// There should be at least one segmemnt provider
		if ((storage.Objects.size() > 0) &&
			(storage.CheckObjectsType<SegmentProvider>() == true))
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
			errorStream << "Either no providers or invalid providers for CycleAction " << storage.Name;
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