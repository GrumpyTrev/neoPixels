#pragma once
#include "ProviderBase.hpp"
#include "Segment.hpp"

namespace Lights
{
	/// @brief Class used to wrap up a Segment in a provider
	class SegmentProvider : public ProviderBase
	{
	public:
		inline SegmentProvider( Segment* initial ) : providedValue( initial ) {};
		inline virtual Segment* Value() { return providedValue; }
		inline virtual Segment* GetValue() { return providedValue; }
		inline virtual void SetValue( Segment* value ) { providedValue = value; }

	protected:
		Segment* providedValue = nullptr;
	};
}