#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* 0=BGR, 1=RGB */
ULONG L_SetPixelOrder(REGARG(ULONG order, "d0"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;
    ULONG retval = 0;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 7*4;

    FBReq[0] = LE32(4*7);       // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x00048006);// SET_PIXEL_ORDER
    FBReq[3] = LE32(4);
    FBReq[4] = 0;
    FBReq[5] = LE32(order);
    FBReq[6] = 0;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
        retval = -1;
    else
        retval = LE32(FBReq[5]);

    ReleaseSemaphore(&MBBase->mb_Lock);

    return retval;
}
