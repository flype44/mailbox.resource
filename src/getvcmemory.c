#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

VOID L_GetVCMemory(REGARG(ULONG *base, "a0"), REGARG(ULONG *size, "a1"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 8*4;

    FBReq[0] = LE32(4*8);       // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x00010006);// GET_VC_MEMORY
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
        if (base) *base = -1;
        if (size) *size = -1;
    }
    else
    {
        if (base) *base = LE32(FBReq[5]);
        if (size) *size = LE32(FBReq[6]);
    }

    ReleaseSemaphore(&MBBase->mb_Lock);
}
