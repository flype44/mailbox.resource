#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

VOID L_SetVirtualSize(REGARG(ULONG width, "d0"), REGARG(ULONG height, "d1"), REGARG(ULONG *actual_width, "a0"), REGARG(ULONG *actual_height, "a1"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 8*4;

    FBReq[0] = LE32(4*8);       // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x00048004);// SET_VIRTUAL_SIZE
    FBReq[3] = LE32(8);
    FBReq[4] = 0;
    FBReq[5] = LE32(width);
    FBReq[6] = LE32(height);
    FBReq[7] = 0;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
    {
        if (actual_width) *actual_width = -1;
        if (actual_height) *actual_height = -1;
    }
    else
    {
        if (actual_width) *actual_width = LE32(FBReq[5]);
        if (actual_height) *actual_height = LE32(FBReq[6]);
    }

    ReleaseSemaphore(&MBBase->mb_Lock);
}
