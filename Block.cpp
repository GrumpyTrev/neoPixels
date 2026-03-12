#include "Block.hpp"
#include <pico/time.h>

#include <iostream>

using namespace std;

namespace Lights
{
	/// @brief Execute the Block
	/// @param count
	bool Block::Execute( uint16_t count )
	{
		if ( TraceOn() == true )
		{
			cout << "Block " << Name() << " entry " << StateToString() << '\n';
		}

		// Processing is controlled by the execution state of the block
		if ( state == Terminating )
		{
			state = Idle;
		}
		else
		{
			if ( state == Idle )
			{
				FirstExecution();
			}

			// The state may have changed to Running, so have a separate if here
			if ( state == Running )
			{
				// Run any items that can be run at this time
				ExecuteRunnableItems();

				// If there is an item to execute then use its delay time as the delay time for this block
				ExecutableItem* nextItemToRun = ItemWithShortestDelay();
				if ( nextItemToRun != nullptr )
				{
					delayTime = nextItemToRun->DelayTime();
				}
				else
				{
					// If this Block has a post-operation delay then use it now
					if ( postDelayer != nullptr )
					{
						delayTime = make_timeout_time_ms( postDelayer->Value() );

						state = Terminating;
					}
					else
					{
						state = Idle;
					}
				}
			}
		}

		if ( TraceOn() == true )
		{
			cout << "Block " << Name() << " exit " << StateToString() << '\n';
		}

		return ( state != Idle );
	}

	/// @brief Called when a Block is first executed.
	void Block::FirstExecution()
	{
		// Make sure there are no running items left over from when this block was last run
		runningItems.clear();

		// Reset the cycle count and the sequence index
		operationCount = 0;
		sequentialItemIndex = 0;

		// Determine the type of execution control being used
		DetermineControlType();

		// The parallel items are executed whatever type of control is being used
		for ( auto& parallelItem : parallelItems )
		{
			RunParallelItem( parallelItem );
		}

		// Only execute any sequential items if Count > 0 or While == true
		if ( sequenceItems.size() > 0 )
		{
			if ( ( ( control == Count ) && ( countLimit > 0 ) ) ||
				( ( control == While ) && ( whileProvider->Value() > 0 ) ) )
			{
				ScheduleSequentialItem( sequenceItems.at( sequentialItemIndex ) );
			}
		}

		// Change the state to Running, even if this may be changed due to there being no runnable items
		state = Running;
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

		if ( TraceOn() == true )
		{
			cout << "Block " << Name() << " end of sequence with operationCount " << operationCount << '\n';
		}

		// Check if execution should be terminated here
		if ( ( ( control == Count ) && ( operationCount == countLimit ) ) ||
			( ( control == While ) && ( whileProvider->Value() <= 0 ) ) ||
			( ( control == Timer ) && ( time_reached( timeLimit ) == true ) ) )
		{
			// Execution count exceeded. Clear the runnable items
			runningItems.clear();
		}
		else
		{
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
		uint16_t itemIndex = 0;
		return (ItemWithShortestDelay(itemIndex));
	}

	/// @brief Find the Item in the runnable list with the shortest delay.
	/// @return
	ExecutableItem* Block::ItemWithShortestDelay( uint16_t& selectedIndex )
	{
		ExecutableItem *selectedItem = nullptr;

		selectedIndex = 0;
		absolute_time_t minDelay = at_the_end_of_time;

		for ( uint16_t runningIndex = 0; runningIndex < runningItems.size(); ++runningIndex )
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
		uint16_t itemIndex = 0;
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
		// Use an int here as it may go negative
		for ( int index = runningItems.size() - 1; index >= 0; index-- )
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
		// Use an int here as it may go negative
		for ( int index = runningItems.size() - 1; index >= 0; index-- )
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

	/// @brief Work out from the supplied parameter the type of control used for this block
	void Block::DetermineControlType()
	{
		if ( whileProvider != nullptr )
		{
			// The While control is evaluated when required
			control = While;

			if ( TraceOn() == true )
			{
				cout << "Block " << Name() << " control is While" << '\n';
			}
		}
		else if ( counter != nullptr )
		{
			// The Count value is set at initialisation
			control = Count;
			countLimit = max( counter->Value(), int32_t( 0 ) );

			if ( TraceOn() == true )
			{
				cout << "Block " << Name() << " control is Count " << countLimit << '\n';
			}
		}
		else if ( executionTimer != nullptr )
		{
			// The timeLimit has been set by the base class
			control = Timer;

			if ( TraceOn() == true )
			{
				cout << "Block " << Name() << " control is Timer " << '\n';
			}
		}
	}
}