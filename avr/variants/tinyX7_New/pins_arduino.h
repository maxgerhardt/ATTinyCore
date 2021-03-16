/* pins_arduino.h - Pin definition functions for ATTinyCore
 * Part of ATTinyCore - github.com/SpenceKonde/ATTinyCore
 *   Copyright (c) 2015~2021 Spence Konde, (c) 2007 David A. Mellis
 *   Free Software - GPL 2.1, please see LICENCE.md for details
 *---------------------------------------------------------------------------*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

/*===========================================================================
 * Microchip ATtiny167, ATtiny87
 *===========================================================================
 * ATTinyCore Standard Pin Mapping
 * PA0 -> PA7 -> PB0 ->PB7 straight through no exceptions. Dead simple and
 * naturally ends on Reset. Only awkward bit is that the crystal goes on pins
 * PB4 and PB5, so if using a crystal, pin 11 is available, pin 15 is never
 * available because it's reset, but the crystal is on pins 12 and 13 leaving
 * 14 standing off to the side. Of course if you use PIN_Pxn to refer to pins
 * you won't even notice this odd looking happenstance.
 * This is recommended for all applications except where Digispark Pro hardware
 * or clone thereof is being used.
 *---------------------------------------------------------------------------*/

#define ATTINYX7 1       //backwards compat
#define __AVR_ATtinyX7__ //recommended

#define NUM_DIGITAL_PINS            16
#define NUM_ANALOG_INPUTS           11

/* Basic Pin Numbering - PIN_Pxn notation is always recommended
 * as it is totally unambiguous, but numbers may be used too */
#define PIN_PA0  ( 0)
#define PIN_PA1  ( 1)
#define PIN_PA2  ( 2)
#define PIN_PA3  ( 3)
#define PIN_PA4  ( 4)
#define PIN_PA5  ( 5)
#define PIN_PA6  ( 6)
#define PIN_PA7  ( 7)
#define PIN_PB0  ( 8)
#define PIN_PB1  ( 9)
#define PIN_PB2  (10)
#define PIN_PB3  (11)
#define PIN_PB4  (12) /* XTAL1 */
#define PIN_PB5  (13) /* XTAL2 */
#define PIN_PB6  (14)
#define PIN_PB7  (15) /* RESET */

#define LED_BUILTIN (PIN_PA3)

/* PIN_An is the digital pin with analog channel An on it. */
#define PIN_A0      (PIN_PA0)
#define PIN_A1      (PIN_PA1)
#define PIN_A2      (PIN_PA2)
#define PIN_A3      (PIN_PA3)
#define PIN_A4      (PIN_PA4)
#define PIN_A5      (PIN_PA5)
#define PIN_A6      (PIN_PA6)
#define PIN_A7      (PIN_PA7)
#define PIN_A8      (PIN_PB5)
#define PIN_A9      (PIN_PB6)
#define PIN_A10     (PIN_PB7)

/* An "analog pins" these map directly to analog channels */
static const uint8_t  A0 =  ADC_CH(0);
static const uint8_t  A1 =  ADC_CH(1);
static const uint8_t  A2 =  ADC_CH(2);
static const uint8_t  A3 =  ADC_CH(3);
static const uint8_t  A4 =  ADC_CH(4);
static const uint8_t  A5 =  ADC_CH(5);
static const uint8_t  A6 =  ADC_CH(6);
static const uint8_t  A7 =  ADC_CH(7);
static const uint8_t  A8 =  ADC_CH(8);
static const uint8_t  A9 =  ADC_CH(9);
static const uint8_t A10 = ADC_CH(10);

/* Interrupt macros to go from pin to PCMSK register and bit within it, and
 * the register to enable/disable banks of PCINTs, and bit within it PCICR
 * is almost always the same for all PCINTs; but must return null pointer
 * if the pin is invalid. The PCICRbit and PCMSK are almost always directly
 * mapped to port; particularly on ugly mappings like this, taking advantage
 * of this is more efficient and easier to write.
 * digitalPinToInterrupt gets the number of the "full service" pin interrupt
 *---------------------------------------------------------------------------*/

#define digitalPinToPCICR(p)    (&PCICR)
#define digitalPinToPCICRbit(p) ((p) >= 8 ? 1 : 0)
#define digitalPinToPCMSK(p)    ((p) >= 8 ?(&PCMSK1) : (&PCMSK0))
#define digitalPinToPCMSKbit(p) (p & 15)

#define digitalPinToInterrupt(p)  ((p) == PIN_PB6 ? 0 : ((p)==PIN_PA3?1: NOT_AN_INTERRUPT))

/* Analog Channel <-> Digital Pin macros */
#define analogInputToDigitalPin(p)  ((p) < 8 ? (p) :((p) + 5))
#define digitalPinToAnalogInput(p)  ((p) < 8 ? (p) : ((p) > 12 ? (p - 5) : (-1)))

/* Which pins have PWM? */
#define digitalPinHasPWM(p)     ((p) == 2 || (p) > 7 )

#define PINMAPPING_NEW

/*---------------------------------------------------------------------------
 * Core Configuration where these are not the defaults
 *---------------------------------------------------------------------------*/
// Choosing not to initialise saves flash.      1 = initialise.
// #define DEFAULT_INITIALIZE_ADC               1
// #define DEFAULT_INITIALIZE_SECONDARY_TIMERS  1

// We have hardware serial, so don't use soft serial.
// #define USE_SOFTWARE_SERIAL                  0

/*---------------------------------------------------------------------------
 * Chip Features - Analog stuff
 *---------------------------------------------------------------------------
 * Analog reference constants are pre-shifted to their final position in the
 * registers to avoid leftshifting at runtime, which is surprisingly slow and
 * wasteful of flash. ADC_REF definition here is longer because we need to
 * take the two high bits (AREF, XREF), and put them in position for where
 * they go in the AMISCR... better here than at runtime!
 *---------------------------------------------------------------------------*/
/* This is weird on the 87/167 - the internal references are selected by the
 * REFS bits, but two additional bits on the AMISCR (Analog MIScellaneous
 * Control Register) are used. AREF controls whether an external reference is
 * used (ignored if internal ref selected) and XREF which outputs the internal
 * reference on the AREF pin. They suggest a 5nF-10nF capacitor on that pin,
 * and note that load on it should be between 1uA and 100uA. No idea why they
 * used two bits, one of which is ignored when internal ref is in use and the
 * other which is ignored when internal ref not in use...
 *---------------------------------------------------------------------------*/
#define ADC_REF(x)       ((((x) & 0x03) << 6) | (((x) & 0x0C) >> 1)

/* Analog Reference bit masks */
#define DEFAULT           ADC_REF(0x00)
#define EXTERNAL          ADC_REF(0x08)
#define INTERNAL1V1       ADC_REF(0x02) /* Not connected to AREF; AREF may be used for other purposes */
#define INTERNAL            INTERNAL1V1
#define INTERNAL2V56      ADC_REF(0x03) /* Not connected to AREF; AREF may be used for other purposes */
#define INTERNAL1V1_XREF  ADC_REF(0x06)
#define INTERNAL2V56_XREF ADC_REF(0x07)

/* Special Analog Channels */
#define ADC_TEMPERATURE    ADC_CH(0x0B)
#define ADC_INTERNAL1V1    ADC_CH(0x0C)
#define ADC_AVCCDIV4       ADC_CH(0x0D)
#define ADC_GROUND         ADC_CH(0x0E)

/* Differential Analog Channels */
#define DIFF_A0_A1_8X      ADC_CH(0x10)
#define DIFF_A0_A1_20X     ADC_CH(0x11)
#define DIFF_A1_A2_8X      ADC_CH(0x12)
#define DIFF_A1_A2_20X     ADC_CH(0x13)
#define DIFF_A2_A3_8X      ADC_CH(0x14)
#define DIFF_A2_A3_20X     ADC_CH(0x15)
#define DIFF_A4_A5_8X      ADC_CH(0x16)
#define DIFF_A4_A5_20X     ADC_CH(0x17)
#define DIFF_A5_A6_8X      ADC_CH(0x18)
#define DIFF_A5_A6_20X     ADC_CH(0x19)
#define DIFF_A6_A7_8X      ADC_CH(0x1A)
#define DIFF_A6_A7_20X     ADC_CH(0x1B)
#define DIFF_A8_A9_8X      ADC_CH(0x1C)
#define DIFF_A8_A9_20X     ADC_CH(0x1D)
#define DIFF_A9_A10_8X     ADC_CH(0x1E)
#define DIFF_A9_A10_20X    ADC_CH(0x1F)

/* Analog Comparator - not used by core */
#define ANALOG_COMP_DDR          (DDRA)
#define ANALOG_COMP_PORT        (PORTA)
#define ANALOG_COMP_PIN          (PINA)
#define ANALOG_COMP_AIN0_BIT        (6)
#define ANALOG_COMP_AIN1_BIT        (7)

/*---------------------------------------------------------------------------
 * Chip Features - SPI, I2C, USART, etc
 *---------------------------------------------------------------------------*/
/*  This part has a USI, not a TWI module - but it DOES have an SPI module!
 *  We provide USI defines so that Arduino.h will sort out SCL, SDA pin assignment.
 *  The included version of Wire.h will use the USI for TWI if requested.
 *---------------------------------------------------------------------------*/

/* Hardware SPI */
#define MISO            PIN_PA2
#define MOSI            PIN_PA4
#define SCK             PIN_PA5
#define SS              PIN_PA6

/* USI */
#define PIN_USI_DI      PIN_PB0
#define PIN_USI_DO      PIN_PB1
#define PIN_USI_SCK     PIN_PB2

#define USI_DATA_DDR       DDRB
#define USI_DATA_PORT     PORTB
#define USI_DATA_PIN       PINB

#define USI_CLOCK_BIT     PINB2
#define USI_DO_BIT        PINB1
#define USI_DI_BIT        PINB0

#define USI_START_VECTOR    USI_START_vect
#define USI_OVERFLOW_VECTOR USI_OVF_vect
#ifndef USI_START_COND_INT
  #define USI_START_COND_INT USISIF
#endif

/* One hardware LIN port, which is a UART with a ton of wacky features */
#define PIN_HWSERIAL0_TX         PIN_PA1
#define PIN_HWSERIAL0_RX         PIN_PA0
#define HWSERIAL0_IS_LIN

#ifdef ARDUINO_MAIN

/*---------------------------------------------------------------------------
 * ATMEL ATTINY167/ATTINY87
 * Arduino-compatible pin mapping
 *
 *                   +-\/-+
 *   RX   ( 0) PA0  1|a   |20  PB0 ( 8)
 *   TX   ( 1) PA1  2|a   |19  PB1 ( 9)
 *       *( 2) PA2  3|a   |18  PB2 (10)
 *   INT1 ( 3) PA3  4|a   |17  PB3 (11)*
 *            AVCC  5|    |16  GND
 *            AGND  6|    |15  VCC
 *        ( 4) PA4  7|a   |14  PB4 (12)  XTAL1
 *        ( 5) PA5  8|a  a|13  PB5 (13)  XTAL2
 *        ( 6) PA6  9|a  a|12  PB6 (14)* INT0
 *        ( 7) PA7 10|a  a|11  PB7 (15)  RESET
 *                   +----+
 *
 * * indicates PWM pin, a indicates ADC (analog input) pins
 *---------------------------------------------------------------------------*/

const uint8_t PROGMEM port_to_mode_PGM[] =
{
  NOT_A_PORT,
  (uint8_t)(uint16_t)&DDRA,
  (uint8_t)(uint16_t)&DDRB,
};

const uint8_t PROGMEM port_to_output_PGM[] =
{
  NOT_A_PORT,
  (uint8_t)(uint16_t)&PORTA,
  (uint8_t)(uint16_t)&PORTB,
};

const uint8_t PROGMEM port_to_input_PGM[] =
{
  NOT_A_PORT,
  (uint8_t)(uint16_t)&PINA,
  (uint8_t)(uint16_t)&PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
  PA, /* 0 */
  PA,
  PA, /* 2 */
  PA, /* 3 */
  PA, /* 4 */
  PA,
  PA,
  PA,
  PB,
  PB,
  PB, /* 10 */
  PB,
  PB,
  PB,
  PB,
  PB, /* 15 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
  _BV(0), /* 0 */
  _BV(1),
  _BV(2), /* 2 */
  _BV(3), /* 3 */
  _BV(4), /* 4 */
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0),
  _BV(1),
  _BV(2), /* 10 */
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7), /* 15 */
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER0A,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIM1AU,
  TIM1BU,
  TIM1AV,
  TIM1BV,
  TIM1AW,
  TIM1BW,
  TIM1AX,
  TIM1BX
};

#endif

#endif
