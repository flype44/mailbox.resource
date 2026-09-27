#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* overscan must point to an array of 4 ULONGs, filled with
   [top, bottom, left, right] on return. Returns 0 on success. */
ULONG L_GetOverscan(REGARG(ULONG *overscan, "a0"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;
    ULONG retval = 0;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 10*4;

    FBReq[0] = LE32(4*10);      // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x0004000a);// GET_OVERSCAN
    FBReq[3] = LE32(16);
    FBReq[4] = 0;
    FBReq[5] = 0;
    FBReq[6] = 0;
    FBReq[7] = 0;
    FBReq[8] = 0;
    FBReq[9] = 0;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
    {
        retval = -1;
    }
    else if (overscan)
    {
        overscan[0] = LE32(FBReq[5]);
        overscan[1] = LE32(FBReq[6]);
        overscan[2] = LE32(FBReq[7]);
        overscan[3] = LE32(FBReq[8]);
    }

    ReleaseSemaphore(&MBBase->mb_Lock);

    return retval;
}
