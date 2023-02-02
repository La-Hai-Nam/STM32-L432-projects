/**
 * @file
 *
 * The actual implementation of the GPIO-based tm1637 driver.
 *
 * The code is based on https://github.com/nimaltd/tm1637 but has been enhanced here to be generic. Additionally, few
 * changes have been applied to match https://github.com/depklyon/raspberrypi-tm1637 and some results of own
 * experiments.
 */
#include "tm1637.h"
#include "tm1637_config.h"
#include "utility.h"

<<<<<<< HEAD

=======
>>>>>>> c8e76fa00c4365a3eb68492fbd2e19f39e78bb6c


/** Maps digits to segments */
static const uint8_t _tm1637_digit[] =
{
  0x3f, /* 0 */
  0x06,
  0x5b,
  0x4f,
  0x66,
  0x6d,
  0x7d,
  0x07,
  0x7f,
  0x6f  /* 9 */
};

static const uint8_t _tm1637_minus = 0x40;
static const uint8_t _tm1637_dot = 0x80;  

/**********************************************************************************************************************/

void tm1637_start(tm1637_t *tm1637)
{
  tm1637->write_dio(tm1637, 0);
  tm1637->delay_us(tm1637, _TM1637_BIT_DELAY / 2);
  tm1637->write_clk(tm1637, 0);
  tm1637->delay_us(tm1637, _TM1637_BIT_DELAY);
}

/**********************************************************************************************************************/

void tm1637_stop(tm1637_t *tm)
{
  tm->write_dio(tm, 0);
  tm->delay_us(tm, _TM1637_BIT_DELAY);
  tm->write_clk(tm, 1);
  tm->delay_us(tm, _TM1637_BIT_DELAY);
  tm->write_dio(tm, 1);
  tm->delay_us(tm, _TM1637_BIT_DELAY);
}

/**********************************************************************************************************************/

uint8_t tm1637_write_byte(tm1637_t *tm, uint8_t data)
{
  /*  write 8 bit data, bit by bit, least significant bit first */
  for (uint8_t i = 0; i < 8; i++)
  {
    tm->write_clk(tm, 0);
    tm->delay_us(tm, _TM1637_BIT_DELAY);
    tm->write_dio(tm, data & (1<<i));
    tm->delay_us(tm, _TM1637_BIT_DELAY);
    tm->write_clk(tm, 1);
    tm->delay_us(tm, _TM1637_BIT_DELAY);
  }

  /* wait for acknowledge */
  tm->write_clk(tm, 0);
  tm->write_dio(tm, 1);
  tm->delay_us(tm, _TM1637_BIT_DELAY);
  tm->write_clk(tm, 1);
  tm->delay_us(tm, _TM1637_BIT_DELAY);
  uint8_t ack = tm->read_dio(tm);
  if (ack == 0)
  {
    tm->write_dio(tm, 0);
  }
  tm->delay_us(tm, _TM1637_BIT_DELAY);
  tm->write_clk(tm, 0);
  return ack;
}

/**********************************************************************************************************************/

void tm1637_init(tm1637_t *tm1637, tm1637_delay_us delay_us, tm1637_write_clk write_clk, tm1637_read_dio read_dio, tm1637_write_dio write_dio, void *userdata)
{
  memset(tm1637, 0, sizeof(tm1637_t)); 

  tm1637->delay_us = delay_us;
  tm1637->write_clk = write_clk;
  tm1637->read_dio = read_dio;
  tm1637->write_dio = write_dio;
  tm1637->userdata = userdata;

  /* set max brightess */
  tm1637_brightness(tm1637, 7);  

  /* Raise signals */
  tm1637->write_clk(tm1637, 1);
  tm1637->write_dio(tm1637, 1);
}

/**********************************************************************************************************************/

void tm1637_brightness(tm1637_t *tm1637, uint8_t brightness_0_to_7)
{
  tm1637->brightness = (brightness_0_to_7 & 0x7) | 0x08;
}

/**********************************************************************************************************************/

void tm1637_write_raw(tm1637_t *tm1637, const uint8_t *raw, uint8_t length, uint8_t pos)
{
  if (pos > 3)
    return;
  if (length > 3)
    length = 4;
  /* Write COMM1 */
  tm1637_start(tm1637);
  tm1637_write_byte(tm1637, TM1637_COMM1);
  tm1637_stop(tm1637);

  /* Write COMM2 + first digit address */
  tm1637_start(tm1637);
  tm1637_write_byte(tm1637, TM1637_COMM2 + (pos & 0x03));
  /* Write the data bytes */
  for (uint8_t k=0; k < length; k++)
    tm1637_write_byte(tm1637, raw[k]);
  tm1637_stop(tm1637);
  /* Write COMM3 + brightness */
  tm1637_start(tm1637);
  tm1637_write_byte(tm1637, TM1637_COMM3 + tm1637->brightness);
  tm1637_stop(tm1637);
}

/**********************************************************************************************************************/

void tm1637_write_segment(tm1637_t *tm1637, const uint8_t *segments, uint8_t length, uint8_t pos)
{
  tm1637_write_raw(tm1637, segments, length, pos);
}

/**********************************************************************************************************************/

void tm1637_write_uint(tm1637_t *tm1637, uint32_t num, uint8_t pos)
{
  uint8_t segments[7] = {0};
  uint32_t start = 0;

  if (num > 9999)
  {
    static const uint8_t error[] = {0xff, 0xff, 0xff, 0xff};
    tm1637_write_raw(tm1637, error, sizeof(error)/sizeof(error[0]), pos);              
    return;
  }

  segments[3] = num % 10;
  segments[2] = (num / 10) % 10;
  segments[1] = (num / 100) % 10;
  segments[0] = (num / 1000) % 10;

  if (num < 10) start = 3;
  else if (num < 100) start = 2;
  else if (num < 1000) start = 1;

  for (uint32_t i = 0; i < 4; i++)
  {
    segments[i] = _tm1637_digit[segments[i]];
  }
  tm1637_write_raw(tm1637, &segments[start], 4, pos);              
}

