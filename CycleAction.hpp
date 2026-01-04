#pragma once
#include <vector>
#include "Action.hpp"

namespace Lights
{
	class CycleAction : public Action
	{
	public:
		/// @brief Explicit default constructor
		inline CycleAction() {}

		/// @brief Copy constructor
		/// @param rhs
		inline CycleAction(const CycleAction &rhs) : Action(rhs)
		{
			providers = rhs.providers;
		};

		/// @brief Clone the item.
		/// @return
		inline CycleAction *Clone() const { return new CycleAction(*this); }

		inline void AddProvider(ProviderCycleInterface *provider) { providers.push_back(provider); }

	protected:
		inline void Execute()
		{
			for (auto &provider : providers)
			{
				provider->Next();
			}
		};

	private:
		std::vector<ProviderCycleInterface *> providers;
	};
}