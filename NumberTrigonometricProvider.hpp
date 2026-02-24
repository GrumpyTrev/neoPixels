#pragma once
#include "NumberProvider.hpp"

namespace Lights
{
    /// @brief A provider to supply a sequence of sine or cosine numbers. 
    ///        The wavelength and an optional start provider can be specified.
    ///        This is an abstract class, the actual values are supplied by the derived classes accessing different 
    ///        lookup tables, or with different offsets.
    class NumberTrigonometricProvider : public NumberProvider
    {
    public:
        inline NumberTrigonometricProvider() : NumberProvider( 0 ) {};

        inline void Next()
        {
            // Get the next value. Note that this will wrap around at 256
            index += interval;
            SetValue( AccessLookupTable( index ) );
        }

        /// @brief Reset the provider
        inline void Reset()
        {
            // Initialise the index. This will just take the lowest 8 bits of any startProvider value
            index = ( startProvider == nullptr ) ? 0 : startProvider->Value();
            SetValue( AccessLookupTable( index ) );
        }

        /// @brief Configure the provider
        /// @param waveLength 
        /// @param start 
        inline void Configure( uint8_t waveLength, NumberProvider* start )
        {
            startProvider = start;

            // The interval is 256 / waveLength
            interval = 256 / waveLength;

            Reset();
        }

    protected:

        virtual int32_t AccessLookupTable( uint8_t index ) = 0;

        /// @brief The interval between supplied sine values
        uint8_t interval;

        /// @brief The index into the sine table of the last value
        uint8_t index = 0;

        /// @brief Optional start provider
        NumberProvider* startProvider = nullptr;

        /* A table containing 8-bit unsigned sine wave (0-255).
        Copy & paste this snippet into a Python REPL to regenerate:
        import math
        for x in range(256):
            print("{:3},".format(int((math.sin(x/128.0*math.pi)+1.0)*127.5+0.5))),
            if x&15 == 15: print
        */
        const uint8_t SineTable[ 256 ] = {
            128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170,
            173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211,
            213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240,
            241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
            254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
            250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232,
            230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198,
            196, 193, 190, 188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155,
            152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109,
            106, 103, 100, 97,  93,  90,  88,  85,  82,  79,  76,  73,  70,  67,  65,
            62,  59,  57,  54,  52,  49,  47,  44,  42,  40,  37,  35,  33,  31,  29,
            27,  25,  23,  21,  20,  18,  17,  15,  14,  12,  11,  10,  9,   7,   6,
            5,   5,   4,   3,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,   0,
            0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,   10,  11,
            12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,  37,
            40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,
            79,  82,  85,  88,  90,  93,  97,  100, 103, 106, 109, 112, 115, 118, 121,
            124 };

        const int8_t SignedSineTable[ 256 ] = {
            0,   3,   6,   9,   12,  15,  18,  21,  24,  27,  30,  33,  36,  39,  42,  45,
            48,  51,  54,  57,  59,  62,  65,  67,  70,  73,  75,  78,  80,  82,  85,  87,
            89,  91,  94,  96,  98,  100, 102, 103, 105, 107, 108, 110, 112, 113, 114, 116,
            117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 126,
            127, 126, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118,
            117, 116, 114, 113, 112, 110, 108, 107, 105, 103, 102, 100, 98,  96,  94,  91,
            89,  87,  85,  82,  80,  78,  75,  73,  70,  67,  65,  62,  59,  57,  54,  51,
            48,  45,  42,  39,  36,  33,  30,  27,  24,  21,  18,  15,  12,  9,   6,   3,
            0,  -3,  -6,  -9,  -12, -15, -18, -21, -24, -27, -30, -33, -36, -39, -42, -45,
           -48, -51, -54, -57, -59, -62, -65, -67, -70, -73, -75, -78, -80, -82, -85, -87,
           -89, -91, -94, -96, -98, -100,-102,-103,-105,-107,-108,-110,-112,-113,-114,-116,
           -117,-118,-119,-120,-121,-122,-123,-123,-124,-125,-125,-126,-126,-126,-126,-126,
           -127,-126,-126,-126,-126,-126,-125,-125,-124,-123,-123,-122,-121,-120,-119,-118,
           -117,-116,-114,-113,-112,-110,-108,-107,-105,-103,-102,-100,-98, -96, -94, -91,
           -89, -87, -85, -82, -80, -78, -75, -73, -70, -67, -65, -62, -59, -57, -54, -51,
           -48, -45, -42, -39, -36, -33, -30, -27, -24, -21, -18, -15, -12, -9,  -6,  -3
        };
    };

    /// @brief A provider to supply a sequence of uint8_t sine numbers. 
    class NumberSineProvider : public NumberTrigonometricProvider
    {
    public:

    protected:
        virtual int32_t AccessLookupTable( uint8_t index ) override { return SineTable[ index ]; }
    };

    /// @brief A provider to supply a sequence of int8_t sine numbers. 
    class NumberSignedSineProvider : public NumberTrigonometricProvider
    {
    public:

    protected:
        virtual int32_t AccessLookupTable( uint8_t index ) override { return SignedSineTable[ index ]; }
    };

    /// @brief A provider to supply a sequence of uint8_t cosine numbers. 
    class NumberCosineProvider : public NumberTrigonometricProvider
    {
    public:

    protected:
        virtual int32_t AccessLookupTable( uint8_t index ) override { return SineTable[ uint8_t( index + 64 ) ]; }
    };

    /// @brief A provider to supply a sequence of int8_t cosine numbers. 
    class NumberSignedCosineProvider : public NumberTrigonometricProvider
    {
    public:

    protected:
        virtual int32_t AccessLookupTable( uint8_t index ) override { return SignedSineTable[ uint8_t( index + 64 ) ]; }
    };
}