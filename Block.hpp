#pragma once
#include "ExecutableItem.hpp"
#include <vector>

namespace Lights
{
	/// @brief Container class for Actions and Blocks that can be performed on an LedSequence
	class Block : public ExecutableItem
	{
	public:
		/// @brief Explicit default constructor
		inline Block() {}

		/// @brief Copy constructor
		/// @param rhs
		inline Block( const Block& rhs ) : ExecutableItem( rhs )
		{
			sequenceItems.reserve(rhs.sequenceItems.size());
			for (auto *elem : rhs.sequenceItems)
			{
				sequenceItems.push_back(elem->Clone());
			}

			parallelItems.reserve(rhs.parallelItems.size());
			for (auto *elem : rhs.parallelItems)
			{
				parallelItems.push_back(elem->Clone());
			}
		};

		/// @brief Clone the item.
		/// @return
		inline Block *Clone() const { return new Block(*this); }

		/// @brief Execute the Block
		/// @param count
		bool Execute( uint16_t count );

		/// @brief Add an ExecutableItem to either the sequential or parallel lists
		/// @param itemToAdd
		void AddItem(ExecutableItem *itemToAdd);

	protected:
		/// @brief Initialise the Block
		inline void InitialiseItem() { state = Idle; }

	private:
		/// @brief Called when a Block is first executed.
		void FirstExecution();

		/// @brief Run any items in the runnable list
		void ExecuteRunnableItems();

		/// @brief Called when all the sequential items have been executed
		void EndOfSequence();

		/// @brief Find the runnable item with the shortest delay.
		ExecutableItem* ItemWithShortestDelay( uint16_t& index );

		/// @brief Find the runnable item with the shortest delay.
		ExecutableItem *ItemWithShortestDelay();

		/// @brief Find the next runnable Item. This is the Item with the shortest
		/// delay whose delay time has already elapsed
		ExecutableItem *NextRunnableItem();

		/// @brief Remove from the runnable list any parallel items that are
		/// synchronised with the end of the sequence
		void TerminateEndSynchronisedItems();

		/// @brief Execute any parallel items that are synchronised with the start of the sequence
		void ExecuteStartSynchronisedItems();

		/// @brief Run an ExecutableItem for the first time
		/// @param itemToRun
		void RunParallelItem(ExecutableItem *itemToRun);

		/// @brief Initialise and then schedule a sequential item
		/// @param itemToRun
		void ScheduleSequentialItem( ExecutableItem* itemToRun );

		/// @brief Determine the type of control used for this block
		void DetermineControlType();

		/// @brief Output the current execution state
		/// @return 
		inline string StateToString()
		{
			return ( state == Idle ) ? "Idle" : ( state == Running ) ? "Running" : "Terminating";
		}

		/// @brief The Items to run in sequence
		vector<ExecutableItem *> sequenceItems;

		/// @brief The Items to run in parallel
		vector<ExecutableItem *> parallelItems;

		/// @brief The Items currently running
		vector<ExecutableItem *> runningItems;

		/// @brief The index of the sequential items currently being executed
		uint16_t sequentialItemIndex = 0;

		/// @brief The count limit for this block, determined at first execution, defaults to 1
		uint16_t countLimit = 1;

		/// @brief The states that this block cen be in
		enum BlockStateType
		{
			Idle,
			Running,
			Terminating
		};

		/// @brief The execution state of this block
		BlockStateType state = Idle;

		/// @brief The types of execution control
		enum ControlType
		{
			While,
			Timer,
			Count
		};

		/// @brief The type of execution control
		ControlType control = Count;
	};
}