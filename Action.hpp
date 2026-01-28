#pragma once
#include "ColourProvider.hpp"
#include "BaseDefinedObject.hpp"
#include "ExecutableItem.hpp"

namespace Lights
{
	/// @brief Abstract class for Actions that can be performed on an LedSequence
	class Action : public BaseDefinedObject, public ExecutableItem
	{
	public:
		/// @brief Explicit default constructor
		inline Action() {}

		/// @brief Copy constructor
		/// @param rhs
		Action(const Action &rhs);

		/// @brief Execute the Action
		/// @param count
		bool Execute( uint16_t count );

		/// @brief Allow the ColourProvider to be specified
		/// @param provider
		inline void Provider(ColourProvider *provider) { actionColourProvider = provider; }

	protected:
		/// @brief Execute the specialised operation associated with the Action
		virtual void Execute() = 0;

		/// @brief Perform specific initialisation for this item
		void InitialiseItem();

		/// @brief The ColourProvider that provides Colours to be used by the Action
		ColourProvider *actionColourProvider;

		/// @brief A counter that can be used by the Action to control its actions
		uint16_t stepCount = 0;
	};
}