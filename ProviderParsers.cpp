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
				new ColourFadeProvider( (ColourProvider*)( storage.StartColour ), (ColourProvider*)( storage.EndColour ),
					GetStoredNumber( 0, storage.Interval ) ) ), "ColourFadeProvider" );
		}
	};

	/// @brief Make a NumberProvider with the specified name and value(s)
	void ObjectParsers::MakeNumberProvider()
	{
		// This is going to be a NumberProvider or a NumberSequenceProvider depending on the
		// number of numbers provided
		if ( ( storage.Objects.size() > 0 ) && ( storage.CheckObjectsType<NumberProvider>() == true ) )
		{
			if ( storage.Objects.size() == 1 )
			{
				NumberProvider* provider = new NumberProvider( GetStoredNumber( 0 ) );
				StoreObject( ApplyCommonProviderParameters( provider ), "NumberProvider" );
			}
			else
			{
				NumberSequenceProvider* provider = new NumberSequenceProvider();
				for ( uint32_t index = 0; index < storage.Objects.size(); index++ )
				{
					provider->AddValue( GetStoredNumber( index ) );
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
		int32_t minValue = GetStoredNumber( 0, storage.Min );
		int32_t maxValue = GetStoredNumber( 65535, storage.Max );

		if ( ( minValue >= 0 ) && ( maxValue <= 65535 ) && ( maxValue >= minValue ) )
		{
			StoreObject( ApplyCommonProviderParameters( new RandomNumberProvider( minValue, maxValue ) ),
				"RandomNumberProvider" );
		}
		else
		{
			ReportError( "Invalid min or max values for RandomNumberProvider %", storage.Name );
		}
	}

	/// @brief Create and store a RandomNumberSetProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeRandomNumberSetProvider()
	{
		int32_t minValue = GetStoredNumber( 0, storage.Min );
		int32_t maxValue = GetStoredNumber( 65535, storage.Max );

		if ( ( minValue >= 0 ) && ( maxValue <= 65535 ) && ( maxValue >= minValue ) )
		{
			StoreObject( ApplyCommonProviderParameters( new RandomNumberSetProvider( minValue, maxValue ) ),
				"RandomNumberProvider" );
		}
		else
		{
			ReportError( "Invalid min or max values for RandomNumberSetProvider %", storage.Name );
		}
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
			(NumberProvider*)storage.Max : new NumberProvider( 65535 );

		StoreObject( ApplyCommonProviderParameters(
			new NumberIntervalProvider( minProvider, maxProvider, GetStoredNumber( 1, storage.Interval ),
				GetStoredBoolean( false, storage.ReverseFlag ) ) ), "NumberIntervalProvider" );
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
					(NumberProvider*)storage.Value, GetStoredNumber( 0, storage.Interval ) ) ), "ColourHSVProvider" );
		}
	}

	/// @brief Create and store a NumberSineProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberSineProvider()
	{
		// Get the optional waveLength ( stored in Interval ), and check it is in range
		int32_t waveLength = GetStoredNumber( 256, storage.Interval );
		if ( ( waveLength > 0 ) && ( waveLength <= 256 ) )
		{
			StoreObject( ApplyCommonProviderParameters(
				new NumberSineProvider( waveLength, (NumberProvider*)storage.Init ) ), "NumberSineProvider" );
		}
		else
		{
			ReportError( "Invalid wavelength value for MakeNumberSineProvider %", storage.Name );
		}
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