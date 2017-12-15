# TFT-Joystick-SnakeGame

	STM32L073Z_EVAL

	Snake game using the TFT Display and Joystick

	(C++ used for main control logic)
	
	---
	
	IDE:
	- SW4STM32

---

## Initial Setup Notes

#### STM32CubeMX

	SPI1: Full-Duplex Master

	PC13: GPIO_EXTI13

	Clock: 32MHz (PLLCLK x4)


#### [BSP Drivers](../additional/setup.md)

STM32L073Z_EVAL BSP drivers are used for the TFT LCD display and joystick.

- `"stm32l073z_eval.h"`
- `"stm32l073z_eval_lcd.h"`
