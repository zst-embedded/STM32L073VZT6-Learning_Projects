# Setup Guides

## References

- [Evaluation Board User Manual](http://www.st.com/resource/en/user_manual/dm00171429.pdf)

- [STM32L073xZ Reference Manual](http://www.st.com/resource/en/reference_manual/dm00095744.pdf)

- [STM32L073xZ Datasheet](http://www.st.com/resource/en/datasheet/stm32l073vz.pdf)

---

## Activate ARM MDK

- [Full Version of ARM MDK for STM32L0](http://www2.keil.com/stmicroelectronics-stm32/mdk)


---

## Import BSP Drivers 

### 1\. Copy files:

    STM32Cube_FW_L0_V1.10.0/Drivers/BSP/STM32L073Z_EVAL
    STM32Cube_FW_L0_V1.10.0/Drivers/BSP/Components
    STM32Cube_FW_L0_V1.10.0/Utilities

***[(Files are here)](./L0_BSP_Drivers)***

### 2\. Add to build

#### Keil uVision:

> In Project Window: ***[Right-click project] > Options for Target > C/C++ > Include Paths***

	../Drivers/BSP/STM32L073Z_EVAL;../Drivers/BSP/Components/Common;../Drivers/BSP/Components

> In Project Window: ***[Right-click project] > Manage Project Items***

+ Create new groups:
	- `Drivers/BSP/Components` 
	- `Drivers/BSP/STM32L073Z_EVAL`

+ Add all files from their respective folders

+ Fix build errors 
	- *Remove the BSP drivers which rely on unenabled peripherals.*
	- *eg. Some need SPI or I2C, and will have build errors.*


#### SW4STM32:

> In Project Explorer: ***[Right-click project] > Properties > C/C++ Build > Settings > Tool Settings > Includes***

    ../Drivers/BSP/STM32L073Z_EVAL
    ../Drivers/BSP/Components/Common
    ../Drivers/BSP/Components





---

## Power board from ST-LINK

Change `JP11`:

- `U5V` (Micro USB)
- `STlk` (ST-LINK)
