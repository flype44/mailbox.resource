#ifndef _MAILBOX_H
#define _MAILBOX_H

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <common/compiler.h>

#define MB_PRIORITY     119
#define MB_VERSION      1
/* Revision history:
   0: original release (RawCommand, StringCommand, Get/SetClockRate,
      Get/SetClockState, Get/SetPowerState, Get/SetGPIOState).
   1: added 46 functions (board identity, clocks, temperature/voltage,
      framebuffer, GPU memory, EDID, ...).
   2: version/packaging bump only, no new functions.
   Client code MUST check lib_Version/lib_Revision (via OpenResource()'s
   returned base, cast to struct Library *) against MB_VERSION/
   MB_MIN_REVISION_* in resources/mailbox.h before calling anything beyond
   the original set -- see that header and Autodocs/mailbox.doc. Bump this
   again on the next round of new functions, and add a new
   MB_MIN_REVISION_* threshold for them in resources/mailbox.h. */
#define MB_REVISION     2

struct MailboxBase {
    struct Library          mb_Node;
    struct ExecBase *       mb_ExecBase;
    struct SignalSemaphore  mb_Lock;
    APTR                    mb_MailBox;
    APTR                    mb_RequestBase;
    ULONG *                 mb_Request;
};

#define NUMBER_OF_FUNCTIONS 56
#define BASE_NEG_SIZE (NUMBER_OF_FUNCTIONS * 6)
#define BASE_POS_SIZE ((sizeof(struct MailboxBase)))

void mbox_send(ULONG channel, ULONG data, struct MailboxBase * Base);
ULONG mbox_recv(ULONG channel, struct MailboxBase * Base);

static inline ULONG LE32(ULONG x) { return __builtin_bswap32(x); }

static inline void wr32le(volatile ULONG *addr, ULONG value) {
    *addr = LE32(value);
    asm volatile("nop");
}

void kprintf(REGARG(const char * msg, "a0"), REGARG(void * args, "a1"));

#define bug(string, ...) \
    do { ULONG args[] = {0, __VA_ARGS__}; kprintf(string, &args[1]); } while(0)

VOID L_RawCommand(REGARG(ULONG * command, "a0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_StringCommand(REGARG(STRPTR command, "a0"), REGARG(STRPTR reply, "a1"), REGARG(ULONG reply_capacity, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetClockRate(REGARG(ULONG clock_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetClockRate(REGARG(ULONG clock_id, "d0"), REGARG(ULONG speed, "d1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetClockState(REGARG(ULONG clock_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetClockState(REGARG(ULONG clock_id, "d0"), REGARG(ULONG state, "d1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetPowerState(REGARG(ULONG id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetPowerState(REGARG(ULONG id, "d0"), REGARG(ULONG state, "d1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetGPIOState(REGARG(ULONG gpio, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetGPIOState(REGARG(ULONG gpio, "d0"), REGARG(ULONG state, "d1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetFirmwareRevision(REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetBoardModel(REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetBoardRevision(REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_GetBoardMACAddress(REGARG(ULONG *hi, "a0"), REGARG(ULONG *lo, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_GetBoardSerial(REGARG(ULONG *hi, "a0"), REGARG(ULONG *lo, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_GetARMMemory(REGARG(ULONG *base, "a0"), REGARG(ULONG *size, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_GetVCMemory(REGARG(ULONG *base, "a0"), REGARG(ULONG *size, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetClockRateMeasured(REGARG(ULONG clock_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetClockRateMin(REGARG(ULONG clock_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetClockRateMax(REGARG(ULONG clock_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetTemperature(REGARG(ULONG temp_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetTemperatureMax(REGARG(ULONG temp_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetVoltage(REGARG(ULONG voltage_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetVoltageMin(REGARG(ULONG voltage_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetVoltageMax(REGARG(ULONG voltage_id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetTurbo(REGARG(ULONG id, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetTurbo(REGARG(ULONG id, "d0"), REGARG(ULONG level, "d1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetDisplayNum(REGARG(ULONG display_num, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_GetPhysicalSize(REGARG(ULONG *width, "a0"), REGARG(ULONG *height, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_SetPhysicalSize(REGARG(ULONG width, "d0"), REGARG(ULONG height, "d1"), REGARG(ULONG *actual_width, "a0"), REGARG(ULONG *actual_height, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_GetVirtualSize(REGARG(ULONG *width, "a0"), REGARG(ULONG *height, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_SetVirtualSize(REGARG(ULONG width, "d0"), REGARG(ULONG height, "d1"), REGARG(ULONG *actual_width, "a0"), REGARG(ULONG *actual_height, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetDepth(REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetDepth(REGARG(ULONG depth, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetPitch(REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_AllocateBuffer(REGARG(ULONG alignment, "d0"), REGARG(ULONG *base, "a0"), REGARG(ULONG *size, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_ReleaseBuffer(REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_BlankScreen(REGARG(ULONG blank, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_AllocateMemory(REGARG(ULONG size, "d0"), REGARG(ULONG alignment, "d1"), REGARG(ULONG flags, "d2"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_LockMemory(REGARG(ULONG handle, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_UnlockMemory(REGARG(ULONG handle, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_ReleaseMemory(REGARG(ULONG handle, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetVoltage(REGARG(ULONG voltage_id, "d0"), REGARG(LONG value, "d1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetPixelOrder(REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetPixelOrder(REGARG(ULONG order, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetAlphaMode(REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetAlphaMode(REGARG(ULONG mode, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_GetVirtualOffset(REGARG(ULONG *x, "a0"), REGARG(ULONG *y, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
VOID L_SetVirtualOffset(REGARG(ULONG x, "d0"), REGARG(ULONG y, "d1"), REGARG(ULONG *actual_x, "a0"), REGARG(ULONG *actual_y, "a1"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetOverscan(REGARG(ULONG *overscan, "a0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetOverscan(REGARG(ULONG *overscan, "a0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetDMAChannels(REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetEDIDBlock(REGARG(ULONG block_number, "d0"), REGARG(UBYTE *edid_out, "a0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetCommandLine(REGARG(STRPTR reply, "a0"), REGARG(ULONG reply_capacity, "d0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_GetPalette(REGARG(ULONG *palette, "a0"), REGARG(struct MailboxBase *MBBase, "a6"));
ULONG L_SetPalette(REGARG(ULONG offset, "d0"), REGARG(ULONG count, "d1"), REGARG(ULONG *colors, "a0"), REGARG(struct MailboxBase *MBBase, "a6"));



#endif /* _MAILBOX_H */
