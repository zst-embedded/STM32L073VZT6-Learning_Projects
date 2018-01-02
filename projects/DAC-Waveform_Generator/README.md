# DAC-Waveform_Generator

    STM32L073Z_EVAL

    Mini Function Generator using DAC.
    The output waveform on PA4 (DAC_OUT1 at CN3).

    The DAC is controlled using DMA with TIM7 as the trigger.

	The TFT Display is controlled by the SPI1 interface.
	TIM6 draws an animation on the display every second.

	Press the TAMPER button to switch between Sine, Triangle and Sawtooth
	waves. The joystick is used to control the frequency of generated wave

	---

	IDE:
	- Keil uVision 5
	- SW4STM32

## References

- [Elastic Notes: Usage DMA and DAC in STM32CubeMX](http://elastic-notes.blogspot.com/p/blog-page_1.html)

- [Firmic Designs: STM32 Getting Started - DAC](http://www.firmic-designs.com/blog/2016/12/04/stm32-getting-started---dac/)

- [AN3126 Application note: Audio and waveform generation using the DAC](http://www.st.com/content/ccc/resource/technical/document/application_note/05/fb/41/91/39/02/4d/1e/CD00259245.pdf/files/CD00259245.pdf/jcr:content/translations/en.CD00259245.pdf)


---


## Initial Setup Notes


#### STM32CubeMX: Configuration

    DAC: OUT1 (PA4)

    SPI1: Full-Duplex Master

    PC13: EXTI13 [TAMPER button]

    PE6: EXTI6 [MFX_IRQOUT]

    Clock: 32MHz (PLLCLK x4)

    TIM6: Activated 

    TIM7: Activated [for DMA controller]


#### STM32CubeMX: DAC with DMA

1. Activate basic timer, TIM6/TIM7, to send requests to DMA controller

2. TIM6/TIM7 Configuration > Parameter Settings 
	- Trigger Event Selection: Update Event
	- Prescalar / Counter Period
	- ***(Step through data every timer period)***

3. DAC Configuration > Parameter Settings 
	- Output Buffer: Enabled
	- Trigger: Timer 6/7 Trigger Out Event

4. DAC Configuration > DMA Settings 
	- Direction: Memory to Peripheral ***(Copy data from memory to DAC)***
	- Data Width: Half Word ***(Using 16-bit int)***
	- Mode: Circular ***(Loop through the values over and over again)***


#### DAC & DMA Sample Code

	// Start TIM7 to send requests to DMA controller 
	HAL_TIM_Base_Start(&htim7);

	// Start DAC channel
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	// EITHER: manually set DAC value
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048); 

	// OR: use DMA to move data from address of array and array size
	const uint16_t sine_wave_array[32] = {
		2047, 1648, 1264, 910, 600,  345,
		156, 39,  0,  39,  156,  345,
		600, 910, 1264, 1648, 2048, 2447,
		2831, 3185, 3495, 3750, 3939, 4056,
		4095, 4056, 3939, 3750, 3495, 3145685,
		2831, 2447
	};
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,
		(uint32_t*) sine_wave_array, 32, DAC_ALIGN_12B_R);


#### Pi constants
ARMCC does not define PI constants.
Add the following to the compiler flags
	
	-DM_PI=3.1415926535897932384626433832795
	-DM_1_PI=0.31830988618379067153776752674503


#### [BSP Drivers](../../additional/setup.md)

STM32L073Z_EVAL BSP drivers are used for the TFT LCD display, joystick and the LEDs.

- `"stm32l073z_eval.h"`
- `"stm32l073z_eval_lcd.h"`

