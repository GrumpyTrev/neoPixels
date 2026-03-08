#include <iostream>
#include "ObjectParsers.hpp"
#include "Commander.hpp"
#include "ExpressionOperator.hpp"

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
		parserMap.emplace( "b", MakeObjectParseData( &ObjectParsers::BrightnessCommand, true ) );
		parserMap.emplace( "x", MakeObjectParseData( &ObjectParsers::ExecuteBlockCommand, true ) );
		parserMap.emplace( "colour", MakeObjectParseData( &ObjectParsers::MakeColour ) );
		parserMap.emplace( "segment", MakeObjectParseData( &ObjectParsers::MakeSegment ) );
		parserMap.emplace( "discreteSegment", MakeObjectParseData( &ObjectParsers::MakeDiscreteSegment ) );
		parserMap.emplace( "intervalSegment", MakeObjectParseData( &ObjectParsers::MakeIntervalSegment ) );
		parserMap.emplace( "segmentSequence", MakeObjectParseData( &ObjectParsers::MakeSegmentSequence ) );
		parserMap.emplace( "colourSequence", MakeObjectParseData( &ObjectParsers::MakeColourSequenceProvider ) );
		parserMap.emplace( "number", MakeObjectParseData( &ObjectParsers::MakeNumberProvider ) );
		parserMap.emplace( "numberSequence", MakeObjectParseData( &ObjectParsers::MakeNumberProvider ) );
		parserMap.emplace( "colourHSV", MakeObjectParseData( &ObjectParsers::MakeColourHSVProvider ) );
		parserMap.emplace( "randomNumber", MakeObjectParseData( &ObjectParsers::MakeRandomNumberProvider ) );
		parserMap.emplace( "randomNumberSet", MakeObjectParseData( &ObjectParsers::MakeRandomNumberSetProvider ) );
		parserMap.emplace( "counter", MakeObjectParseData( &ObjectParsers::MakeNumberIntervalProvider ) );
		parserMap.emplace( "sine", MakeObjectParseData( &ObjectParsers::MakeNumberSineProvider ) );
		parserMap.emplace( "signedSine", MakeObjectParseData( &ObjectParsers::MakeNumberSignedSineProvider ) );
		parserMap.emplace( "cosine", MakeObjectParseData( &ObjectParsers::MakeNumberCosineProvider ) );
		parserMap.emplace( "signedCosine", MakeObjectParseData( &ObjectParsers::MakeNumberSignedCosineProvider ) );
		parserMap.emplace( "set", MakeObjectParseData( &ObjectParsers::MakeSetAction ) );
		parserMap.emplace( "shift", MakeObjectParseData( &ObjectParsers::MakeShiftAction ) );
		parserMap.emplace( "fade", MakeObjectParseData( &ObjectParsers::MakeFadeAction ) );
		parserMap.emplace( "trigger", MakeObjectParseData( &ObjectParsers::MakeTriggerAction ) );
		parserMap.emplace( "block", MakeObjectParseData( &ObjectParsers::MakeBlock ) );
		parserMap.emplace( "expr", MakeObjectParseData( &ObjectParsers::MakeExpressionProvider ) );

		// Initialise the parameter name to storage location map
		parameterParseMap.emplace( "delay", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Delay ) );
		parameterParseMap.emplace( "count", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Count ) );
		parameterParseMap.emplace( "time", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Time ) );
		parameterParseMap.emplace( "segment", MakeParseData( SegmentParameter, (BaseDefinedObject*&)storage.Segment ) );
		parameterParseMap.emplace( "led", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Led ) );
		parameterParseMap.emplace( "startLed", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Led ) );
		parameterParseMap.emplace( "reverse", MakeParseData( BooleanParameter, (BaseDefinedObject*&)storage.ReverseFlag ) );
		parameterParseMap.emplace( "interval", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Interval ) );
		parameterParseMap.emplace( "step", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Interval ) );
		parameterParseMap.emplace( "min", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Min ) );
		parameterParseMap.emplace( "max", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Max ) );
		parameterParseMap.emplace( "type", MakeParseData( ItemTypeParameter, (BaseDefinedObject*&)storage.ExecutionType ) );
		parameterParseMap.emplace( "colour", MakeParseData( ColourParameter, (BaseDefinedObject*&)storage.Colour ) );
		parameterParseMap.emplace( "hue", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Hue ) );
		parameterParseMap.emplace( "sat", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Sat ) );
		parameterParseMap.emplace( "value", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Value ) );
		parameterParseMap.emplace( "fade", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.FadeAmount ) );
		parameterParseMap.emplace( "next", MakeParseData( TriggerParameter, (BaseDefinedObject*&)storage.NextTrigger ) );
		parameterParseMap.emplace( "reset", MakeParseData( TriggerParameter, (BaseDefinedObject*&)storage.ResetTrigger ) );
		parameterParseMap.emplace( "fill", MakeParseData( BooleanParameter, (BaseDefinedObject*&)storage.FillFlag ) );
		parameterParseMap.emplace( "while", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.While ) );
		parameterParseMap.emplace( "init", MakeParseData( NumberParameter, (BaseDefinedObject*&)storage.Init ) );
		parameterParseMap.emplace( "trace", MakeParseData( BooleanParameter, (BaseDefinedObject*&)storage.Trace ) );

		// Add the built-in expression operators to the object store
		objectStorage.AddObject( "add", new ExpressionOperator( ExpressionOperator::add ) );
		objectStorage.AddObject( "sub", new ExpressionOperator( ExpressionOperator::subtract ) );
		objectStorage.AddObject( "div", new ExpressionOperator( ExpressionOperator::divide ) );
		objectStorage.AddObject( "div", new ExpressionOperator( ExpressionOperator::divide ) );
		objectStorage.AddObject( "mul", new ExpressionOperator( ExpressionOperator::multiply ) );
		objectStorage.AddObject( "self", new ExpressionOperator( ExpressionOperator::self ) );
		objectStorage.AddObject( "not", new ExpressionOperator( ExpressionOperator::logicNot ) );
	}

	/// @brief Find a parser for the specified type and run it
	/// @param typeName
	/// @return
	bool ObjectParsers::ParseDefinition(string typeName)
	{
		// Reset the error string
		error = "";

		// Look in the parser map
		map<string, ObjectParseData>::iterator it = parserMap.find( typeName );
		if ( it != parserMap.end() )
		{
			cout << "Found parser for " << typeName << "\n";

			// Extract parameters from the tokens
			if ( ExtractParameters( it->second.IsCommand ) == true )
			{
				// Continue with the creation of the object
				( this->*it->second.Parser )( );
			}
		}

		return ( it != parserMap.end() );
	}

	/// @brief Create a Colour from the tokens and add to the named collection held by Colour
	/// @return
	void ObjectParsers::MakeColour()
	{
		// Define a color using either RGB values or a combined value
		// So there should be either 1 or 3 numbers
		if ( ( ( storage.Objects.size() == 1 ) || ( storage.Objects.size() == 3 ) ) &&
			( storage.CheckObjectsType<NumberProvider>() == true ) )
		{
			if ( storage.Objects.size() == 1 )
			{
				Colour::AddColour( storage.Name, Colour( GetStoredNumber( 0 ) ) );
				cout << "Added colour " << storage.Name << " value " << GetStoredNumber( 0 ) << "\n";
			}
			else if ( storage.Objects.size() == 3 )
			{
				uint8_t redValue = GetStoredNumber( 0 );
				uint8_t greenValue = GetStoredNumber( 1 );
				uint8_t blueValue = GetStoredNumber( 2 );

				Colour::AddColour( storage.Name, Colour( redValue, greenValue, blueValue ) );
				cout << "Added colour " << storage.Name << " red " << redValue << " green " << greenValue << " blue " << blueValue << "\n";
			}
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
		// There should be a single numeric arguement
		if ( ( storage.Objects.size() == 1 ) && ( storage.CheckObjectsType<NumberProvider>() == true ) )
		{
			int32_t brightnessValue = GetStoredNumber( 0 );
			if ( ( brightnessValue >= 0 ) && ( brightnessValue < 256 ) )
			{
				commandStrip->SetBrightness( brightnessValue );
			}
			else
			{
				ReportError( "Brightness value is not valid: %", brightnessValue );
			}
		}
		else
		{
			ReportError( "Expected a brightness value" );
		}
	}

	/// @brief Execute a Block
	/// @return
	void ObjectParsers::ExecuteBlockCommand()
	{
		// There should be just one Block specified
		if ( ( storage.Objects.size() == 1 ) && ( storage.CheckObjectsType<Block>() == true ) )
		{
			Block* blockToExecute = (Block*)storage.Objects[ 0 ];
			cout << "Running Block " << blockToExecute->Name() << "\n";
			commander->BlockToExecute( blockToExecute );
		}
		else
		{
			ReportError( "Invalid number of arguments for a block execution command" );
		}
	}

	/// @brief Store an object in the store
	/// @param provider
	/// @param typeName
	void ObjectParsers::StoreObject(BaseDefinedObject *object, string typeName)
	{
		objectStorage.AddObject( storage.Name, object );
		object->TraceOn( GetStoredBoolean( false, storage.Trace ) );

		cout << "Added " << typeName << " " << storage.Name << " trace=" << std::boolalpha << object->TraceOn() << "\n";
	}

	/// @brief Extract all the known named parameters, colours, ints and stored object references
	/// from the Tokeniser
	/// @return
	bool ObjectParsers::ExtractParameters( bool isCommand )
	{
		// Remove any old parameters
		storage.ClearData();

		// Check for at least a name (if not a command )
		if ( ( isCommand == true ) || ( tokens->TokensLeft() > 0 ) )
		{
			// The first parameter is the object name, unless this is a command
			if ( isCommand == false )
			{
				storage.Name = tokens->Next();
			}

			// Check for parameters and references to stored objects
			while ((tokens->TokensLeft() > 0) && (Error() == false))
			{
				// If the next token is a parameter then use the parameter map to process it
				string token = tokens->Next();
				size_t markerPosition = token.find( '=' );

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
					// Check if is a boolean value. If it is then wrap it up in a BooleanProvider.
					try
					{
						// Try and convert to a number.
						storage.AddObject( new NumberProvider( (int32_t)stoi( token, nullptr, 0 ) ) );
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
								// Look for true or false
								if ( token == "true" )
								{
									storage.AddObject( new BooleanProvider( true ) );
								}
								else if ( token == "false" )
								{
									storage.AddObject( new BooleanProvider( false ) );
								}
								else
								{
									ReportError( "% is not a number, colour, stored object or boolean", token );
								}
							}
						}
					}
				}
			}
		}
		else
		{
			ReportError( "Missing object name" );
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
				parseData.StorageLocation = new NumberProvider( (int32_t)stoi( value, nullptr, 0 ) );
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