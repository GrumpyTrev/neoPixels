#pragma once
#include "Action.hpp"

namespace Lights
{
	class FillAction : public Action
	{
	public:
		/// @brief Explicit default constructor
		inline FillAction() {}

		/// @brief Copy constructor
		/// @param rhs
		inline FillAction(const FillAction &rhs) : Action(rhs) {};

		/// @brief Clone the item.
		/// @return
		inline FillAction *Clone() const { return new FillAction(*this); }

	protected:
		inline void Execute() { itemSegment->Value()->Fill(actionColourProvider->Value()); }
	};
}