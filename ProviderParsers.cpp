#include "ObjectParsers.hpp"
#include "ObjectStore.hpp"
#include "ColourSequenceProvider.hpp"
#include "NumberProvider.hpp"
#include "NumberSequenceProvider.hpp"
#include "RandomNumberProvider.hpp"
#include "RandomNumberSetProvider.hpp"
#include "SegmentSequenceProvider.hpp"
#include "NumberIntervalProvider.hpp"
#include "ColourHSVProvider.hpp"
#include "NumberTrigonometricProvider.hpp"
#include "ExpressionOperator.hpp"
#include "ExpressionProvider.hpp"

using namespace std;

namespace Lights
{
	/// @brief Make a ColourSequenceProvider with one or more Colours
	/// @return
	void ObjectParsers::MakeColourSequenceProvider()
	{
		// Make sure that all the stored objects are ColourProviders, and there is at least one of them
		if ( storage.CheckObjectsType<ColourProvider>() == true )
		{
			ColourSequenceProvider* definedProvider = new ColourSequenceProvider();
			for ( BaseDefinedObject* object : storage.Objects )
			{
				definedProvider->AddValue( static_cast<ColourProvider*>( object )->Value() );
			}

			StoreObject( ApplyCommonProviderParameters( definedProvider ), "ColourSequenceProvider" );
		}
		else
		{
			ReportError( "Invalid colours for ColourSequenceProvider %", storage.Name );
		}
	}

	/// @brief Make a NumberProvider with the specified name and value(s)
	void ObjectParsers::MakeNumberProvider()
	{
		// This is going to be a NumberProvider or a NumberSequenceProvider depending on the
		// number of numbers provided
		if ( storage.CheckObjectsType<NumberProvider>() == true )
		{
			if ( storage.Objects.size() == 1 )
			{
				StoreObject( ApplyCommonProviderParameters( new NumberProvider( GetStoredNumber( 0 ) ) ),
					"NumberProvider" );
			}
			else
			{
				NumberSequenceProvider* provider = new NumberSequenceProvider();
				for ( BaseDefinedObject* object : storage.Objects )
				{
					provider->AddValue( static_cast<NumberProvider*>( object )->Value() );
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
		NumberProvider* minProvider = ( storage.Min != nullptr ) ? storage.Min : new NumberProvider( 0 );
		NumberProvider* maxProvider = ( storage.Max != nullptr ) ? storage.Max : new NumberProvider( 65535 );

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
				new ColourHSVProvider( storage.Hue, storage.Sat, storage.Value,
					GetStoredNumber( 0, storage.Interval ) ) ), "ColourHSVProvider" );
		}
	}

	/// @brief Create and store a NumberSineProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberSineProvider()
	{
		ConfigureTrigonometricProvider( new NumberSineProvider(), "NumberSineProvider" );
	}

	/// @brief Create and store a NumberSignedSineProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberSignedSineProvider()
	{
		ConfigureTrigonometricProvider( new NumberSignedSineProvider(), "NumberSignedSineProvider" );
	}

	/// @brief Create and store a NumberCosineProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberCosineProvider()
	{
		ConfigureTrigonometricProvider( new NumberCosineProvider(), "NumberCosineProvider" );
	}

	/// @brief Create and store a NumberSignedCosineProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberSignedCosineProvider()
	{
		ConfigureTrigonometricProvider( new NumberSignedCosineProvider(), "NumberSignedCosineProvider" );
	}

	/// @brief Configure and store a derived NumberTrigonometricProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::ConfigureTrigonometricProvider( NumberTrigonometricProvider* provider, string typeString )
	{
		// Get the optional waveLength ( stored in Interval ), and check it is in range
		int32_t waveLength = GetStoredNumber( 256, storage.Interval );
		if ( ( waveLength > 0 ) && ( waveLength <= 256 ) )
		{
			ApplyCommonProviderParameters( provider );
			provider->Configure( waveLength, storage.Init );
			StoreObject( provider, typeString );
		}
		else
		{
			ReportError( "Invalid wavelength value for % %", typeString, storage.Name );
		}
	}

	/// @brief Make a SegmentSequenceProvider from the ParameterStorage
	/// @param tokens
	/// @return
	void ObjectParsers::MakeSegmentSequence()
	{
		// There should be at least one segment provider and they should all be SegmentProviders
		if ( storage.CheckObjectsType<SegmentProvider>() == true )
		{
			SegmentSequenceProvider* definedObject = new SegmentSequenceProvider();

			for ( BaseDefinedObject* object : storage.Objects )
			{
				definedObject->AddValue( static_cast<SegmentProvider*>( object )->Value() );
			}

			StoreObject( ApplyCommonProviderParameters( definedObject ), "SegmentSequenceProvider" );
		}
		else
		{
			ReportError( "Either no providers or invalid providers for SegmentProvider %", storage.Name );
		}
	}

	/// @brief Make an ExpressionProvider from the ParameterStorage
	void ObjectParsers::MakeExpressionProvider()
	{
		// There should be at least one object, and all objects should be either NumberProviders or 
		// ExpressionOperators
		if ( storage.Objects.size() > 0 )
		{
			bool validObjects = true;
			uint objectIndex = 0;
			while ( ( validObjects == true ) && ( objectIndex < storage.Objects.size() ) )
			{
				BaseDefinedObject* object = storage.Objects[ objectIndex++ ];
				if ( ( dynamic_cast<NumberProvider*>( object ) == nullptr ) &&
					( dynamic_cast<ExpressionOperator*>( object ) == nullptr ) )
				{
					ReportError( "% is not a NumberProvider or ExpressionOperator for ExpressionProvider %",
						object->Name(), storage.Name );

					validObjects = false;
				}
			}

			if ( validObjects == true )
			{
				StoreObject( ApplyCommonProviderParameters( new ExpressionProvider( storage.Objects ) ),
					"ExpressionProvider" );
			}
			else
			{
				ReportError( "No NumberProviders or ExpressionOperators for ExpressionProvider %", storage.Name );
			}
		}
	}

	/// @brief Apply the extracted parameters to a Provider
	/// @param error
	/// @return
	ProviderBase* ObjectParsers::ApplyCommonProviderParameters( ProviderBase* provider )
	{
		if ( storage.NextTrigger != nullptr )
		{
			storage.NextTrigger->AddCallback( new Callback<ProviderBase>( provider, &ProviderBase::Next ) );

			// If there is a Next trigger then clear SelfIncrement
			provider->SelfIncrement( false );
		}

		if ( storage.ResetTrigger != nullptr )
		{
			storage.ResetTrigger->AddCallback( new Callback<ProviderBase>( provider, &ProviderBase::Reset ) );
		}

		return provider;
	}
}