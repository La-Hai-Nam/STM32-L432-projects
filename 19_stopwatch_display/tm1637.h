#ifndef _TM1637_H_
#define _TM1637_H_

/**
 * This is header for the library to interface the tm1637 module. The implementation is generic and should work
 * for every target hardware which is capable to get and set the state of GPIO pins. For this flexibility, you have
 * to supply functions to the initialization. Note that more sophisticated hardware features of certain microcontrollers
 * is not supported by this library. Also note that the API is not thread-safe.
 *
 * @author Sebastian Bauer
 */

#include <stdint.h>
typedef enum { false, true} bool;
#define TM1637_COMM1    0x40
#define TM1637_COMM2    0xC0
#define TM1637_COMM3    0x80


/********************************************************************************************************************/

struct tm1637;

/** The callback type for function to call for a delay in microseconds */
typedef void (*tm1637_delay_us)(struct tm1637 *tm, uint32_t us);

/** The callback type for a function to set the clock pin to the specific value (0 or 1) */
typedef void (*tm1637_write_clk)(struct tm1637 *tm, unsigned int val);

/** The callback type for a function to set the data input output to the specific value (0 or 1) */
typedef void (*tm1637_write_dio)(struct tm1637 *tm, unsigned int val);

/** The callback type for a function to read the data input output */
typedef unsigned int (*tm1637_read_dio)(struct tm1637 *tm);


/**
 * The context of the interface to the tm1637 module.
 */
struct tm1637
{
  /** The current brightness (from 0 to 7)*/
  uint8_t               brightness;

  /** Tells whether we want to show leading zeros */
  bool                  show_zero;

  /** Some additional user data that can be used to keep arbritary data such as a context */
  void *userdata;

  /** Wait a given amount of time as specified by the us parameter */
  tm1637_delay_us delay_us;

  /** Write the given value to the clk pin */
  tm1637_write_clk write_clk;

  /** Read the given value of the dio pin */
  tm1637_read_dio read_dio;

  /** Write the given value to the dio pin */
  tm1637_write_dio write_dio;
};

/** The tm1636 type-alias */
typedef struct tm1637 tm1637_t;

/** Initialize the context */
void tm1637_init(tm1637_t *tm1637,
    tm1637_delay_us delay_us,
    tm1637_write_clk write_clk,
    tm1637_read_dio read_dio,
    tm1637_write_dio write_dio,
    void *userdata);

/**
 * Sets the brightness.
 *
 * @param tm1637 the target instance
 * @param brightness_0_to_7 the brightness in range from 0 to 7
 */
void tm1637_brightness(tm1637_t *tm1637, uint8_t brightness_0_to_7);

/**
 * Write the given segments to the display
 *
 * Each segment is represented by a single byte and coded as given in the data sheet page 11.
 *
 * @param tm1637 the tm1637 instance
 * @param segments defines the segments to be displayed. Each byte represents the bitmask of a single segment
 * @param length the number of segments to be displayed. Cannot be larger than 4.
 * @param pos the number of the first segment to be updated. Cannot be larger than 3.
 */
void tm1637_write_segment(tm1637_t *tm1637, const uint8_t *segments, uint8_t length, uint8_t pos);

/**
 * Write the given integer value to be displayed as a decimal number in the display.
 *
 * If the given number is too large, an alternative pattrn is displayed instead. 
 *
 * @param tm1637 the tm1637 instance
 * @param num defines the number that shall be displayed. Between 0 and inclusive 9999.
 * @param pos the number of the first segment to be updated. Cannot be larger than 3.
 */
void tm1637_write_uint(tm1637_t *tm1637, uint32_t num, uint8_t pos);

// static void tm_delay_us(struct tm1637 *tm, uint32_t us);

// static void tm_write_clk(struct tm1637 *tm, unsigned int val);

// static void tm_write_dio(struct tm1637 *tm, unsigned int val);

// static unsigned int tm_read_dio(struct tm1637 *tm);



#endif
