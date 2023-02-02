.global _start

.syntax unified

.equ STACK_HIGH,	    0x20010000

.equ RCC_BASE,		    0x40021000
.equ RCC_AHB2ENR,	    RCC_BASE + 0x4C
.equ RCC_AHB2ENR_PB,	(1<<1)

.equ GPIOB_BASE,	    0x48000400
.equ GPIOB_MODER,	    GPIOB_BASE + 0x00
.equ GPIO_MODER_PIN4,	(1<<8)
.equ GPIOB_ODR,	        GPIOB_BASE + 0x14
.equ GPIOB_PIN4_HIGH,	(1<<4)
.equ DELAY,		        333333

Vectors:
.word STACK_HIGH	
.word _start + 1

_start:
	ldr r0, = RCC_AHB2ENR
	ldr r1, = RCC_AHB2ENR_PB
	str r1, [r0]
	
	ldr r0, = GPIOB_MODER
	ldr r1, = GPIO_MODER_PIN4
	str r1, [r0]


	ldr r0, = GPIOB_ODR
	ldr r1, = GPIOB_PIN4_HIGH
	str r1, [r0]
    
loop:
	bl playS
	bl playO
	bl playS
	bl wordEnd
	
	b loop
	
playS:
	push {lr}
	ldr r2,= 1
	bl Sound
	bl On
	ldr r2,= 1
	bl Sound
	bl Off
	ldr r2,= 1
	bl Sound
	bl On
	ldr r2,= 1
	bl Sound
	bl Off
	ldr r2,= 1
	bl Sound
	bl On
	ldr r2,= 3
	bl Sound
	bl Off
	pop {lr}
	bx lr
	
playOn:
	push {lr}
	ldr r2,= 3
	bl Sound
	bl On
	ldr r2,= 1
	bl Sound
	bl Off
	ldr r2,= 3
	bl Sound
	bl On
	ldr r2,= 1
	bl Sound
	bl Off
	ldr r2,= 3
	bl Sound
	bl On
	ldr r2,= 3
	bl Sound
	bl Off
	pop {lr}
	bx lr
	
wordEnd:
	push {lr}
	ldr r2,= 7
	bl Sound
	bl Off
	pop {lr}
	bx lr


Sound:
	push {lr}
	ldr r3, = DELAY
	mul r2, r3
	pop {lr}
	bx lr

On:
	push {lr}
	ldr r1, = GPIOB_PIN4_HIGH
	str r1, [r0]
    
waitStart:
	subs r2, r2, #1
	bne waitStart
	pop {lr}
	bx lr
	
Off:
	push {lr}
	ldr r1, = 0x00
	str r1, [r0]

waitEnd:
	subs r2, r2, #1
	bne waitEnd
	pop {lr}
	bx lr