#pragma once
#include "ColourProvider.hpp"
#include "NumberProvider.hpp"

// TESTING
#include <iostream>

namespace Lights
{
	class ColourHSVProvider : public ColourProvider
	{
	public:
		inline ColourHSVProvider( NumberProvider* hue, NumberProvider* sat, NumberProvider* value) : 
            hueProvider(hue), satProvider(sat), valProvider(value), ColourProvider(Colour::InvalidColour) {};

		/// @brief Supply the next number
		inline virtual void Next()
		{
            uint hue = hueProvider->Value();
            uint sat = satProvider->Value();
            uint value = valProvider->Value();
            SetValue( Colour::ColourHSV( hue, sat, value ) );

			//cout << "ColourHSVProvider " << hue << " " << sat << " " << value << "\n";
		}

        /// @brief Return the current colour
        /// @return
        inline virtual Colour Value()
        {
            // Make sure to get a colour if this provider has not been initialised yet
            if (GetValue().value == Colour::InvalidColour.value)
            {
                Next();
            }

            return ColourProvider::Value();
        };

	protected:
		/// @brief Hue provider
		NumberProvider* hueProvider;

		/// @brief Saturation number
		NumberProvider* satProvider;

        /// @brief Value provider
        NumberProvider* valProvider;
	};
}