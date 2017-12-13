# TFT_Display-Test

	STM32L073Z_EVAL

	Hello World on TFT LCD Display (MB895/S module on the STM32L073Z_EVAL board)

	Press and hold the TAMPER push button [PC13] for a progress bar

	The TFT Display is controlled by the SPI1 interface.

	---
	
	IDE:
	- Keil uVision 5
	- SW4STM32

---

## Initial Setup Notes

#### STM32CubeMX

	SPI1: Full-Duplex Master

	PC13: GPIO_Input

	Clock: 32MHz (PLLCLK x4)


#### [BSP Drivers](../additional/setup.md)

STM32L073Z_EVAL BSP drivers are used for the TFT LCD display and the LEDs.

- `"stm32l073z_eval.h"`
- `"stm32l073z_eval_lcd.h"`
