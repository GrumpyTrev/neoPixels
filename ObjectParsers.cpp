#include <iostream>
#include "ObjectParsers.hpp"
#include "ObjectStore.hpp"
#include "ColourProvider.hpp"
#include "Trace.hpp"
#include "Block.hpp"
#include "Commander.hpp"

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
		parserMap.emplace("numberInterval", &ObjectParsers::MakeNumberIntervalProvider);
		parserMap.emplace("sineSequence", &ObjectParsers::MakeNumberSineProvider);
		parserMap.emplace("fill", &ObjectParsers::MakeFillAction);
		parserMap.emplace("set", &ObjectParsers::MakeSetAction);
		parserMap.emplace("cycle", &ObjectParsers::MakeCycleAction);
		parserMap.emplace("shift", &ObjectParsers::MakeShiftAction);
		parserMap.emplace("block", &ObjectParsers::MakeBlock);

		// Initialise the command name to processing method map
		commandMap.emplace("b", &ObjectParsers::BrightnessCommand);
		commandMap.emplace("x", &ObjectParsers::ExecuteBlockCommand);
		commandMap.emplace("t", &ObjectParsers::TraceCommand);

		// Initialise the parameter name to storage location map
		parameterParseMap.emplace("delay", MakeParseData(NumberParameter, storage.DelayProvider));
		parameterParseMap.emplace("count", MakeParseData(NumberParameter, storage.CountProvider));
		parameterParseMap.emplace("time", MakeParseData(NumberParameter, storage.TimeProvider));
		parameterParseMap.emplace("segment", MakeParseData(SegmentParameter, storage.SegmentProvider));
		parameterParseMap.emplace("startLed", MakeParseData(NumberParameter, storage.StartLedProvider));
		parameterParseMap.emplace("reverse", MakeParseData(BooleanParameter, storage.ReverseProvider));
		parameterParseMap.emplace("interval", MakeParseData(NumberParameter, storage.IntervalProvider));
		parameterParseMap.emplace("maxLed", MakeParseData(NumberParameter, storage.CountProvider));
		parameterParseMap.emplace("start", MakeParseData(ColourParameter, storage.FadeStartColourProvider));
		parameterParseMap.emplace("end", MakeParseData(ColourParameter, storage.FadeEndColourProvider));
		parameterParseMap.emplace("step", MakeParseData(NumberParameter, storage.IntervalProvider));
		parameterParseMap.emplace("minimum", MakeParseData(NumberParameter, storage.MinRangeProvider));
		parameterParseMap.emplace("maximum", MakeParseData(NumberParameter, storage.MaxRangeProvider));
		parameterParseMap.emplace("type", MakeParseData(ItemTypeParameter, storage.ItemTypeProvider));
		parameterParseMap.emplace("colour", MakeParseData(ColourParameter, storage.ItemColourProvider));
		parameterParseMap.emplace("hue", MakeParseData(NumberParameter, storage.HueProvider));
		parameterParseMap.emplace("sat", MakeParseData(NumberParameter, storage.SatProvider));
		parameterParseMap.emplace("value", MakeParseData(NumberParameter, storage.ValueProvider));
	}

	/// @brief Find a parser for the specified type and run it
	/// @param typeName
	/// @return
	bool ObjectParsers::ParseDefinition(string typeName)
	{
		bool found = false;

		// Look for commands first
		map<string, ObjectParsers::DefinitionParser>::iterator it = commandMap.find(typeName);
		if (it != commandMap.end())
		{
			found = true;
			errorStream.str("");

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
				errorStream.str("");

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
			uint redValue = storage.Numbers[0];
			uint greenValue = storage.Numbers[1];
			uint blueValue = storage.Numbers[2];

			Colour::AddColour(storage.Name, Colour(redValue, greenValue, blueValue));
			cout << "Added colour " << storage.Name << " red " << redValue << " green " << greenValue << " blue " << blueValue << "\n";
		}
		else
		{
			errorStream << "Invalid number of numbers for a colour definition" << storage.Name;
		}
	}

	/// @brief Parse and execute a brightness command
	/// @return
	void ObjectParsers::BrightnessCommand()
	{
		// The brightness command expects a numeric value
		if (tokens->TokensLeft() == 1)
		{
			uint brightnessValue;
			if (tokens->NextUint(brightnessValue) == true)
			{
				commandStrip->SetBrightness(brightnessValue);
			}
			else
			{
				errorStream << "Brightness value is not valid: " << tokens->Current();
			}
		}
		else
		{
			errorStream << "Expected a brightness value";
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
			errorStream << "Invalid number of tokens for a trace command";
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
				errorStream << "Cannot find block " << blockName;
			}
		}
		else
		{
			errorStream << "Invalid number of tokens for a block execution command";
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
						errorStream << "Unknown parameter " << parameter;
					}
				}
				else
				{
					// The token is not a parameter.
					// Check if is a number. If it is add it to the storage vector for numbers.
					// Check if is a colour. If it is then wrap it up in a ColourReference and
					// add it to the storage vector for objects.
					// Check if it is a reference to a stored item. If it is then add it to the
					// storage vector for objects.
					try
					{
						// Try and convert to a number
						uint number = (uint)stoi(token, nullptr, 0);
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
								errorStream << token << " is not a number, colour or stored object";
							}
						}
					}
				}
			}
		}
		else
		{
			errorStream << "Missing object name";
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
				uint number = (uint)stoi(value, nullptr, 0);
				parseData.StorageLocation = new NumberProvider(number);
			}
			catch (invalid_argument const &ex)
			{
				// Not a number. Check a NumberProvider
				NumberProvider *parsedObject = dynamic_cast<NumberProvider *>(objectStorage.GetObject(value));
				if (parsedObject == nullptr)
				{
					errorStream << value << " is not a NumberProvider";
				}
				else
				{
					parseData.StorageLocation = parsedObject;
				}
			}
		}
		else if (parseData.Type == SegmentParameter)
		{
			// Look for a SequenceProvider
			SegmentProvider *parsedObject = dynamic_cast<SegmentProvider *>(objectStorage.GetObject(value));
			if (parsedObject == nullptr)
			{
				errorStream << value << " is not a SequenceProvider";
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
				errorStream << value << " is not a Boolean";
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
					errorStream << "Colour " << value << " not known";
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
				errorStream << value << " is not a valid Item type";
			}
			else
			{
				parseData.StorageLocation = new ExecutableTypeProvider(itemType);
			}
		}
	}
}