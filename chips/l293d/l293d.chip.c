// Rebuild (needs clang with the wasm32 target + wasm-ld, no wasi-libc required):
//   clang --target=wasm32 -nostdlib -O2 -Wno-unused-function -Wl,--import-memory \
//     -Wl,--export-table -Wl,--no-entry -Wl,-z,stack-size=4096 \
//     -o l293d.chip.wasm l293d.chip.c
//
// L293D quadruple half-H driver - simplified digital model for Wokwi.
//
// DIP-16 pinout (same order as the datasheet, pin 1 .. pin 16):
//   1 EN1 (1,2EN)  2 IN1 (1A)  3 OUT1 (1Y)  4 GND1   5 GND2   6 OUT2 (2Y)  7 IN2 (2A)  8 VCC2 (motor supply)
//   9 EN2 (3,4EN) 10 IN3 (3A) 11 OUT3 (3Y) 12 GND3  13 GND4  14 OUT4 (4Y) 15 IN4 (4A) 16 VCC1 (logic supply)
//
// Behaviour of every half-bridge n (1..4):
//   ENx = HIGH -> OUTn follows INn (driven HIGH or LOW)
//   ENx = LOW  -> OUTn is released (high impedance), nothing flows through the load
// A PWM signal on EN therefore chops the output, exactly like the real chip, so the
// average voltage across the load is proportional to the PWM duty cycle.
//
// The supply pins (VCC1, VCC2, GND1..GND4) exist only so the circuit can be wired like
// the real schematic; they are not used by the model.
//
// Built without libc (no malloc/printf): all state is static.

#include "wokwi-api.h"

static pin_t en1, en2;
static pin_t in[4];
static pin_t out[4];

static void drive(int n, pin_t en) {
  if (pin_read(en)) {
    pin_mode(out[n], OUTPUT);
    pin_write(out[n], pin_read(in[n]));
  } else {
    pin_mode(out[n], INPUT); // high impedance
  }
}

static void update(void *user_data, pin_t pin, uint32_t value) {
  (void)user_data;
  (void)pin;
  (void)value;
  drive(0, en1);
  drive(1, en1);
  drive(2, en2);
  drive(3, en2);
}

void chip_init(void) {
  en1 = pin_init("EN1", INPUT);
  en2 = pin_init("EN2", INPUT);
  in[0] = pin_init("IN1", INPUT);
  in[1] = pin_init("IN2", INPUT);
  in[2] = pin_init("IN3", INPUT);
  in[3] = pin_init("IN4", INPUT);
  out[0] = pin_init("OUT1", INPUT);
  out[1] = pin_init("OUT2", INPUT);
  out[2] = pin_init("OUT3", INPUT);
  out[3] = pin_init("OUT4", INPUT);

  static const pin_watch_config_t watch = {
    .user_data = 0,
    .edge = BOTH,
    .pin_change = update,
  };
  pin_watch(en1, &watch);
  pin_watch(en2, &watch);
  for (int i = 0; i < 4; i++) {
    pin_watch(in[i], &watch);
  }

  update(0, NO_PIN, 0);
}