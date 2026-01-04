#include "Block.hpp"
#include <pico/time.h>

#include <iostream>
#include "Trace.hpp"

using namespace std;

namespace Lights
{
	/// @brief Execute the Block
	/// @param count
	bool Block::Execute(uint count)
	{
		if (Trace::TraceOn() == true)
		{
			cout << TimeDisplay() << "Block " << Name() << " executing. Op count " << operationCount << "\n";
		}

		bool stillExecuting = false;

		// If this block has just finished its post operation delay then do nothing
		if (postOperationDelaying == true)
		{
			postOperationDelaying = false;
		}
		else
		{
			// If this is the first time the Block has been run then execute all the parallel items,
			// initialise the first sequential item and schedule it for execution
			if (firstExecution == true)
			{
				if (Trace::TraceOn() == true)
				{
					cout << TimeDisplay() << "Block " << Name() << " parallel " << parallelItems.size()
						 << " sequential " << sequenceItems.size() << "\n";
				}
				FirstExecution();
			}

			// If this Block has an execution time, check that the time has not expired
			if ((executionTimer == nullptr) || (time_reached(timeLimit) == false))
			{
				// Either no execution time or not reached yet.
				// Run any items that can be run at this time
				ExecuteRunnableItems();
			}
			else
			{
				// No more time for this Block to run. Clear the runnable items
				runningItems.clear();
			}

			// If there is an item to execute then use its delay time as the delay time for this block
			ExecutableItem *nextItemToRun = ItemWithShortestDelay();
			if (nextItemToRun != nullptr)
			{
				delayTime = nextItemToRun->DelayTime();

				stillExecuting = true;
			}
			else
			{
				// If this Block has a post-operation delay then use it now
				if (postDelayer != nullptr)
				{
					delayTime = make_timeout_time_ms(postDelayer->Value());

					// Keep track that this Block is in its post operation delay
					postOperationDelaying = true;

					stillExecuting = true;
				}
			}

			if (Trace::TraceOn() == true)
			{
				if (stillExecuting == true)
				{
					uint delayInMs = absolute_time_diff_us(get_absolute_time(), delayTime) / 1000;
					if (nextItemToRun != nullptr)
					{
						cout << TimeDisplay() << "Block " << Name() << " finished. Op count " << operationCount << " Item post delay " << delayInMs << "\n";
					}
					else
					{
						cout << TimeDisplay() << "Block " << Name() << " finished. Op count " << operationCount << " Block post delay " << delayInMs << "\n";
					}
				}
				else
				{
					cout << TimeDisplay() << "Block " << Name() << " finished. Op count " << operationCount << " No post delay\n";
				}
			}
		}

		return stillExecuting;
	}

	/// @brief Called when a Block is first executed.
	void Block::FirstExecution()
	{
		firstExecution = false;

		// Make sure there are no running items left over from when this block was last run
		runningItems.clear();

		// Reset the cycle count and the sequence index
		operationCount = 0;
		sequentialItemIndex = 0;

		// Run all the parallel items for the first time
		for (auto &parallelItem : parallelItems)
		{
			RunParallelItem(parallelItem);
		}

		// If there are any sequential items then pick the first one to run
		if (sequenceItems.size() > 0)
		{
			ScheduleSequentialItem(sequenceItems.at(sequentialItemIndex));
		}
	}

	/// @brief Run any items in the runnable list
	void Block::ExecuteRunnableItems()
	{
		// Execute all the Items whose delay time has been reached.
		// N.B. Multiple items may have the same execution time so put this in a loop
		ExecutableItem *itemToRun = NextRunnableItem();
		while (itemToRun != nullptr)
		{
			// Execute the Item. If it has not finished add it back to the executing list
			if (itemToRun->Execute(operationCount) == true)
			{
				runningItems.push_back(itemToRun);
			}
			else
			{
				// The item has finished executing.
				// If this is a sequential Item then move to the next sequential Item
				if (itemToRun->IsParallel() == false)
				{
					// Check if the end of the sequential items has been reached
					if (++sequentialItemIndex >= sequenceItems.size())
					{
						// End of the sequence.
						EndOfSequence();
					}
					else
					{
						// End of sequence not reached. Execute the next sequential item
						ScheduleSequentialItem(sequenceItems.at(sequentialItemIndex));
					}
				}
			}

			// Any more items to run at this time
			itemToRun = NextRunnableItem();
		}
	}

	/// @brief Called when all the sequential items have been executed
	void Block::EndOfSequence()
	{
		// End of the sequence. Terminate any parallel Items that are
		// Synchonised with the end of the sequence
		TerminateEndSynchronisedItems();

		// Reset the sequence index and increment the execution count
		sequentialItemIndex = 0;
		operationCount++;

		// If this is not a timed execution then check whether the execution count
		// has been exceeded
		uint countLimit = (counter != nullptr) ? counter->Value() : 1;

		if ((executionTimer == nullptr) && (operationCount >= countLimit))
		{
			// Execution count exceeded. Clear the runnable items
			runningItems.clear();
		}
		else
		{
			// If there is a timer then just reset the execution count when it is exceeded
			if ((executionTimer != nullptr) && (operationCount >= countLimit))
			{
				operationCount = 0;
			}

			// As this is the start of the sequence then run any parallel
			// items synchronised with the start of the sequence
			ExecuteStartSynchronisedItems();

			// Execute the sequential item
			ScheduleSequentialItem(sequenceItems.at(sequentialItemIndex));
		}
	}

	/// @brief Add an ExecutableItem to either the sequential or parallel lists
	/// @param itemToAdd
	void Block::AddItem(ExecutableItem *itemToAdd)
	{
		if (itemToAdd->IsParallel() == true)
		{
			parallelItems.push_back(itemToAdd);
		}
		else
		{
			sequenceItems.push_back(itemToAdd);
		}
	}

	/// @brief Run a parallel item for the first time
	/// @param itemToRun
	void Block::RunParallelItem(ExecutableItem *itemToRun)
	{
		// Initialise the Item when it is first run
		itemToRun->Initialise(itemSegment);

		// If the item has not finished add it to the executing list
		if (itemToRun->Execute(operationCount) == true)
		{
			runningItems.push_back(itemToRun);
		}
	}

	/// @brief Initialise and then schedule a sequential item
	/// @param itemToRun
	void Block::ScheduleSequentialItem(ExecutableItem *itemToRun)
	{
		// Initialise the item
		itemToRun->Initialise(itemSegment);

		// Set the item to run straight away and add it to the running items
		itemToRun->DelayTime(get_absolute_time());
		runningItems.push_back(itemToRun);
	}

	/// @brief Find the runnable item with the shortest delay.
	ExecutableItem *Block::ItemWithShortestDelay()
	{
		uint itemIndex = 0;
		return (ItemWithShortestDelay(itemIndex));
	}

	/// @brief Find the Item in the runnable list with the shortest delay.
	/// @return
	ExecutableItem *Block::ItemWithShortestDelay(uint &selectedIndex)
	{
		ExecutableItem *selectedItem = nullptr;

		selectedIndex = 0;
		absolute_time_t minDelay = at_the_end_of_time;

		for (uint runningIndex = 0; runningIndex < runningItems.size(); ++runningIndex)
		{
			ExecutableItem *runningItem = runningItems.at(runningIndex);
			if (to_us_since_boot(runningItem->DelayTime()) < to_us_since_boot(minDelay))
			{
				selectedItem = runningItem;
				selectedIndex = runningIndex;
				minDelay = runningItem->DelayTime();
			}
		}

		return selectedItem;
	}

	/// @brief Find the next runnable Item. This is the Item with the shortest
	/// delay whose delay time has already elapsed
	ExecutableItem *Block::NextRunnableItem()
	{
		uint itemIndex = 0;
		ExecutableItem *itemToRun = ItemWithShortestDelay(itemIndex);

		if ((itemToRun != nullptr) && (time_reached(itemToRun->DelayTime()) == true))
		{
			runningItems.erase(runningItems.begin() + itemIndex);
		}
		else
		{
			itemToRun = nullptr;
		}

		return itemToRun;
	}

	/// @brief Remove from the runnable list any parallel items that are
	/// synchronised with the end of the sequence
	void Block::TerminateEndSynchronisedItems()
	{
		for (int index = runningItems.size() - 1; index >= 0; index--)
		{
			ExecutableItem *item = runningItems.at(index);

			if ((item->ItemSynch() == parallelEnd) || (item->ItemSynch() == parallelBoth))
			{
				runningItems.erase(runningItems.begin() + index);
			}
		}
	}

	/// @brief Execute any parallel items that are synchronised with the start of the sequence
	void Block::ExecuteStartSynchronisedItems()
	{
		// First of all removed any start synchronised items from the running list
		for (int index = runningItems.size() - 1; index >= 0; index--)
		{
			ExecutableItem *item = runningItems.at(index);

			if ((item->ItemSynch() == parallelStart) || (item->ItemSynch() == parallelBoth))
			{
				runningItems.erase(runningItems.begin() + index);
			}
		}

		// Now run any start synchronised items
		for (auto &parallelItem : parallelItems)
		{
			if ((parallelItem->ItemSynch() == parallelStart) || (parallelItem->ItemSynch() == parallelBoth))
			{
				RunParallelItem(parallelItem);
			}
		}
	}
}