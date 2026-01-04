#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "ws2812Strip.hpp"
#include "Commander.hpp"

using namespace Lights;

#define NUM_PIXELS 50

#define WS2812_PIN 10
#define OUTPUT_ENABLE_PIN 11

const uint ResetGpio = 14;

/// @brief Called when the user reset line goes low.
/// This can sometime go low when the 5v power is switched on/off
/// so delay a little time and check the line is still low.
/// If so then perform the reset
void ResetInterruptHandler()
{
	printf("ResetInterruptHandler\n");
	busy_wait_ms(250);
	gpio_acknowledge_irq(ResetGpio, GPIO_IRQ_EDGE_FALL);
	if (gpio_get(ResetGpio) == 0)
	{
		printf("Reseting\n");
		reset_usb_boot(0, 0);
	}
}

int main()
{
	stdio_init_all();

	// Enable the level shifter output
	gpio_init(OUTPUT_ENABLE_PIN);
	gpio_set_dir(OUTPUT_ENABLE_PIN, GPIO_OUT);
	gpio_put(OUTPUT_ENABLE_PIN, 0);

	// Initialise the reset interrupt handling
	gpio_init(ResetGpio);
	gpio_set_dir(ResetGpio, GPIO_IN);
	gpio_pull_up(ResetGpio);

	gpio_add_raw_irq_handler(ResetGpio, ResetInterruptHandler);
	gpio_set_irq_enabled(ResetGpio, GPIO_IRQ_EDGE_FALL, true);
	irq_set_enabled(IO_IRQ_BANK0, true);

	// Initialise the strip of LEDs
	Lights::ws2812Strip leds(WS2812_PIN, NUM_PIXELS);
	leds.SetBrightness(10);
	leds.ApplyGamma(true);

	// The single command handler
	Commander command(&leds);
	command.Execute();
}
