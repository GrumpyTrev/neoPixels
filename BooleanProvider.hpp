#pragma once
#include "ProviderBase.hpp"

namespace Lights
{
	/// @brief Class used to wrap up a boolean in a provider
	class BooleanProvider : public ProviderBase
	{
	public:
		inline BooleanProvider( bool initial ) : providedValue( initial ) {};
		inline virtual bool Value() { return providedValue; }
		inline virtual bool GetValue() { return providedValue; }
		inline virtual void SetValue( bool value ) { providedValue = value; }

	protected:
		bool providedValue = false;
	};
}