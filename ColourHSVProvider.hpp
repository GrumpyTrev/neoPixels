#pragma once
#include "ColourProvider.hpp"
#include "NumberProvider.hpp"

// TESTING
#include <iostream>

namespace Lights
{
    /// @brief The ColourHSVProvider class provides a Colour based on the suppied hue, value and saturation values
    ///        The hue can be in the range 0 - 65535. If a number of steps has been specified then supplied hue value
    ///        is the step number and is scaled up to be in the range 0 - 65535
    class ColourHSVProvider : public ColourProvider
	{
	public:
        inline ColourHSVProvider( NumberProvider* hue, NumberProvider* sat, NumberProvider* value, uint16_t steps ) :
            hueProvider( hue ), satProvider( sat ), valProvider( value ), numberIntervals( steps ), ColourProvider( Colour::InvalidColour ) {
        };

		/// @brief Supply the next number
		inline virtual void Next()
		{
            uint16_t hue = hueProvider->Value();

            if ( numberIntervals > 0 )
            {
                uint16_t stepNumber = hue % numberIntervals;
                hue = uint32_t( stepNumber << 16 ) / numberIntervals;
            }

            uint8_t sat = satProvider->Value();
            uint16_t value = valProvider->Value();
            SetValue( Colour::ColourHSV( hue, sat, value ) );

            // cout << "ColourHSVProvider " << hue << " " << (uint16_t)sat << " " << value << "\n";
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

        /// @brief The number of steps around the HSV cone to provide
        uint16_t numberIntervals = 0;
    };
}