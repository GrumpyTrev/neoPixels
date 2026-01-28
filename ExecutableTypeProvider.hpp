#pragma once
#include "ExecutableItem.hpp"

namespace Lights
{
	/// @brief Class used to wrap up a ExecutableItem.SynchType in a provider
	class ExecutableTypeProvider : public ProviderBase
	{
	public:
		inline ExecutableTypeProvider( ExecutableItem::SynchType synchType ) : providedValue( synchType ) {};
		inline virtual ExecutableItem::SynchType Value() { return providedValue; }
		inline virtual ExecutableItem::SynchType GetValue() { return providedValue; }
		inline virtual void SetValue( ExecutableItem::SynchType value ) { providedValue = value; }

	protected:
		ExecutableItem::SynchType providedValue = ExecutableItem::sequential;
	};
}