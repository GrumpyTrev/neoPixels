#pragma once
#include "ExecutableItem.hpp"

namespace Lights
{
	/// @brief Class used to wrap up a ExecutableItem.SynchType in a provider
	class ExecutableTypeProvider : public BaseProvider<ExecutableItem::SynchType>
	{
	public:
		inline ExecutableTypeProvider(ExecutableItem::SynchType synchType)
		{
			providerImplementation = new SimpleProvider<ExecutableItem::SynchType>(synchType);
		}

	protected:
		inline ExecutableTypeProvider() {};
	};
}