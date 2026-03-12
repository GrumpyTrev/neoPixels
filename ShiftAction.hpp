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
		inline ShiftAction( const ShiftAction& rhs ) : Action( rhs ) { ledNumberProvider = rhs.ledNumberProvider; };

		/// @brief Clone the item.
		/// @return
		inline ShiftAction* Clone() const { return new ShiftAction( *this ); }

		/// @brief Allow the number of LEDs to shift to be specified
		/// @param provider
		inline void LedProvider( NumberProvider* provider ) { ledNumberProvider = provider; }

	protected:
		inline void Execute()
		{
			if ( ledNumberProvider == nullptr )
			{
				itemSegment->Value()->Shift();
			}
			else
			{
				// The ledNumberProvider has got to be at least 2 for a shift to work
				if ( ledNumberProvider->Value() > 1 )
				{
					itemSegment->Value()->Shift( ledNumberProvider->Value() );
				}
			}
		};

	private:

		/// @brief A NumberProvider used to provide the number of LEDs to shift
		NumberProvider* ledNumberProvider = nullptr;
	};
}