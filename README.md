# STM32L073VZT6 Learning Projects

This is a collection of projects which I have done on the [STM32L073Z_EVAL Evaluation Board](http://www.st.com/en/evaluation-tools/stm32l073z-eval.html). It has the [STM32L073VZT6](http://www.st.com/en/microcontrollers/stm32l073vz.html) (ARM Cortex-M0+) chip.

These projects use the STM32CubeMX app to generate project files. I have used [SW4STM32](http://www.st.com/en/development-tools/sw4stm32.html) and [Keil uVision ARM MDK](http://www2.keil.com/mdk5) for the IDE.

Project                                            | Done on    | Category           | Remarks
---------------------------------------------------|:----------:| ------------------ | -----------------
[TFT-Display_BSP]                                  | 2017-12-06 | TFT Display        |
[Joystick_BSP]                                     | 2017-12-10 | Joystick           | 
[TFT-Joystick-SnakeGame]                           | 2017-12-11 | Joystick, TFT Display | *C++11*
[DAC-Waveform_Generator]                           | 2017-12-14 | DAC, DMA           |
[RNG-Dice]                                         | 2017-12-14 | RNG                |
[TFT-CustomFonts]                                  | 2018-01-03 | TFT Display        | [STM32-LCD_Font_Generator](https://github.com/zst-embedded/STM32-LCD_Font_Generator)

[TFT-Display_BSP]: ./projects/TFT-Display_BSP
[Joystick_BSP]: ./projects/Joystick_BSP
[TFT-Joystick-SnakeGame]: ./projects/TFT-Joystick-SnakeGame
[DAC-Waveform_Generator]: ./projects/DAC-Waveform_Generator
[RNG-Dice]: ./projects/RNG-Dice
[TFT-CustomFonts]: ./projects/TFT-CustomFonts

## [Setup Guide + References](additional/setup.md)


## STM32 Demonstration Software

There was a [preloaded demo application on the board](./additional/Demonstration_SW4STM32-STM32L073Z_EVAL) (source code is provided by ST).
Since Keil had a code size limit, I have rearranged it to compile as a standalone SW4STM32 project.
