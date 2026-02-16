#include <iostream>
#include "ObjectParsers.hpp"
#include "ObjectStore.hpp"
#include "ColourProvider.hpp"
#include "Trace.hpp"
#include "Block.hpp"
#include "Commander.hpp"
#include "TriggerAction.hpp"
#include "StringFormatter.hpp"

using namespace std;

namespace Lights
{
	/// @brief The ObjectParsers class contains methods to parse definitions for all the objects
	/// @param tokeniserToUse
	/// @param stripToUse
	ObjectParsers::ObjectParsers(Tokeniser *tokeniserToUse, LedStrip *stripToUse, Commander *commanderToUse)
		: tokens(tokeniserToUse), commandStrip(stripToUse), commander(commanderToUse)
	{
		// Initialise the object name to Maker method map
		parserMap.emplace("colour", &ObjectParsers::MakeColour);
		parserMap.emplace("segment", &ObjectParsers::MakeSegment);
		parserMap.emplace("discreteSegment", &ObjectParsers::MakeDiscreteSegment);
		parserMap.emplace("intervalSegment", &ObjectParsers::MakeIntervalSegment);
		parserMap.emplace("segmentSequence", &ObjectParsers::MakeSegmentSequence);
		parserMap.emplace("colourSequence", &ObjectParsers::MakeColourSequenceProvider);
		parserMap.emplace("number", &ObjectParsers::MakeNumberProvider);
		parserMap.emplace("numberSequence", &ObjectParsers::MakeNumberProvider);
		parserMap.emplace("colourWheel", &ObjectParsers::MakeColourWheelProvider);
		parserMap.emplace("colourFader", &ObjectParsers::MakeColourFadeProvider);
		parserMap.emplace("colourHSV", &ObjectParsers::MakeColourHSVProvider);
		parserMap.emplace("randomNumber", &ObjectParsers::MakeRandomNumberProvider);
		parserMap.emplace("randomNumberSet", &ObjectParsers::MakeRandomNumberSetProvider);
		parserMap.emplace( "counter", &ObjectParsers::MakeNumberIntervalProvider );
		parserMap.emplace( "sine", &ObjectParsers::MakeNumberSineProvider );
		parserMap.emplace( "set", &ObjectParsers::MakeSetAction );
		parserMap.emplace("shift", &ObjectParsers::MakeShiftAction);
		parserMap.emplace( "fade", &ObjectParsers::MakeFadeAction );
		parserMap.emplace( "trigger", &ObjectParsers::MakeTriggerAction );
		parserMap.emplace("block", &ObjectParsers::MakeBlock);

		// Initialise the command name to processing method map
		commandMap.emplace("b", &ObjectParsers::BrightnessCommand);
		commandMap.emplace("x", &ObjectParsers::ExecuteBlockCommand);
		commandMap.emplace("t", &ObjectParsers::TraceCommand);

		// Initialise the parameter name to storage location map
		parameterParseMap.emplace( "delay", MakeParseData( NumberParameter, storage.Delay ) );
		parameterParseMap.emplace( "count", MakeParseData( NumberParameter, storage.Count ) );
		parameterParseMap.emplace( "time", MakeParseData( NumberParameter, storage.Time ) );
		parameterParseMap.emplace( "segment", MakeParseData( SegmentParameter, storage.Segment ) );
		parameterParseMap.emplace( "led", MakeParseData( NumberParameter, storage.Led ) );
		parameterParseMap.emplace( "startLed", MakeParseData( NumberParameter, storage.Led ) );
		parameterParseMap.emplace( "reverse", MakeParseData( BooleanParameter, storage.ReverseFlag ) );
		parameterParseMap.emplace( "interval", MakeParseData( NumberParameter, storage.Interval ) );
		parameterParseMap.emplace( "start", MakeParseData( ColourParameter, storage.StartColour ) );
		parameterParseMap.emplace( "end", MakeParseData( ColourParameter, storage.EndColour ) );
		parameterParseMap.emplace( "step", MakeParseData( NumberParameter, storage.Interval ) );
		parameterParseMap.emplace( "min", MakeParseData( NumberParameter, storage.Min ) );
		parameterParseMap.emplace( "max", MakeParseData( NumberParameter, storage.Max ) );
		parameterParseMap.emplace( "type", MakeParseData( ItemTypeParameter, storage.ExecutionType ) );
		parameterParseMap.emplace( "colour", MakeParseData( ColourParameter, storage.Colour ) );
		parameterParseMap.emplace( "hue", MakeParseData( NumberParameter, storage.Hue ) );
		parameterParseMap.emplace( "sat", MakeParseData( NumberParameter, storage.Sat ) );
		parameterParseMap.emplace( "value", MakeParseData( NumberParameter, storage.Value ) );
		parameterParseMap.emplace( "fade", MakeParseData( NumberParameter, storage.FadeAmount ) );
		parameterParseMap.emplace( "next", MakeParseData( TriggerParameter, storage.NextTrigger ) );
		parameterParseMap.emplace( "reset", MakeParseData( TriggerParameter, storage.ResetTrigger ) );
		parameterParseMap.emplace( "fill", MakeParseData( BooleanParameter, storage.FillFlag ) );
		parameterParseMap.emplace( "when", MakeParseData( NumberParameter, storage.When ) );
		parameterParseMap.emplace( "whenNot", MakeParseData( NumberParameter, storage.WhenNot ) );
		parameterParseMap.emplace( "init", MakeParseData( NumberParameter, storage.Init ) );
	}

	/// @brief Find a parser for the specified type and run it
	/// @param typeName
	/// @return
	bool ObjectParsers::ParseDefinition(string typeName)
	{
		bool found = false;
		error = "";

		// Look for commands first
		map<string, DefinitionParser>::iterator it = commandMap.find(typeName);
		if (it != commandMap.end())
		{
			found = true;

			// Process the command
			(this->*it->second)();
		}
		else
		{
			// Not a command, try an object definition
			it = parserMap.find(typeName);
			if (it != parserMap.end())
			{
				cout << "Found parser for " << typeName << "\n";

				found = true;

				// Extract parameters from the tokens
				if (ExtractParameters() == true)
				{
					// Continue with the creation of the object
					(this->*it->second)();
				}
			}
		}

		return found;
	}

	/// @brief Create a Colour from the tokens and add to the named collection held by Colour
	/// @return
	void ObjectParsers::MakeColour()
	{
		// Define a color using either RGB values or a combined value
		// So there should be either 1 or 3 numbers
		if (storage.Numbers.size() == 1)
		{
			Colour::AddColour(storage.Name, Colour(storage.Numbers[0]));
			cout << "Added colour " << storage.Name << " value " << storage.Numbers[0] << "\n";
		}
		else if (storage.Numbers.size() == 3)
		{
			uint16_t redValue = storage.Numbers[ 0 ];
			uint16_t greenValue = storage.Numbers[ 1 ];
			uint16_t blueValue = storage.Numbers[ 2 ];

			Colour::AddColour(storage.Name, Colour(redValue, greenValue, blueValue));
			cout << "Added colour " << storage.Name << " red " << redValue << " green " << greenValue << " blue " << blueValue << "\n";
		}
		else
		{
			ReportError( "Invalid number of numbers for a colour definition %", storage.Name );
		}
	}

	/// @brief Parse and execute a brightness command
	/// @return
	void ObjectParsers::BrightnessCommand()
	{
		// The brightness command expects a numeric value
		if (tokens->TokensLeft() == 1)
		{
			uint16_t brightnessValue;
			if (tokens->NextUint(brightnessValue) == true)
			{
				commandStrip->SetBrightness(brightnessValue);
			}
			else
			{
				ReportError( "Brightness value is not valid: %", tokens->Current() );
			}
		}
		else
		{
			error = "Expected a brightness value";
		}
	}

	/// @brief Execute a trace command
	/// @return
	void ObjectParsers::TraceCommand()
	{
		// There should be just a single on or off specified
		if (tokens->TokensLeft() == 1)
		{
			Trace::TraceOn(tokens->Next() == "on");
		}
		else
		{
			error = "Invalid number of tokens for a trace command";
		}
	}

	/// @brief Execute a Block
	/// @return
	void ObjectParsers::ExecuteBlockCommand()
	{
		// There should be just one Block specified
		if (tokens->TokensLeft() == 1)
		{
			string blockName = tokens->Next();
			Block *blockToExecute = dynamic_cast<Block *>(objectStorage.GetObject(blockName));

			if (blockToExecute != nullptr)
			{
				cout << "Running Block " << blockName << "\n";
				commander->BlockToExecute(blockToExecute);
			}
			else
			{
				ReportError( "Cannot find block %", blockName );
			}
		}
		else
		{
			error = "Invalid number of tokens for a block execution command";
		}
	}

	/// @brief Store an object in the store
	/// @param provider
	/// @param typeName
	void ObjectParsers::StoreObject(BaseDefinedObject *object, string typeName)
	{
		objectStorage.AddObject(storage.Name, object);
		cout << "Added " << typeName << " " << storage.Name << "\n";
	}

	/// @brief Extract all the known named parameters, colours, uints and stored object references
	/// from the Tokeniser
	/// @return
	bool ObjectParsers::ExtractParameters()
	{
		// Remove any old parameters
		storage.ClearData();

		// Check for at least a name
		if (tokens->TokensLeft() > 0)
		{
			storage.Name = tokens->Next();

			// Check for parameters and references to stored objects
			while ((tokens->TokensLeft() > 0) && (Error() == false))
			{
				// If the next token is a parameter then use the parameter map to process it
				string token = tokens->Next();
				uint markerPosition = token.find('=');

				if ((markerPosition > 0) && (markerPosition < token.length() - 1))
				{
					// Extract the parameter and value strings
					string parameter = token.substr(0, markerPosition);
					string value = token.substr(markerPosition + 1);

					// Is this parameter known to us?
					map<string, ParameterParseData>::iterator it = parameterParseMap.find(parameter);
					if (it != parameterParseMap.end())
					{
						ExtractParameter(value, it->second);
					}
					else
					{
						ReportError( "Unknown parameter %", parameter );
					}
				}
				else
				{
					// The token is not a parameter.
					// Check if is a number. If it is add it to the storage vector for numbers.
					// Check if is a colour. If it is then wrap it up in a ColourProvider and
					// add it to the storage vector for objects.
					// Check if it is a reference to a stored item. If it is then add it to the
					// storage vector for objects.
					try
					{
						// Try and convert to a number. Use uint32_t here as some uses require > 16 bits
						uint32_t number = (uint32_t)stoi( token, nullptr, 0 );
						storage.AddNumber(number);
					}
					catch (invalid_argument const &ex)
					{
						// Not a number, try a colour
						Colour possibleColour = Colour::GetColour(token);
						if (possibleColour.value != Colour::InvalidColour.value)
						{
							storage.AddObject(new ColourProvider(possibleColour));
						}
						else
						{
							// Not a colour try any stored object
							BaseDefinedObject *storedObject = objectStorage.GetObject(token);
							if (storedObject != nullptr)
							{
								storage.AddObject(storedObject);
							}
							else
							{
								ReportError( "% is not a number, colour or stored object", token );
							}
						}
					}
				}
			}
		}
		else
		{
			error = "Missing object name";
		}

		return Error() == false;
	}

	/// @brief Convert the supplied value to the type specified in the ParameterParseData.
	/// 	   Wrap the result in a 'provider' and store in the location also given in the ParameterParseData
	/// @param value
	/// @param parseData
	void ObjectParsers::ExtractParameter(string value, ParameterParseData parseData)
	{
		if (parseData.Type == NumberParameter)
		{
			// Look for a number or a referenced NumberProvider
			try
			{
				uint16_t number = (uint16_t)stoi( value, nullptr, 0 );
				parseData.StorageLocation = new NumberProvider(number);
			}
			catch (invalid_argument const &ex)
			{
				// Not a number. Check a NumberProvider
				NumberProvider *parsedObject = dynamic_cast<NumberProvider *>(objectStorage.GetObject(value));
				if (parsedObject == nullptr)
				{
					ReportError( "% is not a NumberProvider", value );
				}
				else
				{
					parseData.StorageLocation = parsedObject;
				}
			}
		}
		else if (parseData.Type == SegmentParameter)
		{
			// Look for a SegmentProvider
			SegmentProvider *parsedObject = dynamic_cast<SegmentProvider *>(objectStorage.GetObject(value));
			if (parsedObject == nullptr)
			{
				ReportError( "% is not a SegmentProvider", value );
			}
			else
			{
				parseData.StorageLocation = parsedObject;
			}
		}
		else if (parseData.Type == BooleanParameter)
		{
			// Look for true or false
			if (value == "true")
			{
				parseData.StorageLocation = new BooleanProvider(true);
			}
			else if (value == "false")
			{
				parseData.StorageLocation = new BooleanProvider(false);
			}
			else
			{
				ReportError( "% is not a Boolean", value );
			}
		}
		else if (parseData.Type == ColourParameter)
		{
			// Lookup the colour name in the defined colours collection
			Colour actionColour = Colour::GetColour(value);
			if (actionColour.value == Colour::InvalidColour.value)
			{
				// Lookup the colour name in the collection of colour providers
				parseData.StorageLocation = dynamic_cast<ColourProvider *>(objectStorage.GetObject(value));

				if (parseData.StorageLocation == nullptr)
				{
					ReportError( "Colour % not known", value );
				}
			}
			else
			{
				parseData.StorageLocation = new ColourProvider(actionColour);
			}
		}
		else if (parseData.Type == ItemTypeParameter)
		{
			ExecutableItem::SynchType itemType = ExecutableItem::sequential;
			if (ExecutableItem::TypeFromString(value, itemType) == false)
			{
				ReportError( "% is not a valid Item type", value );
			}
			else
			{
				parseData.StorageLocation = new ExecutableTypeProvider(itemType);
			}
		}
		else if ( parseData.Type == TriggerParameter )
		{
			// Look for a TriggerAction
			TriggerAction* parsedObject = dynamic_cast<TriggerAction*>( objectStorage.GetObject( value ) );
			if ( parsedObject == nullptr )
			{
				ReportError( "% is not a TriggerAction", value );
			}
			else
			{
				parseData.StorageLocation = parsedObject;
			}
		}
	}
}