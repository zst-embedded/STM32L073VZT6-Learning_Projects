# RNG-Dice

	STM32L073Z_EVAL

	A project using the random number generator (RNG).

	Press the TAMPER button to roll the dice on the TFT Display.
	
	---
	
	IDE:
	- Keil uVision 5
	- SW4STM32

## References

- https://github.com/MaJerle/stm32f429/blob/master/00-STM32F429_LIBRARIES/tm_stm32f4_rng.c

- https://www.reddit.com/r/stm32f4/comments/5eas82/troubles_with_rng_on_discovery/

- https://community.st.com/thread/15547

---

## Initial Setup Notes

#### STM32CubeMX

	SPI1: Full-Duplex Master

	RNG: Activated

	PC13: GPIO_EXTI13

	Clock: 32MHz (PLLCLK x4)

	NVIC:
	  - EXTI line 4 to 15 interrupts
	  - AES, RNG and LPUART1 interrupts


#### [BSP Drivers](../../additional/setup.md)

STM32L073Z_EVAL BSP drivers are used for the TFT LCD display.

- `"stm32l073z_eval_lcd.h"`
