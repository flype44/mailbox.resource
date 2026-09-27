#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* palette must point to an array of 256 ULONGs (RGBA entries), filled on
   return. Returns 0 on success. */
ULONG L_GetPalette(REGARG(ULONG *palette, "a0"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;
    ULONG *FBReq = MBBase->mb_Request;
    ULONG retval = 0;
    ULONG i = 0;

    ObtainSemaphore(&MBBase->mb_Lock);

    FBReq[i++] = 0;               // Length (set below)
    FBReq[i++] = 0;                // Request
    FBReq[i++] = LE32(0x0004000b); // GET_PALETTE
    FBReq[i++] = LE32(256 * 4);
    FBReq[i++] = 0;                // Request/response indicator

    ULONG palette_pos = i;
    for (ULONG j = 0; j < 256; j++)
        FBReq[i++] = 0;

    FBReq[i++] = 0;                // end tag

    FBReq[0] = LE32(i << 2);
    ULONG len = i * 4;

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp == 0xffffffff)
    {
        retval = -1;
    }
    else if (palette)
    {
        for (ULONG j = 0; j < 256; j++)
            palette[j] = LE32(FBReq[palette_pos + j]);
    }

    ReleaseSemaphore(&MBBase->mb_Lock);

    return retval;
}
