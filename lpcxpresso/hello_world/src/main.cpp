// A basic hello world example using the ezlpc library. Text output is sent to
// the USB serial port (CDC virtual com). 

#include "ezlpc.h"
#include "io_pins.h"
#include "passive_timer.h"
#include "system_time.h"
#include "usb_serial.h"

// Timer for generating the inter-message delay.
static PassiveTimer timer;

// LED at port 0, pin 7.
static io_pins::OutputPin led(0, 7);

static void setup() {
  ezlpc::setup();
  // Uses timer32_0 for generating a 1 usec 32 bit clock (does not use interrupts).
  system_time::setup();
  // Initialize the USB serial connection. This will allow us to print messages.
  usb_serial::setup();

  // Reset the timer to time now. This starts the first cycle.
  timer.reset();
}

static void loop() {
  static int message_count = 0;
  const uint32 time_now_usecs = timer.usecs();
  // Generates a short blink at the beginning of each cycle.
  led.set(time_now_usecs <= 500000);
  if (time_now_usecs >= 1000000) {
    usb_serial::printf("Hello world: %d, %u\n", message_count++,
        system_time::usecs());
    // Advance cycle start time rather than reseting to time now. This
    // way we don't accumulate time errors.
    timer.advance_start_time_usecs(1000000);
  }
}

int main(void) {
  setup();
  for (;;) {
    loop();
  }
}