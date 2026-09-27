#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* Board serial number (64 bit, as 2 words). Unlike every other property, the
   firmware hands this one back as a raw byte blob rather than two proper
   32-bit values, so it needs no byte-swap at all - confirmed by working out
   the math from an actual wrong-looking reading (with a swap applied,
   "10000000c937f849" - the expected RPi4 new-style serial - came out as
   "0000001049f837c9"). */
VOID L_GetBoardSerial(REGARG(ULONG *hi, "a0"), REGARG(ULONG *lo, "a1"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 8*4;

    FBReq[0] = LE32(4*8);       // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x00010004);// GET_BOARD_SERIAL
    FBReq[3] = LE32(8);
    FBReq[4] = 0;
    FBReq[5] = 0;
    FBReq[6] = 0;
    FBReq[7] = 0;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
    {
        if (hi) *hi = -1;
        if (lo) *lo = -1;
    }
    else
    {
        if (hi) *hi = FBReq[5];
        if (lo) *lo = FBReq[6];
    }

    ReleaseSemaphore(&MBBase->mb_Lock);
}
