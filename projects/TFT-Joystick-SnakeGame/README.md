# TFT-Joystick-SnakeGame

	STM32L073Z_EVAL

	Snake game using the TFT Display and Joystick

	(C++ used for main control logic)
	
	---
	
	IDE:
	- Keil uVision 5
	- SW4STM32

---

## Initial Setup Notes

#### Memory configuration 

When compiling in Keil, if the program overruns the default stack/heap size, a hard fault will occur.
This can be fixed by doubling the stack/heap size in `startup_stm32l073xx.s`

	;Default stack size is 0x400
	Stack_Size      EQU     0x800

	;Default heap size is 0x200
	Heap_Size       EQU     0x400

#### STM32CubeMX

	SPI1: Full-Duplex Master

	PC13: GPIO_EXTI13

	Clock: 32MHz (PLLCLK x4)


#### [BSP Drivers](../../additional/setup.md)

STM32L073Z_EVAL BSP drivers are used for the TFT LCD display and joystick.

- `"stm32l073z_eval.h"`
- `"stm32l073z_eval_lcd.h"`

---

## C++ Linker Errors

`stm32l073z_eval.h` was modified to add the following function prototypes:
	
	/* Link functions for LCD peripheral */
	void                     LCD_IO_Init(void);
	void                     LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
	void                     LCD_IO_WriteReg(uint8_t Reg);
	uint16_t                 LCD_IO_ReadData(uint16_t RegValue);
	void                     LCD_Delay(uint32_t delay);

Without these, C++ ame mangling is done to these functions which will cause the following errors:

	linking...
	TFT-Joystick-SnakeGame\TFT-Joystick-SnakeGame.axf: Error: L6218E: Undefined symbol LCD_Delay (referred from hx8347d.o).
	TFT-Joystick-SnakeGame\TFT-Joystick-SnakeGame.axf: Error: L6218E: Undefined symbol LCD_IO_Init (referred from hx8347d.o).
	TFT-Joystick-SnakeGame\TFT-Joystick-SnakeGame.axf: Error: L6218E: Undefined symbol LCD_IO_ReadData (referred from hx8347d.o).
	TFT-Joystick-SnakeGame\TFT-Joystick-SnakeGame.axf: Error: L6218E: Undefined symbol LCD_IO_WriteMultipleData (referred from hx8347d.o).
	TFT-Joystick-SnakeGame\TFT-Joystick-SnakeGame.axf: Error: L6218E: Undefined symbol LCD_IO_WriteReg (referred from hx8347d.o).
