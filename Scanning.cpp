#include "Scanning.hpp"

namespace Lights
{
	/// @brief Run a block of pixels back and forth
	/// @param segment
	/// @return
	uint Scanning::Scan(LedSegment *segment)
	{
		return Scan(segment, segment->GetColour(0), segment->GetColour(1), false);
	}

	/// @brief Run two blocks of pixels back and forth
	/// @param segment
	/// @return
	uint Scanning::ScanDual(LedSegment *segment)
	{
		return Scan(segment, segment->GetColour(0), segment->GetColour(1), true);
	}

	/// @brief Run a block of pixels back and forth, allowing the block to fully disappear at both ends
	/// @param segment
	/// @return
	uint Scanning::ScanFull(LedSegment *segment)
	{
		return Scan(segment, segment->GetColour(0), segment->GetColour(1), false, true);
	};

	/// @brief Run two blocks of pixels back and forth, allowing the block to fully disappear at both ends
	/// @param segment
	/// @return
	uint Scanning::ScanDualFull(LedSegment *segment)
	{
		return Scan(segment, segment->GetColour(0), segment->GetColour(1), true, true);
	};

	/// @brief Run a block of pixels back and forth, the colour of the 'set' pixels is randwomly set every
	/// double pass
	/// @param segment
	/// @return
	uint Scanning::ScanRandom(LedSegment *segment)
	{
		return ScanInternalRandom(segment, false);
	};

	uint Scanning::ScanDualRandom(LedSegment *segment)
	{
		return ScanInternalRandom(segment, true);
	};

	/// @brief Run a block of pixels back and forth, the colour of the 'set' pixels is randwomly set every
	/// double pass
	/// @param segment
	/// @return
	uint Scanning::ScanInternalRandom(LedSegment *segment, bool dual)
	{
		// Is it time to specify a new colour
		if (segment->StepCount() == 0)
		{
			// AuxParam will store the random color wheel index
			segment->SetAuxParam(AnimationHelper::RandomColourWheelIndex(segment->AuxParam()));
		}

		// Use the stored index to select the colour to run with
		Colour runColour = AnimationHelper::ColourWheel(segment->AuxParam());

		return Scan(segment, runColour, segment->GetColour(1), dual, true);
	};

	/// @brief Run one or two blocks of pixels back and forth
	/// @param segment
	/// @param colour1 The foreground running colour
	/// @param colour2 The background colour
	/// @param dual
	/// @return
	uint Scanning::Scan(LedSegment *segment, Colour colour1, Colour colour2, bool dual, bool full)
	{
		// The current direction is held in the top bit of Options()
		int8_t dir = (segment->Options() & 0x80) > 0 ? -1 : 1;

		// The number of pixels in the block is given in the lower 7 bits of Options()
		uint8_t size = 1 << (segment->Options() & 0x7f);

		// Cache the step count and Led count
		uint32_t step = segment->StepCount();
		uint numLeds = segment->NumLeds();

		// Set all pixels to the background colour
		segment->Fill(colour2);

		// What's the index of the first pixel to set
		int firstPixel = (full == false) ? step : step - size;

		// Set 'size' pixels from StepCount to the running colour
		for (int index = firstPixel; index < firstPixel + size; index++)
		{
			// When 'full' is true the index may be set outside of the bounds of the strip
			// So check here
			if ((index >= 0) && (index < numLeds))
			{
				segment->SetPixelColour(index, colour1);

				if (dual == true)
				{
					// Set the other block as well
					segment->SetPixelColour(numLeds - 1 - index, colour1);
				}
			}
		}

		// Adjust the position for the block next time
		step += dir;
		segment->SetStepCount(step);

		// Toggle the direction if reached the start or end of the strip
		if (step == 0)
		{
			segment->SetOptions(segment->Options() & 0x7f);
		}
		else if (((full == false) && (step >= (numLeds - size))) ||
				 ((full == true) && (step >= (numLeds + size))))
		{
			segment->SetOptions(segment->Options() | 0x80);
		}

		return segment->Speed() / (numLeds * 2);
	}
}