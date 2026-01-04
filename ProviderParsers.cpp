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

			StoreObject(definedProvider, "ColourSequenceProvider");
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
		StoreObject(new ColourWheelProvider(), "ColourWheelProvider");
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
			StoreObject(new ColourFadeProvider((ColourProvider *)(storage.FadeStartColourProvider),
											   (ColourProvider *)(storage.FadeEndColourProvider),
											   GetStoredNumber(0, storage.IntervalProvider)),
						"ColourFadeProvider");
		}
	};

	/// @brief Make a NumberProvider with the specified name and values
	void ObjectParsers::MakeNumberProvider()
	{
		// This is going to be a NumberProvider or a NumberSequenceProvider depending on the
		// number of numbers provided
		if (storage.Numbers.size() > 0)
		{
			NumberProvider *provider = nullptr;
			if (storage.Numbers.size() == 1)
			{
				provider = new NumberProvider(storage.Numbers.at(0));
				StoreObject(provider, "NumberProvider");
			}
			else
			{
				provider = new NumberSequenceProvider();
				for (uint number : storage.Numbers)
				{
					provider->AddValue(number);
				}
				StoreObject(provider, "NumberSequenceProvider");
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
		if ((storage.MinRangeProvider == nullptr) || (storage.MaxRangeProvider == nullptr))
		{
			errorStream << "Missing range parameter for " << storage.Name;
		}
		else
		{
			StoreObject(new RandomNumberProvider(GetStoredNumber(0, storage.MinRangeProvider),
												 GetStoredNumber(0, storage.MaxRangeProvider)),
						"RandomNumberProvider");
		}
	}

	/// @brief Create and store a RandomNumberSetProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeRandomNumberSetProvider()
	{
		if ((storage.MinRangeProvider == nullptr) || (storage.MaxRangeProvider == nullptr))
		{
			errorStream << "Missing range parameter for " << storage.Name;
		}
		else
		{
			StoreObject(new RandomNumberSetProvider(GetStoredNumber(0, storage.MinRangeProvider),
													GetStoredNumber(0, storage.MaxRangeProvider)),
						"RandomNumberSetProvider");
		}
	}
	
	/// @brief Create and store a NumberIntervalProvider instance
	/// @param tokens
	/// @return
	void ObjectParsers::MakeNumberIntervalProvider()
	{
		if ((storage.MinRangeProvider == nullptr) || (storage.MaxRangeProvider == nullptr))
		{
			errorStream << "Missing range parameter for " << storage.Name;
		}
		else
		{
			// The step is optional
			uint step = GetStoredNumber(1, storage.IntervalProvider);

			// The reverse is optional
			bool reverse = GetStoredBoolean( false, storage.ReverseProvider);

			StoreObject(new NumberIntervalProvider(GetStoredNumber(0, storage.MinRangeProvider),
												   GetStoredNumber(0, storage.MaxRangeProvider), step, reverse ),
						"NumberIntervalProvider");
		}
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
			StoreObject(new ColourHSVProvider( ( NumberProvider*) storage.HueProvider, 
						( NumberProvider*) storage.SatProvider, ( NumberProvider*) storage.ValueProvider),
						"ColourHSVProvider");
		}
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

			StoreObject(definedObject, "SegmentSequenceProvider");
		}
		else
		{
			errorStream << "Either no providers or invalid providers for CycleAction " << storage.Name;
		}
	}
}