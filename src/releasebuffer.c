#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

VOID L_ReleaseBuffer(REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 6*4;

    FBReq[0] = LE32(4*6);       // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x00048001);// RELEASE_BUFFER
    FBReq[3] = 0;
    FBReq[4] = 0;
    FBReq[5] = 0;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    ReleaseSemaphore(&MBBase->mb_Lock);
}
