#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* Sets 'count' consecutive palette entries starting at 'offset', from the
   RGBA array pointed to by 'colors'. count must not exceed 256. Returns 0
   on success, 1 if the firmware reported an invalid palette index, or -1
   on a mailbox transport error/invalid count. */
ULONG L_SetPalette(REGARG(ULONG offset, "d0"), REGARG(ULONG count, "d1"), REGARG(ULONG *colors, "a0"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;
    ULONG *FBReq = MBBase->mb_Request;
    ULONG retval = 0;
    ULONG i = 0;

    if (count > 256)
        return -1;

    ObtainSemaphore(&MBBase->mb_Lock);

    FBReq[i++] = 0;                  // Length (set below)
    FBReq[i++] = 0;                   // Request
    FBReq[i++] = LE32(0x0004800b);    // SET_PALETTE
    FBReq[i++] = LE32((2 + count)*4);
    FBReq[i++] = 0;                   // Request/response indicator

    ULONG status_pos = i;
    FBReq[i++] = LE32(offset);
    FBReq[i++] = LE32(count);

    for (ULONG j = 0; j < count; j++)
        FBReq[i++] = colors ? LE32(colors[j]) : 0;

    FBReq[i++] = 0;                   // end tag

    FBReq[0] = LE32(i << 2);
    ULONG len = i * 4;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
        retval = -1;
    else
        retval = LE32(FBReq[status_pos]);

    ReleaseSemaphore(&MBBase->mb_Lock);

    return retval;
}
