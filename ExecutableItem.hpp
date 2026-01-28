#pragma once

#include "NumberProvider.hpp"
#include "SegmentProvider.hpp"
#include <pico/types.h>
#include <map>

namespace Lights
{
	class ExecutableItem
	{
	public:
		/// @brief Explicit default constructor
		inline ExecutableItem() {}

		/// @brief Copy constructor
		/// @param rhs
		ExecutableItem(const ExecutableItem &rhs);

		/// @brief Initialise the Item prior to execution
		void Initialise(SegmentProvider *segmentToApply);

		/// @brief Execute the item
		/// This must be implemented by any derived classes
		/// @param count
		/// @return True if a post execution delay is required
		virtual bool Execute( uint16_t count ) = 0;

		/// @brief Clone the item.
		/// This must be implemented by any derived classes
		/// @return
		virtual ExecutableItem *Clone() const = 0;

		/// @brief Set the providers
		/// @param provider
		inline void Counter(NumberProvider *provider) { counter = provider; }
		inline void ExecutionTime(NumberProvider *provider) { executionTimer = provider; }
		inline void PostDelay(NumberProvider *provider) { postDelayer = provider; }

		/// @brief What type of parallelism does this Item support
		enum SynchType
		{
			/// @brief This is a sequential item
			sequential,
			/// @brief This is a parallel item but with no synchronisation
			parallel,
			/// @brief This is a parallel item that is synchronised to start when the sequential items
			/// start execution
			parallelStart,
			/// @brief This is a parallel item that is synchronised to end when the sequential items
			/// have finished execution
			parallelEnd,
			/// @brief This is a parallel item that is synchronised to start and end when the sequential
			/// items start and stop execution
			parallelBoth
		};

		static bool TypeFromString(string fromString, SynchType &type);

		/// @brief Access the parallelism of this Item
		/// @return
		inline SynchType ItemSynch() { return itemSynch; }
		inline void ItemSynch(SynchType type) { itemSynch = type; }

		/// @brief Is this item to be executed in parallel with other items
		/// @return
		inline bool IsParallel() { return itemSynch != sequential; }

		/// @brief The next time that this Executable should run
		/// @return
		inline absolute_time_t DelayTime() { return delayTime; }
		inline void DelayTime(absolute_time_t delay) { delayTime = delay; }

		/// @brief Provide access to the SegmentProvider associated with the Item
		/// @return
		inline SegmentProvider *Segment() { return itemSegment; }
		inline void Segment(SegmentProvider *sequenceToUse) { itemSegment = sequenceToUse; }

		/// @brief Set the segment to use if non made explicit
		/// @param segment 
		inline void DefaultSegment( SegmentProvider* segment ) { defaultSegment = segment; }

	protected:
		/// @brief Specific initialisation required for all derived classes
		virtual void InitialiseItem() = 0;

		/// @brief Optional one-off initialisation
		inline virtual void OneOffInitialisation() {};

		/// @brief Optional providers for post-operation delay, execution time and count
		NumberProvider *postDelayer = nullptr;
		NumberProvider *executionTimer = nullptr;
		NumberProvider *counter = nullptr;

		/// @brief Flag controlling one-off initialisation
		bool initialised = false;

		/// @brief The time at which execution of this Item should be terminated
		absolute_time_t timeLimit;

		/// @brief What type of parallelism does this Item support
		SynchType itemSynch = sequential;

		/// @brief A counter used to repeat this Item the required number of times
		uint16_t operationCount = 0;

		/// @brief The time this Item should run again
		absolute_time_t delayTime;

		/// @brief The SegmentProvider used by the Item
		SegmentProvider *itemSegment = nullptr;

		/// @brief The SegmentProvider to be used if no segemnt is explicitly provided
		SegmentProvider* defaultSegment = nullptr;

	private:
		/// @brief Mapping from parallelism string to type
		static std::map<string, SynchType> parallelMap;
	};

	string TimeDisplay();
}