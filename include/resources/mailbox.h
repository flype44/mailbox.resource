#ifndef RESOURCES_MAILBOX_H
#define RESOURCES_MAILBOX_H
/*
**	$VER: mailbox.h 1.2
**
**	VideoCore mailbox property-channel tag definitions for "mailbox.resource".
**
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define MAILBOXNAME "mailbox.resource"

/* channels */
#define MB_CHANNEL_POWER_MANAGEMENT (0)
#define MB_CHANNEL_FRAMEBUFFER      (1) /* legacy, pre-property-tag interface */
#define MB_CHANNEL_VIRTUAL_UART     (2)
#define MB_CHANNEL_VCHIQ            (3)
#define MB_CHANNEL_LEDS             (4)
#define MB_CHANNEL_BUTTONS          (5)
#define MB_CHANNEL_TOUCHSCREEN      (6)
#define MB_CHANNEL_TAGS_ARM_TO_VC   (8) /* the one this resource uses */
#define MB_CHANNEL_TAGS_VC_TO_ARM   (9)

/* Mailbox */
#define MB_SUCCESS              (0x80000000)

/* Mailbox tags */
#define MB_GET_FIRMWARE_REV     (0x00000001)
#define MB_SET_CURSOR_INFO      (0x00008010)
#define MB_SET_CURSOR_STATE     (0x00008011)
#define MB_SET_SCREEN_GAMMA     (0x00008012)
#define MB_GET_BOARD_MODEL      (0x00010001)
#define MB_GET_BOARD_REVISION   (0x00010002)
#define MB_GET_BOARD_MACADDR    (0x00010003)
#define MB_GET_BOARD_SERIAL     (0x00010004)
#define MB_GET_ARM_MEMORY       (0x00010005)
#define MB_GET_VC_MEMORY        (0x00010006)
#define MB_GET_CLOCKS           (0x00010007)
#define MB_GET_POWER_STATE      (0x00020001)
#define MB_GET_POWER_TIMING     (0x00020002)
#define MB_SET_POWER_STATE      (0x00028001)
#define MB_GET_CLOCK_STATE      (0x00030001)
#define MB_GET_CLOCK_RATE       (0x00030002)
#define MB_GET_VOLTAGE          (0x00030003)
#define MB_GET_CLOCK_RATE_MAX   (0x00030004)
#define MB_GET_VOLTAGE_MAX      (0x00030005)
#define MB_GET_TEMPERATURE      (0x00030006)
#define MB_GET_CLOCK_RATE_MIN   (0x00030007)
#define MB_GET_VOLTAGE_MIN      (0x00030008)
#define MB_GET_TURBO            (0x00030009)
#define MB_GET_TEMPERATURE_MAX  (0x0003000a)
#define MB_ALLOCATE_MEMORY      (0x0003000c)
#define MB_LOCK_MEMORY          (0x0003000d)
#define MB_UNLOCK_MEMORY        (0x0003000e)
#define MB_RELEASE_MEMORY       (0x0003000f)
#define MB_GET_EXECUTE_CODE     (0x00030010)
#define MB_GET_DISPMANX_HANDLE  (0x00030014)
#define MB_GET_EDID_BLOCK       (0x00030020)
#define MB_GET_LED_STATUS       (0x00030041)
#define MB_GET_CLOCK_RATE_M     (0x00030047)
#define MB_GET_GENCMD_RESULT    (0x00030080)
#define MB_TEST_LED_STATUS      (0x00034041)
#define MB_SET_CLOCK_STATE      (0x00038001)
#define MB_SET_CLOCK_RATE       (0x00038002)
#define MB_SET_VOLTAGE          (0x00038003)
#define MB_SET_TURBO            (0x00038009)
#define MB_SET_LED_STATUS       (0x00038041)
#define MB_ALLOCATE_BUFFER      (0x00040001)
#define MB_BLANK_SCREEN         (0x00040002)
#define MB_GET_PHYSICAL_SIZE    (0x00040003)
#define MB_GET_VIRTUAL_SIZE     (0x00040004)
#define MB_GET_DEPTH            (0x00040005)
#define MB_GET_PIXEL_ORDER      (0x00040006)
#define MB_GET_ALPHA_MODE       (0x00040007)
#define MB_GET_PITCH            (0x00040008)
#define MB_GET_VIRTUAL_OFFSET   (0x00040009)
#define MB_GET_OVERSCAN         (0x0004000a)
#define MB_GET_PALETTE          (0x0004000b)
#define MB_TEST_PHYSICAL_SIZE   (0x00044003)
#define MB_TEST_VIRTUAL_SIZE    (0x00044004)
#define MB_TEST_DEPTH           (0x00044005)
#define MB_TEST_PIXEL_ORDER     (0x00044006)
#define MB_TEST_ALPHA_MODE      (0x00044007)
#define MB_TEST_VIRTUAL_OFFSET  (0x00044009)
#define MB_TEST_OVERSCAN        (0x0004400a)
#define MB_TEST_PALETTE         (0x0004400b)
#define MB_RELEASE_BUFFER       (0x00048001)
#define MB_SET_PHYSICAL_SIZE    (0x00048003)
#define MB_SET_VIRTUAL_SIZE     (0x00048004)
#define MB_SET_DEPTH            (0x00048005)
#define MB_SET_PIXEL_ORDER      (0x00048006)
#define MB_SET_ALPHA_MODE       (0x00048007)
#define MB_SET_VIRTUAL_OFFSET   (0x00048009)
#define MB_SET_OVERSCAN         (0x0004800a)
#define MB_SET_PALETTE          (0x0004800b)
#define MB_SET_DISPLAY_NUM      (0x00048013)
#define MB_GET_COMMAND_LINE     (0x00050001)
#define MB_GET_DMA_CHANNELS     (0x00060001)

/* led id */
#define LED_PIN_STATUS          (42)
#define LED_PIN_POWER           (130)

/* clock id */
#define CLK_EMMC_ID             (0x1)
#define CLK_UART_ID             (0x2)
#define CLK_ARM_ID              (0x3)
#define CLK_CORE_ID             (0x4)
#define CLK_V3D_ID              (0x5)
#define CLK_H264_ID             (0x6)
#define CLK_ISP_ID              (0x7)
#define CLK_SDRAM_ID            (0x8)
#define CLK_PIXEL_ID            (0x9)
#define CLK_PWM_ID              (0xA)
#define CLK_HEVC_ID             (0xB)
#define CLK_EMMC2_ID            (0xC)
#define CLK_M2MC_ID             (0xD)
#define CLK_PIXEL_BVB_ID        (0xE)
#define CLK_MAX_ID              (0xF)

/* power id */
#define POWER_SDCARD            (0x0)
#define POWER_UART0             (0x1)
#define POWER_UART1             (0x2)
#define POWER_USBHCD            (0x3)
#define POWER_I2C0              (0x4)
#define POWER_I2C1              (0x5)
#define POWER_I2C2              (0x6)
#define POWER_SPI               (0x7)
#define POWER_CCP2TX            (0x8)
#define POWER_UNKNOWN1          (0x9)
#define POWER_UNKNOWN2          (0xA)

/* voltage id */
#define VOLTAGE_ID_RESERVED     (0x0)
#define VOLTAGE_ID_CORE         (0x1)
#define VOLTAGE_ID_SDRAM_C      (0x2)
#define VOLTAGE_ID_SDRAM_P      (0x3)
#define VOLTAGE_ID_SDRAM_I      (0x4)

/* AllocateMemory() 'flags' bitmask */
#define MEM_FLAG_DISCARDABLE       (1 << 0)  /* can be resized to 0 at any time */
#define MEM_FLAG_NORMAL            (0 << 2)  /* normal allocating alias */
#define MEM_FLAG_DIRECT            (1 << 2)  /* 0xC alias uncached */
#define MEM_FLAG_COHERENT          (2 << 2)  /* 0x8 alias, cache coherent */
#define MEM_FLAG_L1_NONALLOCATING  ((1 << 2) | (2 << 2)) /* DIRECT | COHERENT */
#define MEM_FLAG_ZERO              (1 << 4)  /* initialise buffer to zero */
#define MEM_FLAG_NO_INIT           (1 << 5)  /* don't initialise (default is initialise to 0) */
#define MEM_FLAG_HINT_PERMALOCK    (1 << 6)  /* likely to be locked for long periods of time */

#endif /* RESOURCES_MAILBOX_H */
