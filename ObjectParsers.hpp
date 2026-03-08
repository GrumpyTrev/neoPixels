#pragma once

#include "Tokeniser.hpp"
#include "ExecutableItem.hpp"
#include "BooleanProvider.hpp"
#include "ExecutableTypeProvider.hpp"
#include "ObjectStore.hpp"
#include "StringFormatter.hpp"
#include "NumberTrigonometricProvider.hpp"
#include "TriggerAction.hpp"

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
		inline string ErrorString() { return error; }

	private:
		/// @brief The methods used to create each object type
		void MakeColour();
		void MakeSegment();
		void MakeIntervalSegment();
		void MakeDiscreteSegment();
		void MakeSegmentSequence();
		void MakeColourSequenceProvider();
		void MakeNumberProvider();
		void MakeColourHSVProvider();
		void MakeRandomNumberProvider();
		void MakeRandomNumberSetProvider();
		void MakeNumberIntervalProvider();
		void MakeNumberSineProvider();
		void MakeNumberSignedSineProvider();
		void MakeNumberCosineProvider();
		void MakeNumberSignedCosineProvider();
		void MakeSetAction();
		void MakeShiftAction();
		void MakeBlock();
		void MakeFadeAction();
		void MakeTriggerAction();
		void MakeExpressionProvider();

		/// @brief Command processing methods
		void BrightnessCommand();
		void ExecuteBlockCommand();

		/// @brief Apply the extracted parameters to an ExecutableItem
		/// @param item
		ExecutableItem* ApplyCommonItemParameters( ExecutableItem* item );

		/// @brief Apply the extracted parameters to a Provider
		/// @param provider 
		ProviderBase* ApplyCommonProviderParameters( ProviderBase* provider );

		/// @brief Configure a derived NumberTrigonometricProvider
		/// @param provider 
		/// @param typeString 
		void ConfigureTrigonometricProvider( NumberTrigonometricProvider* provider, string typeString );

		/// @brief Has an error been found during parsing
		/// @return
		inline bool Error() { return error.length() > 0; }

		/// @brief Report an error through the 'error' variable
		/// @tparam T 
		/// @tparam ...Args 
		/// @param format 
		/// @param value 
		/// @param ...args 
		template<typename T, typename... Args>
		inline void ReportError( const char* format, T value, Args... args )
		{
			error = "ERROR " + StringFormat( format, value, args... );
		}

		/// @brief Report an error through the 'error' variable
		inline void ReportError( const char* format ) { error = "ERROR " + (string)format; }

		/// @brief The types of named parameters
		enum ParameterType
		{
			NumberParameter = 1,
			SegmentParameter = 2,
			BooleanParameter = 4,
			ColourParameter = 8,
			ItemTypeParameter = 16,
			TriggerParameter = 32
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
		}

		/// @brief Define a DefinitionParser as a pointer to a function that returns
		/// any errors. The resultant object is stored in the ObjectParsers
		typedef void ( ObjectParsers::* DefinitionParser )( );

		/// @brief The structure to hold object parsing data
		struct ObjectParseData
		{
			DefinitionParser Parser;
			bool IsCommand;
		};

		/// @brief Create a ObjectParseData instance to be stored in the parse map
		/// @param type
		/// @param storageLocation
		/// @return
		inline ObjectParseData MakeObjectParseData( DefinitionParser parser, bool isCommand = false )
		{
			return { parser, isCommand };
		}

		/// @brief Extract all the known named parameters, colours, uints and stored object references
		/// from the Tokeniser
		/// @return
		bool ExtractParameters( bool isCommand );

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
		bool GetStoredBoolean( bool defaultValue, BooleanProvider* provider )
		{
			return ( provider == nullptr ) ? defaultValue : provider->Value();
		}

		/// @brief Get an int value from the provider, if it exists
		/// @param defaultValue
		/// @param provider
		/// @return
		int32_t GetStoredNumber( int32_t defaultValue, NumberProvider* provider )
		{
			return ( provider == nullptr ) ? defaultValue : provider->Value();
		}

		/// @brief Get an int value from the object vector at the specified index
		/// @param index
		/// @return
		int32_t GetStoredNumber( uint32_t index )
		{
			return ( index >= storage.Objects.size() ) ? 0 : ( (NumberProvider*)( storage.Objects[ index ] ) )->Value();
		}

		/// @brief Get a SynchType value from the provider, if it exists
		/// @param defaultValue
		/// @param provider
		/// @return
		ExecutableItem::SynchType GetStoredSynchType( ExecutableItem::SynchType defaultValue,
			ExecutableTypeProvider* provider )
		{
			return ( provider == nullptr ) ? defaultValue : provider->Value();
		}

		/// @brief Struct holding all the parameters, objects and numbers extracted from the Tokeniser
		struct ParameterStorage
		{
			/// @brief Clear the struct
			inline void ClearData()
			{
				Delay = nullptr;
				Count = nullptr;
				Time = nullptr;
				Segment = nullptr;
				Led = nullptr;
				ReverseFlag = nullptr;
				Interval = nullptr;
				Min = nullptr;
				Max = nullptr;
				ExecutionType = nullptr;
				Colour = nullptr;
				Hue = nullptr;
				Sat = nullptr;
				Value = nullptr;
				NextTrigger = nullptr;
				ResetTrigger = nullptr;
				FillFlag = nullptr;
				FadeAmount = nullptr;
				While = nullptr;
				Init = nullptr;
				Trace = nullptr;
				Objects.clear();
				Name = "";
			}

			/// @brief Add a referenced object
			/// @param objectToAdd
			inline void AddObject(BaseDefinedObject *objectToAdd) { Objects.push_back(objectToAdd); }

			/// @brief Check that all the stored objects are of a specified type
			/// @tparam T
			/// @return
			template <class T>
			inline bool CheckObjectsType()
			{
				if ( Objects.size() > 0 )
				{
					for ( BaseDefinedObject* object : Objects )
					{
						if ( dynamic_cast<T*>( object ) == nullptr )
						{
							return false;
						}
					}

					return true;
				}

				return false;
			}

			/// @brief The providers holding the named parameters
			NumberProvider* Delay;
			NumberProvider* Count;
			NumberProvider* Time;
			SegmentProvider* Segment;
			NumberProvider* Led;
			BooleanProvider* ReverseFlag;
			NumberProvider* Interval;
			NumberProvider* Min;
			NumberProvider* Max;
			ExecutableTypeProvider* ExecutionType;
			ColourProvider* Colour;
			NumberProvider* Hue;
			NumberProvider* Sat;
			NumberProvider* Value;
			TriggerAction* NextTrigger;
			TriggerAction* ResetTrigger;
			BooleanProvider* FillFlag;
			NumberProvider* FadeAmount;
			NumberProvider* While;
			NumberProvider* Init;
			BooleanProvider* Trace;

			/// @brief The name of the object
			string Name;

			/// @brief Any stored objects referenced
			vector<BaseDefinedObject*> Objects;
		};

		/// @brief Mapping from object type name to maker function and command flag
		map<string, ObjectParseData> parserMap;

		/// @brief The physical strip used to validate pixel numbers in definitions
		LedStrip *commandStrip;

		/// @brief The Tokeniser that contains the tokens to be parsed
		Tokeniser *tokens;

		/// @brief The Commander to use to execute blocks
		Commander *commander;

		/// @brief Error string
		string error;

		/// @brief The one and only ParameterStorage used to hold parameters extracted from the
		/// Tokeniser
		ParameterStorage storage;

		/// @brief The map of parameter name to parameter type and storage location
		map<string, ParameterParseData> parameterParseMap;

		/// @brief The storage area for all constructed objects
		ObjectStore objectStorage;
	};
}