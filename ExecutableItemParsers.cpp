#include "ObjectParsers.hpp"
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
			definedAction->Provider( storage.Colour );
			definedAction->LedProvider( storage.Led );
			definedAction->FillWithSingleColour( GetStoredBoolean( false, storage.FillFlag ) );
			StoreObject( ApplyCommonItemParameters( definedAction ), "SetAction" );
		}
	};

	/// @brief Parse a ShiftAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeShiftAction()
	{
		ShiftAction* definedAction = new ShiftAction();
		definedAction->LedProvider( storage.Led );
		StoreObject( ApplyCommonItemParameters( definedAction ), "ShiftAction" );
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
			ApplyCommonItemParameters( definedAction );
			definedAction->Percentage( percentage );
			definedAction->LedProvider( storage.Led );
			StoreObject( definedAction, "FadeAction" );
		}
	};

	/// @brief Parse a TriggerAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeTriggerAction()
	{
		StoreObject( ApplyCommonItemParameters( new TriggerAction() ), "TriggerAction" );
	};

	/// @brief Parse a Block definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeBlock()
	{
		// There should be at least one ExecutableItem
		if ( storage.CheckObjectsType<ExecutableItem>() == true )
		{
			Block* definedBlock = new Block();

			for ( BaseDefinedObject* object : storage.Objects )
			{
				definedBlock->AddItem( static_cast<ExecutableItem*>( object )->Clone() );
			}

			StoreObject( ApplyCommonItemParameters( definedBlock ), "Block" );
		}
		else
		{
			ReportError( "Either no items or invalid items for Block %", storage.Name );
		}
	}

	/// @brief Apply the common executable item parameters already found to the item
	/// @param error
	/// @return
	ExecutableItem* ObjectParsers::ApplyCommonItemParameters( ExecutableItem* item )
	{
		item->Name( storage.Name );
		item->Counter( storage.Count );
		item->ExecutionTime( storage.Time );
		item->PostDelay( storage.Delay );
		item->Segment( storage.Segment );
		item->While( storage.While );
		item->ItemSynch( GetStoredSynchType( ExecutableItem::sequential, storage.ExecutionType ) );
		item->DefaultSegment( static_cast<SegmentProvider*>( objectStorage.GetObject( "fullSegment" ) ) );

		return item;
	}
}