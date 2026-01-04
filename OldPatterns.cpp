int pixelQueue = 0;
uint8_t colourCycle = 0;

void pattern_snakes(LedSegment *seg, uint t)
{
	for (uint i = 0; i < seg->NumLeds(); ++i)
	{
		uint x = (i + (t >> 1)) % 64;
		if (x < 10)
			seg->SetPixelColour(i, red);
		else if (x >= 15 && x < 25)
			seg->SetPixelColour(i, green);
		else if (x >= 30 && x < 40)
			seg->SetPixelColour(i, blue);
		else
			seg->SetPixelColour(i, black);
	}
	seg->Show();
}

void pattern_random(LedSegment *seg, uint t)
{
	if (t % 8)
		return;
	for (int i = 0; i < seg->NumLeds(); ++i)
		seg->SetPixelColour(i, Colour(rand()));
	seg->Show();
}

void pattern_sparkle(LedSegment *seg, uint t)
{
	if (t % 8)
		return;
	for (int i = 0; i < seg->NumLeds(); ++i)
		seg->SetPixelColour(i, Colour(rand() % 16 ? 0 : 0xffffff));
	seg->Show();
}

void pattern_greys(LedSegment *seg, uint t)
{
	int max = 100; // let's not draw too much current!
	t %= max;
	for (int i = 0; i < seg->NumLeds(); ++i)
	{
		seg->SetPixelColour(i, Colour(t * 0x10101));
		if (++t >= max)
			t = 0;
	}
	seg->Show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
Colour Wheel(uint8_t WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return Colour(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170)
	{
		WheelPos -= 85;
		return Colour(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Colour(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(LedSegment *seg, uint t)
{
	// Clear the pixels set last time
	for (int i = 0; i < seg->NumLeds(); i += 3)
	{
		seg->SetPixelColour(i + pixelQueue, black); //  Set pixel's color (in RAM)
	}

	// Set the next set of pixels
	pixelQueue++;
	if (pixelQueue >= 3)
		pixelQueue = 0;

	for (int i = 0; i < seg->NumLeds(); i += 3)
	{
		seg->SetPixelColour(i + pixelQueue, Wheel(colourCycle)); //  Set pixel's color (in RAM)
	}

	seg->Show(); //  Update strip to match

	colourCycle++;
}

typedef void (*pattern)(LedSegment *seg, uint t);
const struct
{
	pattern pat;
	const char *name;
	uint delay;
} pattern_table[] = {
	//	{pattern_snakes, "Snakes!", 10},
	//	{pattern_random, "Random data", 10},
	//	{pattern_sparkle, "Sparkles", 10},
	//	{pattern_greys, "Greys", 10},
	{theaterChase, "Theatre", 200},
};
