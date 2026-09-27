#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* Returns the ATAG/DT bootargs string the firmware handed to the kernel.
   reply, if non-NULL, receives up to reply_capacity bytes (truncated at
   1024, the tag's fixed response buffer size). Returns the actual response
   length reported by the firmware, or -1 on error. */
ULONG L_GetCommandLine(REGARG(STRPTR reply, "a0"), REGARG(ULONG reply_capacity, "d0"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;
    ULONG *FBReq = MBBase->mb_Request;
    ULONG retval;
    ULONG len = 0;
    ULONG i = 0;

    ObtainSemaphore(&MBBase->mb_Lock);

    FBReq[i++] = 0;              // Length (set below)
    FBReq[i++] = 0;               // Request
    FBReq[i++] = LE32(0x00050001);// GET_COMMAND_LINE
    FBReq[i++] = LE32(1024);
    FBReq[i++] = 0;               // Request/response indicator

    ULONG cmdline_pos = i;
    for (ULONG j = 0; j < (1024 >> 2); j++)
        FBReq[i++] = 0;

    FBReq[i++] = 0;               // end tag

    FBReq[0] = LE32(i << 2);
    len = i * 4;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
    {
        retval = -1;
    }
    else
    {
        retval = LE32(FBReq[4]) & ~0x80000000;

        if (reply && reply_capacity > 0)
        {
            CopyMem(&FBReq[cmdline_pos], reply, reply_capacity > 1024 ? 1024 : reply_capacity);
        }
    }

    ReleaseSemaphore(&MBBase->mb_Lock);

    return retval;
}
