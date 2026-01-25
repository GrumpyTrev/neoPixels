#pragma once

#include "Tokeniser.hpp"
#include "ExecutableItem.hpp"
#include "BooleanProvider.hpp"
#include "ExecutableTypeProvider.hpp"
#include "ObjectStore.hpp"
#include <sstream>

using namespace std;

namespace Lights
{
	// Forward reference to Commander
	class Commander;
	class ObjectParsers
	{
	public:
		/// @brief Construct and initialise an ObjectParsers instance
		ObjectParsers(Tokeniser *tokeniserToUse, LedStrip *stripToUse, Commander *commanderToUse);

		/// @brief Find a parser for the specified type and run it
		/// @param typeName
		/// @return
		bool ParseDefinition(string typeName);

		/// @brief The error string held in the private stream
		/// @return
		inline string ErrorString() { return errorStream.str(); }

	private:
		/// @brief The methods used to create each object type
		void MakeColour();
		void MakeSegment();
		void MakeIntervalSegment();
		void MakeDiscreteSegment();
		void MakeSegmentSequence();
		void MakeColourSequenceProvider();
		void MakeNumberProvider();
		void MakeColourWheelProvider();
		void MakeColourFadeProvider();
		void MakeColourHSVProvider();
		void MakeRandomNumberProvider();
		void MakeRandomNumberSetProvider();
		void MakeNumberIntervalProvider();
		void MakeNumberSineProvider();
		void MakeFillAction();
		void MakeSetAction();
		void MakeShiftAction();
		void MakeCycleAction();
		void MakeBlock();
		void MakeFadeAction();

		/// @brief Command processing methods
		void BrightnessCommand();
		void ExecuteBlockCommand();
		void TraceCommand();

		/// @brief Apply the extracted parameters to an ExecutableItem
		/// @param item
		void
		ApplyCommonItemParameters(ExecutableItem *item);

		/// @brief Has an error been found during parsing
		/// @return
		inline bool Error() { return errorStream.tellp() > 0; }

		/// @brief The types of named parameters
		enum ParameterType
		{
			NumberParameter = 1,
			SegmentParameter = 2,
			BooleanParameter = 4,
			ColourParameter = 8,
			ItemTypeParameter = 16
		};

		/// @brief The structure used to hold a parameter type, and the location it should be stored
		struct ParameterParseData
		{
			ParameterType Type;
			BaseDefinedObject *&StorageLocation;
		};

		/// @brief Create a ParameterParseData instance to be stored in the parameter map
		/// @param type
		/// @param storageLocation
		/// @return
		inline ParameterParseData MakeParseData(ParameterType type, BaseDefinedObject *&storageLocation)
		{
			return {type, storageLocation};
		};

		/// @brief Extract all the known named parameters, colours, uints and stored object references
		/// from the Tokeniser
		/// @return
		bool ExtractParameters();

		/// @brief Extract a specific parameter type from the string value suppl;ied
		/// @param value
		/// @param parseData
		void ExtractParameter(string value, ParameterParseData parseData);

		/// @brief Store an object in the store
		/// @param provider
		/// @param typeName
		void StoreObject(BaseDefinedObject *provider, string typeName);

		/// @brief Get a boolean value from the provider, if it exists
		/// @param defaultValue
		/// @param provider
		/// @return
		bool GetStoredBoolean(bool defaultValue, BaseDefinedObject *provider)
		{
			return (provider == nullptr) ? defaultValue : ((BooleanProvider *)(provider))->Value();
		}

		/// @brief Get a uint value from the provider, if it exists
		/// @param defaultValue
		/// @param provider
		/// @return
		uint GetStoredNumber(uint defaultValue, BaseDefinedObject *provider)
		{
			return (provider == nullptr) ? defaultValue : ((NumberProvider *)(provider))->Value();
		}

		/// @brief Get a SynchType value from the provider, if it exists
		/// @param defaultValue
		/// @param provider
		/// @return
		ExecutableItem::SynchType GetStoredSynchType(ExecutableItem::SynchType defaultValue, BaseDefinedObject *provider)
		{
			return (provider == nullptr) ? defaultValue : ((ExecutableTypeProvider *)(provider))->Value();
		}

		/// @brief Struct holding all the parameters, objects and numbers extracted from the Tokeniser
		struct ParameterStorage
		{
			/// @brief Clear the struct
			inline void ClearData()
			{
				DelayProvider = nullptr;
				CountProvider = nullptr;
				TimeProvider = nullptr;
				SegmentProvider = nullptr;
				StartLedProvider = nullptr;
				ReverseProvider = nullptr;
				IntervalProvider = nullptr;
				FadeStartColourProvider = nullptr;
				FadeEndColourProvider = nullptr;
				MinRangeProvider = nullptr;
				MaxRangeProvider = nullptr;
				ItemTypeProvider = nullptr;
				ItemColourProvider = nullptr;
				HueProvider = nullptr;
				SatProvider = nullptr;
				ValueProvider = nullptr;
				Objects.clear();
				Numbers.clear();
				Name = "";
			}

			/// @brief Add a referenced object
			/// @param objectToAdd
			inline void AddObject(BaseDefinedObject *objectToAdd) { Objects.push_back(objectToAdd); }

			/// @brief Add a number
			/// @param numberToAdd
			inline void AddNumber(uint numberToAdd) { Numbers.push_back(numberToAdd); }

			/// @brief Check that all the stored objects are of a specified type
			/// @tparam T
			/// @return
			template <class T>
			inline bool CheckObjectsType()
			{
				for (BaseDefinedObject *object : Objects)
				{
					if (dynamic_cast<T *>(object) == nullptr)
					{
						return false;
					}
				}
				return true;
			}

			/// @brief The providers holding the named parameters
			BaseDefinedObject *DelayProvider;
			BaseDefinedObject *CountProvider;
			BaseDefinedObject *TimeProvider;
			BaseDefinedObject *SegmentProvider;
			BaseDefinedObject *StartLedProvider;
			BaseDefinedObject *ReverseProvider;
			BaseDefinedObject *IntervalProvider;
			BaseDefinedObject *FadeStartColourProvider;
			BaseDefinedObject *FadeEndColourProvider;
			BaseDefinedObject *MinRangeProvider;
			BaseDefinedObject *MaxRangeProvider;
			BaseDefinedObject *ItemTypeProvider;
			BaseDefinedObject *ItemColourProvider;
			BaseDefinedObject *HueProvider;
			BaseDefinedObject *SatProvider;
			BaseDefinedObject *ValueProvider;

			/// @brief The name of the object
			string Name;

			/// @brief Any stored objects referenced
			vector<BaseDefinedObject *> Objects;

			/// @brief Any numbers defined for the object
			vector<uint> Numbers;
		};

		/// @brief Define a DefinitionParser as a pointer to a function that returns
		/// any errors. The resultant object is stored in the ObjectParsers
		typedef void (ObjectParsers::*DefinitionParser)();

		/// @brief Mapping from object type name to maker function
		map<string, DefinitionParser> parserMap;

		/// @brief Mapping from command name to processing method
		map<string, DefinitionParser> commandMap;

		/// @brief The physical strip used to validate pixel numbers in definitions
		LedStrip *commandStrip;

		/// @brief The Tokeniser that contains the tokens to be parsed
		Tokeniser *tokens;

		/// @brief The Commander to use to execute blocks
		Commander *commander;

		/// @brief Error string
		ostringstream errorStream;

		/// @brief The one and only ParameterStorage used to hold parameters extracted from the
		/// Tokeniser
		ParameterStorage storage;

		/// @brief The map of parameter name to parameter type and storage location
		map<string, ParameterParseData> parameterParseMap;

		/// @brief The storage area for all constructed objects
		ObjectStore objectStorage;
	};
}