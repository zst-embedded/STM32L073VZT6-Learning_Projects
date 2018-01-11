# FATFS-SD_Card

    STM32L073Z_EVAL

    This project is to test out the FATFS implementation.
    On start, the program links the driver upon inserting the SD card.

    It then writes to "WRITE_TEST.TXT" and reads from "READ_TEST.TXT" which
    is then displayed on the TFT LCD Display.

    It appends a char to "COUNT.TXT" on each boot, acting as a boot counter.
    
    The filenames are longer than 8.3 filenames, hence LFN is enabled.

    ---

    IDE:
    - Keil uVision 5
    - SW4STM32

## References

- [UM1721 User manual: Developing Applications on STM32Cube with FatFs](http://www.st.com/content/ccc/resource/technical/document/user_manual/61/79/2b/96/c8/b4/48/19/DM00105259.pdf/files/DM00105259.pdf/jcr:content/translations/en.DM00105259.pdf)
- [Open or create a file for append - ELM by ChaN](http://elm-chan.org/fsw/ff/res/app1.c)
- [user_diskio.c for HAL and FatFS combination](https://gist.github.com/sasakaranovic/37eddf3f332108e728326c991d2081a8)
- [SD cards with STM32 over SPI](https://ralimtek.com/Stm32_SPI_SD/)


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

    Clock: 32MHz (PLLCLK x4)

    FATFS: User-defined
    - USE_LFN: Enabled with static working buffer on the BSS.


#### FATFS SD Card Disk IO Driver

The driver to link the underlying BSP SD card functions to FATFS module.

Copy into `Src`:

- `~/STM32Cube/Repository/STM32Cube_FW_L0_V1.10.0/Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.c`
- `~/STM32Cube/Repository/STM32Cube_FW_L0_V1.10.0/Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.h`


#### [BSP Drivers](../../additional/setup.md)

STM32L073Z_EVAL BSP drivers are used for the TFT LCD display, and SD card.

- `"stm32l073z_eval.h"`
- `"stm32l073z_eval_lcd.h"`
- `"stm32l073z_eval_sd.h"`


---

## FATFS APIs

Refer to:

- `2.5 FatFs APIs` in the [UM1721 User manual](http://www.st.com/content/ccc/resource/technical/document/user_manual/61/79/2b/96/c8/b4/48/19/DM00105259.pdf/files/DM00105259.pdf/jcr:content/translations/en.DM00105259.pdf)
- `ff.h` in FATFS sources

## FATFS Abstraction Layers

For reference, these are the FATFS abstraction layers for the STM32L073Z_EVAL board.

#### Basic SPI functions

    // stm32l073z_eval.c
    void SD_IO_Init(void);
    void SD_IO_CSState(uint8_t state);
    void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
    uint8_t SD_IO_WriteByte(uint8_t Data);

#### SD interfacing (for sending various commands)

    // stm32l073z_eval_sd.c
    static uint8_t SD_GetCIDRegister(SD_CID* Cid);
    static uint8_t SD_GetCSDRegister(SD_CSD* Csd);
    static uint8_t SD_GetDataResponse(void);
    static uint8_t SD_GoIdleState(void);
    static SD_CmdAnswer_typedef SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Answer);
    static uint8_t SD_WaitData(uint8_t data);
    static uint8_t SD_ReadData(void);

#### BSP SD functions

    // stm32l073z_eval_sd.c
    uint8_t BSP_SD_Init(void);
    uint8_t BSP_SD_IsDetected(void);
    uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
    uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
    uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
    uint8_t BSP_SD_GetStatus(void);
    uint8_t BSP_SD_GetCardInfo(SD_CardInfo *pCardInfo);

#### FATFS Disk I/O driver

    // sd_diskio.c
    const Diskio_drvTypeDef  SD_Driver = {
        SD_initialize,  // BSP_SD_Init
        SD_status,      // BSP_SD_GetStatus
        SD_read,        // BSP_SD_ReadBlocks
        SD_write,       // BSP_SD_WriteBlocks
        SD_ioctl,       // BSP_SD_GetCardInfo
    };

#### Link driver
    
    // ff_gen_drv.c
    FRESULT result = FATFS_LinkDriver(&SD_Driver, SDPath);
    // returns 0 on success

---






