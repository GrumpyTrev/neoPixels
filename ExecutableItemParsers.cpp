#include <iostream>
#include "ObjectParsers.hpp"
#include "ObjectStore.hpp"
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
		SetAction *definedAction = new SetAction();

		// Check for fill colour parameter
		if (storage.ItemColourProvider == nullptr)
		{
			errorStream << "No colour for SetAction " << storage.Name;
		}
		else
		{
			ApplyCommonItemParameters(definedAction);
			definedAction->Provider((ColourProvider *)(storage.ItemColourProvider));

			// Get the pixel number from the optional provider. Leave the PixelProvider as null if
			// no  number is specified
			definedAction->PixelProvider((NumberProvider *)storage.StartLedProvider );

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
		FadeAction *definedAction = new FadeAction();

		// Check for fade percentage parameter (stored in the IntervalProvider)
		if (storage.IntervalProvider == nullptr)
		{
			errorStream << "No fade percentage for FadeAction " << storage.Name;
		}
		else
		{
			ApplyCommonItemParameters(definedAction);

			definedAction->Percentage( ( (NumberProvider *)storage.IntervalProvider )->GetValue());
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
		Block *definedBlock = new Block();

		// There should be at least one ExecutableItem
		if ((storage.Objects.size() > 0) &&
			(storage.CheckObjectsType<ExecutableItem>() == true))
		{
			ApplyCommonItemParameters(definedBlock);

			for (BaseDefinedObject *object : storage.Objects)
			{
				definedBlock->AddItem(dynamic_cast<ExecutableItem *>(object)->Clone());
			}

			StoreObject(definedBlock, "Block");
		}
		else
		{
			errorStream << "Either no items or invalid items for Block " << storage.Name;
		}
	}

	/// @brief Apply the common executable item parameters already found to the item
	/// @param error
	/// @return
	void ObjectParsers::ApplyCommonItemParameters(ExecutableItem *item)
	{
		dynamic_cast<BaseDefinedObject *>(item)->Name(storage.Name);
		item->Counter((NumberProvider *)(storage.CountProvider));
		item->ExecutionTime((NumberProvider *)(storage.TimeProvider));
		item->PostDelay((NumberProvider *)(storage.DelayProvider));
		item->Segment((SegmentProvider *)(storage.SegmentProvider));
		item->ItemSynch(GetStoredSynchType(ExecutableItem::sequential, storage.ItemTypeProvider));
		item->DefaultSegment( dynamic_cast<SegmentProvider*>( objectStorage.GetObject( "fullSegment" ) ) );
	}
}