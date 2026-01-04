#pragma once
#include "Colour.hpp"
#include "SimpleProvider.hpp"

namespace Lights
{
	/// @brief A provider to supply a single fixed colour
	class ColourProvider : public BaseProvider<Colour>
	{
	public:
		inline ColourProvider(Colour colour) { providerImplementation = new SimpleProvider<Colour>(colour); }

	protected:
		inline ColourProvider() {}
	};
}