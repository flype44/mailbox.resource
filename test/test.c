/*
    mailbox_test -- runs every mailbox.doc EXAMPLES snippet in
    sequence, one function per AutoDoc entry, generated directly
    from the same entries.json that produced Autodocs/mailbox.doc.

    Every MB_Set*() call site in main() is wrapped in #if 0: these
    run with illustrative literal values (not read-back/round-
    tripped) exactly as shown in the doc, and risk hanging the host
    running this test (e.g. SetClockRate/SetVoltage on the wrong
    clock/rail, or SetPhysicalSize/SetVirtualSize/SetDepth against
    the live framebuffer). Flip one on deliberately, one at a time,
    only once you're ready for that risk.

    MB_BlankScreen()/MB_AllocateBuffer() are NOT disabled and still
    reallocate/blank the live framebuffer when this runs -- expect
    visible display disruption.
*/

#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/libraries.h>
#include <dos/dosextens.h>
#include <workbench/startup.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/mailbox.h>

#include <resources/mailbox.h>

struct DosLibrary * DOSBase;
struct ExecBase * SysBase;
APTR MailboxBase;

static VOID Test_MB_AllocateBuffer(VOID)
{
    ULONG base, size;

    MB_AllocateBuffer(64, &base, &size);
    Printf("Framebuffer: %lx, %ld bytes\n", base, size);

    /* ... use the framebuffer ... */

    MB_ReleaseBuffer();
}

static VOID Test_MB_AllocateMemory(VOID)
{
    UBYTE testCode[16] = { 0 };
    ULONG handle, busAddr;
    UBYTE *vc4Code;

    handle = MB_AllocateMemory(sizeof(testCode), 4,
        MEM_FLAG_COHERENT | MEM_FLAG_DIRECT | MEM_FLAG_HINT_PERMALOCK);
    Printf("MB_AllocateMemory: handle=%08lx\n", handle);

    if (handle && handle != (ULONG)-1)
    {
        busAddr = MB_LockMemory(handle);
        vc4Code = (UBYTE *)(busAddr & 0x3fffffff);
        Printf("MB_LockMemory: busAddr=%08lx\n", busAddr);

        CopyMem(testCode, vc4Code, sizeof(testCode));

        MB_UnlockMemory(handle);
        MB_ReleaseMemory(handle);
    }
}

static VOID Test_MB_BlankScreen(VOID)
{
    ULONG state = MB_BlankScreen(TRUE);
    Printf("Screen blanked: %ld\n", state);
}

static VOID Test_MB_GetAlphaMode(VOID)
{
    ULONG mode = MB_GetAlphaMode();
    Printf("Alpha mode: %ld\n", mode);
}

static VOID Test_MB_GetARMMemory(VOID)
{
    ULONG base, size;

    MB_GetARMMemory(&base, &size);
    Printf("ARM memory: %lx, %ld bytes\n", base, size);
}

static VOID Test_MB_GetBoardMACAddress(VOID)
{
    ULONG hi, lo;

    MB_GetBoardMACAddress(&hi, &lo);
    Printf("MAC: %08lx%04lx\n", hi, lo >> 16);
}

static VOID Test_MB_GetBoardModel(VOID)
{
    ULONG model = MB_GetBoardModel();
    Printf("Board model: %ld\n", model);
}

static VOID Test_MB_GetBoardRevision(VOID)
{
    ULONG revision = MB_GetBoardRevision();
    Printf("Board revision: %08lx\n", revision);
}

static VOID Test_MB_GetBoardSerial(VOID)
{
    ULONG hi, lo;

    MB_GetBoardSerial(&hi, &lo);
    Printf("Serial: %08lx%08lx\n", hi, lo);
}

static VOID Test_MB_GetClockRate(VOID)
{
    ULONG rate = MB_GetClockRate(CLK_ARM_ID);
    Printf("ARM clock: %ld Hz\n", rate);
}

static VOID Test_MB_GetClockRateMax(VOID)
{
    ULONG rate = MB_GetClockRateMax(CLK_ARM_ID);
    Printf("ARM clock max: %ld Hz\n", rate);
}

static VOID Test_MB_GetClockRateMeasured(VOID)
{
    ULONG rate = MB_GetClockRateMeasured(CLK_ARM_ID);
    Printf("ARM clock measured: %ld Hz\n", rate);
}

static VOID Test_MB_GetClockRateMin(VOID)
{
    ULONG rate = MB_GetClockRateMin(CLK_ARM_ID);
    Printf("ARM clock min: %ld Hz\n", rate);
}

static VOID Test_MB_GetClockState(VOID)
{
    ULONG state = MB_GetClockState(CLK_ARM_ID);
    Printf("ARM clock state: %ld\n", state);
}

static VOID Test_MB_GetCommandLine(VOID)
{
    UBYTE cmdline[256];

    MB_GetCommandLine(cmdline, sizeof(cmdline));
    Printf("Boot args: %s\n", cmdline);
}

static VOID Test_MB_GetDepth(VOID)
{
    ULONG depth = MB_GetDepth();
    Printf("Depth: %ld bpp\n", depth);
}

static VOID Test_MB_GetDMAChannels(VOID)
{
    ULONG channels = MB_GetDMAChannels();
    Printf("DMA channels: %08lx\n", channels);
}

static VOID Test_MB_GetEDIDBlock(VOID)
{
    #define EDID_BLOCK_SIZE  (128)
    #define EDID_BLOCK_COUNT (256)

    static UBYTE edidData[EDID_BLOCK_SIZE * EDID_BLOCK_COUNT];
    UBYTE *buffer = edidData;

    if (MB_GetEDIDBlock(0, buffer) == 0)
    {
        UBYTE extensions = buffer[126];
        buffer += EDID_BLOCK_SIZE;

        if (extensions > 0 && extensions < EDID_BLOCK_COUNT)
        {
            ULONG block;

            for (block = 0; block < extensions; block++)
            {
                if (MB_GetEDIDBlock(block + 1, buffer) != 0)
                    break;

                buffer += EDID_BLOCK_SIZE;
            }
        }

        Printf("MB_GetEDIDBlock: read %ld byte(s)\n",
            (ULONG)(buffer - edidData));
    }
    else
    {
        Printf("MB_GetEDIDBlock: FAIL (no display connected?)\n");
    }
}

static VOID Test_MB_GetFirmwareRevision(VOID)
{
    ULONG revision = MB_GetFirmwareRevision();
    Printf("Firmware revision: %ld\n", revision);
}

static VOID Test_MB_GetGPIOState(VOID)
{
    ULONG state = MB_GetGPIOState(LED_PIN_STATUS);
    Printf("Status LED: %ld\n", state);
}

static VOID Test_MB_GetOverscan(VOID)
{
    ULONG overscan[4];

    MB_GetOverscan(overscan);
    Printf("Overscan: %ld %ld %ld %ld\n",
        overscan[0], overscan[1], overscan[2], overscan[3]);
}

static VOID Test_MB_GetPalette(VOID)
{
    ULONG palette[256];

    MB_GetPalette(palette);
    Printf("Colour 1: %08lx\n", palette[1]);
}

static VOID Test_MB_GetPhysicalSize(VOID)
{
    ULONG display, width, height;

    for (display = 0; display < 2; display++)
    {
        MB_SetDisplayNum(display);
        MB_GetPhysicalSize(&width, &height);

        Printf("HDMI%ld: %ld x %ld\n", display, width, height);
    }
}

static VOID Test_MB_GetPitch(VOID)
{
    ULONG pitch = MB_GetPitch();
    Printf("Pitch: %ld bytes/line\n", pitch);
}

static VOID Test_MB_GetPixelOrder(VOID)
{
    ULONG order = MB_GetPixelOrder();
    Printf("Pixel order: %ld\n", order);
}

static VOID Test_MB_GetPowerState(VOID)
{
    ULONG state = MB_GetPowerState(POWER_SDCARD);
    Printf("SD card power: %ld\n", state);
}

static VOID Test_MB_GetTemperature(VOID)
{
    ULONG milli = MB_GetTemperature(0);
    Printf("Temperature: %ld.%03ld C\n", milli / 1000, milli % 1000);
}

static VOID Test_MB_GetTemperatureMax(VOID)
{
    ULONG milli = MB_GetTemperatureMax(0);
    Printf("Max temperature: %ld.%03ld C\n", milli / 1000, milli % 1000);
}

static VOID Test_MB_GetTurbo(VOID)
{
    ULONG level = MB_GetTurbo(0);
    Printf("Turbo level: %ld\n", level);
}

static VOID Test_MB_GetVCMemory(VOID)
{
    ULONG base, size;

    MB_GetVCMemory(&base, &size);
    Printf("VC memory: %lx, %ld bytes\n", base, size);
}

static VOID Test_MB_GetVirtualOffset(VOID)
{
    ULONG x, y;

    MB_GetVirtualOffset(&x, &y);
    Printf("Pan offset: %ld, %ld\n", x, y);
}

static VOID Test_MB_GetVirtualSize(VOID)
{
    ULONG width, height;

    MB_GetVirtualSize(&width, &height);
    Printf("Virtual size: %ld x %ld\n", width, height);
}

static VOID Test_MB_GetVoltage(VOID)
{
    ULONG offset = MB_GetVoltage(VOLTAGE_ID_CORE);
    Printf("Core voltage offset: %ld\n", offset);
}

static VOID Test_MB_GetVoltageMax(VOID)
{
    ULONG offset = MB_GetVoltageMax(VOLTAGE_ID_CORE);
    Printf("Core voltage max offset: %ld\n", offset);
}

static VOID Test_MB_GetVoltageMin(VOID)
{
    ULONG offset = MB_GetVoltageMin(VOLTAGE_ID_CORE);
    Printf("Core voltage min offset: %ld\n", offset);
}

static VOID Test_MB_LockMemory(VOID)
{
    ULONG handle = MB_AllocateMemory(4096, 4, MEM_FLAG_COHERENT);
    ULONG busAddr = MB_LockMemory(handle);

    Printf("Bus address: %lx\n", busAddr);

    MB_UnlockMemory(handle);
    MB_ReleaseMemory(handle);
}

static VOID Test_MB_RawCommand(VOID)
{
    ULONG command[7] = { 7 * 4, 0, 0x00000001, 4, 0, 0, 0 };

    MB_RawCommand(command);
    Printf("Firmware revision: %ld\n", command[5]);
}

static VOID Test_MB_ReleaseBuffer(VOID)
{
    /* ... MB_AllocateBuffer(), use the framebuffer ... */

    MB_ReleaseBuffer();
}

static VOID Test_MB_ReleaseMemory(VOID)
{
    ULONG handle = MB_AllocateMemory(4096, 4, MEM_FLAG_COHERENT);

    /* ... use the block via MB_LockMemory() ... */

    MB_ReleaseMemory(handle);
}

static VOID Test_MB_SetAlphaMode(VOID)
{
    ULONG mode = MB_SetAlphaMode(2);
    Printf("Alpha mode: %ld\n", mode);
}

static VOID Test_MB_SetClockRate(VOID)
{
    ULONG rate = MB_SetClockRate(CLK_ARM_ID, 600000000);
    Printf("ARM clock set to: %ld Hz\n", rate);
}

static VOID Test_MB_SetClockState(VOID)
{
    ULONG state = MB_SetClockState(CLK_UART_ID, 1);
    Printf("UART clock state: %ld\n", state);
}

static VOID Test_MB_SetDepth(VOID)
{
    ULONG depth = MB_SetDepth(16);
    Printf("Depth set to: %ld bpp\n", depth);
}

static VOID Test_MB_SetDisplayNum(VOID)
{
    ULONG display = 1; /* HDMI1 */

    if (MB_SetDisplayNum(display) == display)
    {
        ULONG width, height;

        MB_GetPhysicalSize(&width, &height);
        Printf("HDMI%ld: %ld x %ld\n", display, width, height);
    }
    else
    {
        Printf("HDMI%ld: not available\n", display);
    }

    MB_SetDisplayNum(0); /* restore */
}

static VOID Test_MB_SetGPIOState(VOID)
{
    ULONG state = MB_SetGPIOState(LED_PIN_STATUS, 1);
    Printf("Status LED set to: %ld\n", state);
}

static VOID Test_MB_SetOverscan(VOID)
{
    ULONG overscan[4] = { 0, 0, 0, 0 };

    MB_SetOverscan(overscan);
    Printf("Overscan applied: %ld %ld %ld %ld\n",
        overscan[0], overscan[1], overscan[2], overscan[3]);
}

static VOID Test_MB_SetPalette(VOID)
{
    ULONG red = 0xff0000ff;

    ULONG status = MB_SetPalette(1, 1, &red);
    Printf("MB_SetPalette status: %ld\n", status);
}

static VOID Test_MB_SetPhysicalSize(VOID)
{
    ULONG width, height;

    MB_SetPhysicalSize(1920, 1080, &width, &height);
    Printf("Physical size applied: %ld x %ld\n", width, height);
}

static VOID Test_MB_SetPixelOrder(VOID)
{
    ULONG order = MB_SetPixelOrder(1);
    Printf("Pixel order set to: %ld\n", order);
}

static VOID Test_MB_SetPowerState(VOID)
{
    ULONG state = MB_SetPowerState(POWER_USBHCD, 1);
    Printf("USB power set to: %ld\n", state);
}

static VOID Test_MB_SetTurbo(VOID)
{
    ULONG level = MB_SetTurbo(0, 1);
    Printf("Turbo level set to: %ld\n", level);
}

static VOID Test_MB_SetVirtualOffset(VOID)
{
    ULONG x, y;

    MB_SetVirtualOffset(0, 0, &x, &y);
    Printf("Pan offset applied: %ld, %ld\n", x, y);
}

static VOID Test_MB_SetVirtualSize(VOID)
{
    ULONG width, height;

    MB_SetVirtualSize(1920, 1080, &width, &height);
    Printf("Virtual size applied: %ld x %ld\n", width, height);
}

static VOID Test_MB_SetVoltage(VOID)
{
    ULONG offset = MB_SetVoltage(VOLTAGE_ID_CORE, -2);
    Printf("Core voltage offset set to: %ld\n", offset);
}

static VOID Test_MB_StringCommand(VOID)
{
    UBYTE reply[256];

    MB_StringCommand("measure_temp", reply, sizeof(reply));
    Printf("vcgencmd reply: %s\n", reply);
}

static VOID Test_MB_UnlockMemory(VOID)
{
    ULONG handle = MB_AllocateMemory(4096, 4, MEM_FLAG_COHERENT);
    ULONG busAddr = MB_LockMemory(handle);

    /* ... use the block at busAddr ... */

    MB_UnlockMemory(handle);
}

int main(int argc, struct WBStartup *wbmsg)
{
    struct Library *lib;

    SysBase = *(struct ExecBase **)4;

    DOSBase = (struct DosLibrary *)OpenLibrary("dos.library", 37);
    if (DOSBase == NULL)
        return 20;

    MailboxBase = OpenResource(MAILBOXNAME);
    if (MailboxBase == NULL)
    {
        Printf("Can't open mailbox.resource!\n");
        CloseLibrary((struct Library *)DOSBase);
        return 20;
    }

    lib = (struct Library *)MailboxBase;

    Printf("mailbox.resource version %ld.%ld\n",
        (ULONG)lib->lib_Version, (ULONG)lib->lib_Revision);

    if (lib->lib_Version == 1 && lib->lib_Revision == 0)
    {
        Printf("\n-- mailbox.resource version 1.0 functions --\n");
        Printf("\n-- MB_GetClockRate --\n");
        Test_MB_GetClockRate();
        Printf("\n-- MB_GetClockState --\n");
        Test_MB_GetClockState();
        Printf("\n-- MB_GetGPIOState --\n");
        Test_MB_GetGPIOState();
        Printf("\n-- MB_GetPowerState --\n");
        Test_MB_GetPowerState();
        Printf("\n-- MB_RawCommand --\n");
        Test_MB_RawCommand();
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetClockRate --\n");
        Test_MB_SetClockRate();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetClockState --\n");
        Test_MB_SetClockState();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetGPIOState --\n");
        Test_MB_SetGPIOState();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetPowerState --\n");
        Test_MB_SetPowerState();
        #endif
        Printf("\n-- MB_StringCommand --\n");
        Test_MB_StringCommand();
    }

    if (lib->lib_Version == 1 && lib->lib_Revision == 1)
    {
        Printf("\n-- mailbox.resource version 1.1 functions --\n");
        Printf("\n-- MB_AllocateBuffer --\n");
        Test_MB_AllocateBuffer();
        Printf("\n-- MB_AllocateMemory --\n");
        Test_MB_AllocateMemory();
        Printf("\n-- MB_BlankScreen --\n");
        Test_MB_BlankScreen();
        Printf("\n-- MB_GetAlphaMode --\n");
        Test_MB_GetAlphaMode();
        Printf("\n-- MB_GetARMMemory --\n");
        Test_MB_GetARMMemory();
        Printf("\n-- MB_GetBoardMACAddress --\n");
        Test_MB_GetBoardMACAddress();
        Printf("\n-- MB_GetBoardModel --\n");
        Test_MB_GetBoardModel();
        Printf("\n-- MB_GetBoardRevision --\n");
        Test_MB_GetBoardRevision();
        Printf("\n-- MB_GetBoardSerial --\n");
        Test_MB_GetBoardSerial();
        Printf("\n-- MB_GetClockRateMax --\n");
        Test_MB_GetClockRateMax();
        Printf("\n-- MB_GetClockRateMeasured --\n");
        Test_MB_GetClockRateMeasured();
        Printf("\n-- MB_GetClockRateMin --\n");
        Test_MB_GetClockRateMin();
        Printf("\n-- MB_GetCommandLine --\n");
        Test_MB_GetCommandLine();
        Printf("\n-- MB_GetDepth --\n");
        Test_MB_GetDepth();
        Printf("\n-- MB_GetDMAChannels --\n");
        Test_MB_GetDMAChannels();
        Printf("\n-- MB_GetEDIDBlock --\n");
        Test_MB_GetEDIDBlock();
        Printf("\n-- MB_GetFirmwareRevision --\n");
        Test_MB_GetFirmwareRevision();
        Printf("\n-- MB_GetOverscan --\n");
        Test_MB_GetOverscan();
        Printf("\n-- MB_GetPalette --\n");
        Test_MB_GetPalette();
        Printf("\n-- MB_GetPhysicalSize --\n");
        Test_MB_GetPhysicalSize();
        Printf("\n-- MB_GetPitch --\n");
        Test_MB_GetPitch();
        Printf("\n-- MB_GetPixelOrder --\n");
        Test_MB_GetPixelOrder();
        Printf("\n-- MB_GetTemperature --\n");
        Test_MB_GetTemperature();
        Printf("\n-- MB_GetTemperatureMax --\n");
        Test_MB_GetTemperatureMax();
        Printf("\n-- MB_GetTurbo --\n");
        Test_MB_GetTurbo();
        Printf("\n-- MB_GetVCMemory --\n");
        Test_MB_GetVCMemory();
        Printf("\n-- MB_GetVirtualOffset --\n");
        Test_MB_GetVirtualOffset();
        Printf("\n-- MB_GetVirtualSize --\n");
        Test_MB_GetVirtualSize();
        Printf("\n-- MB_GetVoltage --\n");
        Test_MB_GetVoltage();
        Printf("\n-- MB_GetVoltageMax --\n");
        Test_MB_GetVoltageMax();
        Printf("\n-- MB_GetVoltageMin --\n");
        Test_MB_GetVoltageMin();
        Printf("\n-- MB_LockMemory --\n");
        Test_MB_LockMemory();
        Printf("\n-- MB_ReleaseBuffer --\n");
        Test_MB_ReleaseBuffer();
        Printf("\n-- MB_ReleaseMemory --\n");
        Test_MB_ReleaseMemory();
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetAlphaMode --\n");
        Test_MB_SetAlphaMode();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetDepth --\n");
        Test_MB_SetDepth();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetDisplayNum --\n");
        Test_MB_SetDisplayNum();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetOverscan --\n");
        Test_MB_SetOverscan();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetPalette --\n");
        Test_MB_SetPalette();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetPhysicalSize --\n");
        Test_MB_SetPhysicalSize();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetPixelOrder --\n");
        Test_MB_SetPixelOrder();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetTurbo --\n");
        Test_MB_SetTurbo();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetVirtualOffset --\n");
        Test_MB_SetVirtualOffset();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetVirtualSize --\n");
        Test_MB_SetVirtualSize();
        #endif
        #if 0 /* mutates live hardware state -- see the file header */
        Printf("\n-- MB_SetVoltage --\n");
        Test_MB_SetVoltage();
        #endif
        Printf("\n-- MB_UnlockMemory --\n");
        Test_MB_UnlockMemory();
    }

    CloseLibrary((struct Library *)DOSBase);

    return 0;
}
