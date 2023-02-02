  .global _start

  .syntax unified

  /* The following values are adapted to the Nucleo L432KC
   * any other model may need adjustments */
  .equ STACK_HIGH,          0x20010000

  /* Relevant addresses and values for the RCC register bank */
  .equ RCC_BASE,            0x40021000
  .equ RCC_AHB2ENR,         RCC_BASE + 0x4c
  .equ RCC_AHB2ENR_PB,      (1<<1)

  /* Relevant addresses and values of the GPIOB register bank */
  .equ GPIOB_BASE,          0x48000400
  .equ GPIOB_MODER,         GPIOB_BASE + 0x00
  .equ GPIO_MODER_PIN3O,    (1<<6)
  .equ GPIOB_ODR,           GPIOB_BASE + 0x14
  .equ GPIO_PIN3_HIGH,      (1<<3)

  .equ DELAY,               0x80000

Vectors:
  .word STACK_HIGH  /* initial value of the sp */
  .word _start + 1

_start:
  /* Get contents of AHB2 bus clock enable register */
  ldr     r0, =RCC_AHB2ENR
  ldr     r1, [r0]

  /* Set bit to enable clock for GPIO port B */
  ldr     r2, =RCC_AHB2ENR_PB
  orrs    r1, r2
  str     r1, [r0]

  /* Configure GPIO pin as output */
  ldr     r0, =GPIOB_MODER
  ldr     r1, =GPIO_MODER_PIN3O
  str     r1, [r0]

  /* Load address of GPIOB output data register in r0 */
  ldr     r0, =GPIOB_ODR
  /* Load bitmask of gpio pin3 in r1 */
  ldr     r1, =GPIO_PIN3_HIGH
.blink:
  /* Store current r1 into GPIOB output data register */
  str     r1, [r0]

  /* Busy wait */
  ldr     r2, =DELAY
.loop:
  subs    r2, r2, #1
  bne     .loop

  /* Toggle bit corresponding to bit 3 */
  eors    r1, #GPIO_PIN3_HIGH

  /* Repeat endlessly */
  b       .blink
