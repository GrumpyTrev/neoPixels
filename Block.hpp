#pragma once
#include "BaseDefinedObject.hpp"
#include "ExecutableItem.hpp"
#include <vector>

namespace Lights
{
	/// @brief Container class for Actions and Blocks that can be performed on an LedSequence
	class Block : public BaseDefinedObject, public ExecutableItem
	{
	public:
		/// @brief Explicit default constructor
		inline Block() {}

		/// @brief Copy constructor
		/// @param rhs
		inline Block(const Block &rhs) : ExecutableItem(rhs), BaseDefinedObject(rhs)
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
		bool Execute(uint count);

		/// @brief Add an ExecutableItem to either the sequential or parallel lists
		/// @param itemToAdd
		void AddItem(ExecutableItem *itemToAdd);

	protected:
		/// @brief Initialise the Block
		inline void InitialiseItem() { firstExecution = true; }

	private:
		/// @brief Called when a Block is first executed.
		void FirstExecution();

		/// @brief Run any items in the runnable list
		void ExecuteRunnableItems();

		/// @brief Called when all the sequential items have been executed
		void EndOfSequence();

		/// @brief Find the runnable item with the shortest delay.
		ExecutableItem *ItemWithShortestDelay(uint &index);

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
		void ScheduleSequentialItem(ExecutableItem *itemToRun);

		/// @brief The Items to run in sequence
		vector<ExecutableItem *> sequenceItems;

		/// @brief The Items to run in parallel
		vector<ExecutableItem *> parallelItems;

		/// @brief The Items currently running
		vector<ExecutableItem *> runningItems;

		/// @brief Flag set to indicate that this is the first time this block has been executed
		bool firstExecution = true;

		/// @brief The index of the sequential items currently being executed
		uint sequentialItemIndex = 0;

		/// @brief Flag used to indicate when this block is using its post operational delay
		bool postOperationDelaying = false;
	};
}