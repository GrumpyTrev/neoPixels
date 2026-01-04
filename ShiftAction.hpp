#pragma once
#include "Action.hpp"

namespace Lights
{
	class ShiftAction : public Action
	{
	public:
		/// @brief Explicit default constructor
		inline ShiftAction() {}

		/// @brief Copy constructor
		/// @param rhs
		inline ShiftAction(const ShiftAction &rhs) : Action(rhs) {};

		/// @brief Clone the item.
		/// @return
		inline ShiftAction *Clone() const { return new ShiftAction(*this); }

	protected:
		inline void Execute() { itemSegment->Value()->Shift(); };
	};
}