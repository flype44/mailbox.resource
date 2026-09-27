#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

VOID L_SetVirtualOffset(REGARG(ULONG x, "d0"), REGARG(ULONG y, "d1"), REGARG(ULONG *actual_x, "a0"), REGARG(ULONG *actual_y, "a1"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 8*4;

    FBReq[0] = LE32(4*8);       // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x00048009);// SET_VIRTUAL_OFFSET
    FBReq[3] = LE32(8);
    FBReq[4] = 0;
    FBReq[5] = LE32(x);
    FBReq[6] = LE32(y);
    FBReq[7] = 0;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
    {
        if (actual_x) *actual_x = -1;
        if (actual_y) *actual_y = -1;
    }
    else
    {
        if (actual_x) *actual_x = LE32(FBReq[5]);
        if (actual_y) *actual_y = LE32(FBReq[6]);
    }

    ReleaseSemaphore(&MBBase->mb_Lock);
}
