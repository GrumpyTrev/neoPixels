#include <iostream>
#include "ObjectParsers.hpp"
#include "Action.hpp"
#include "SetAction.hpp"
#include "ShiftAction.hpp"
#include "Block.hpp"
#include "FadeAction.hpp"
#include "TriggerAction.hpp"

using namespace std;

namespace Lights
{
	/// @brief Parse a SetAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeSetAction()
	{
		// Check for the colour provider
		if ( storage.Colour == nullptr )
		{
			ReportError( "No colour for SetAction %", storage.Name );
		}
		else
		{
			SetAction* definedAction = new SetAction();
			ApplyCommonItemParameters( definedAction );
			definedAction->Provider( (ColourProvider*)storage.Colour );
			definedAction->LedProvider( (NumberProvider*)storage.Led );
			definedAction->WhenProvider( (NumberProvider*)storage.When );
			definedAction->WhenNotProvider( (NumberProvider*)storage.WhenNot );
			definedAction->FillWithSingleColour( GetStoredBoolean( false, storage.FillFlag ) );
			StoreObject(definedAction, "SetAction");
		}
	};

	/// @brief Parse a ShiftAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeShiftAction()
	{
		ShiftAction *definedAction = new ShiftAction();
		ApplyCommonItemParameters(definedAction);
		StoreObject(definedAction, "ShiftAction");
	};

	/// @brief Parse a FadeAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeFadeAction()
	{
		// Check for fade percentage parameter of the correct range
		int32_t percentage = GetStoredNumber( 0, storage.FadeAmount );
		if ( ( storage.FadeAmount == nullptr ) || ( percentage < 1 ) || ( percentage > 1000 ) )
		{
			ReportError( "No fade percentage, or invalid value, for FadeAction %", storage.Name );
		}
		else
		{
			FadeAction* definedAction = new FadeAction();
			ApplyCommonItemParameters(definedAction);
			definedAction->Percentage( percentage );
			definedAction->LedProvider( (NumberProvider*)storage.Led );
			StoreObject(definedAction, "FadeAction");
		}
	};

	/// @brief Parse a TriggerAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeTriggerAction()
	{
		TriggerAction* definedAction = new TriggerAction();
		ApplyCommonItemParameters( definedAction );
		StoreObject( definedAction, "TriggerAction" );
	};

	/// @brief Parse a Block definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeBlock()
	{
		// There should be at least one ExecutableItem
		if ( ( storage.Objects.size() > 0 ) && ( storage.CheckObjectsType<ExecutableItem>() == true ) )
		{
			Block* definedBlock = new Block();
			ApplyCommonItemParameters(definedBlock);

			for (BaseDefinedObject *object : storage.Objects)
			{
				definedBlock->AddItem(dynamic_cast<ExecutableItem *>(object)->Clone());
			}

			StoreObject(definedBlock, "Block");
		}
		else
		{
			ReportError( "Either no items or invalid items for Block %", storage.Name );
		}
	}

	/// @brief Apply the common executable item parameters already found to the item
	/// @param error
	/// @return
	void ObjectParsers::ApplyCommonItemParameters(ExecutableItem *item)
	{
		dynamic_cast<BaseDefinedObject *>(item)->Name(storage.Name);
		item->Counter( (NumberProvider*)( storage.Count ) );
		item->ExecutionTime( (NumberProvider*)( storage.Time ) );
		item->PostDelay( (NumberProvider*)( storage.Delay ) );
		item->Segment( (SegmentProvider*)( storage.Segment ) );
		item->ItemSynch( GetStoredSynchType( ExecutableItem::sequential, storage.ExecutionType ) );
		item->DefaultSegment( dynamic_cast<SegmentProvider*>( objectStorage.GetObject( "fullSegment" ) ) );
	}
}