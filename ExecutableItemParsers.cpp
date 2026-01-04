#include <iostream>
#include "ObjectParsers.hpp"
#include "ObjectStore.hpp"
#include "Action.hpp"
#include "FillAction.hpp"
#include "SetAction.hpp"
#include "ShiftAction.hpp"
#include "CycleAction.hpp"
#include "Block.hpp"

using namespace std;

namespace Lights
{
	/// @brief Parse a FillAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeFillAction()
	{
		FillAction *definedAction = new FillAction();

		// Check for fill colour parameter
		if (storage.ItemColourProvider == nullptr)
		{
			errorStream << "No colour for FillAction " << storage.Name;
		}
		else
		{
			ApplyCommonItemParameters(definedAction);
			definedAction->Provider((ColourProvider *)(storage.ItemColourProvider));
			StoreObject(definedAction, "FillAction");
		}
	};

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

	/// @brief Parse a CycleAction definition
	/// @param tokens
	/// @return
	void ObjectParsers::MakeCycleAction()
	{
		CycleAction *definedAction = new CycleAction();

		// There should be at least one provider
		if ((storage.Objects.size() > 0) &&
			(storage.CheckObjectsType<ProviderCycleInterface>() == true))
		{
			ApplyCommonItemParameters(definedAction);

			for (BaseDefinedObject *object : storage.Objects)
			{
				definedAction->AddProvider((ProviderCycleInterface *)(object));
			}

			StoreObject(definedAction, "CycleAction");
		}
		else
		{
			errorStream << "Either no providers or invalid providers for CycleAction " << storage.Name;
		}
	}

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
	}
}