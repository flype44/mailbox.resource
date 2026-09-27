#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* Generic VC4 GPU memory allocator. flags is a bitmask of MEM_FLAG_* (e.g.
   COHERENT | DIRECT | HINT_PERMALOCK for CPU-uncached, non-relocatable
   memory suitable for uploading code, per upload_code() usage in
   VideoCore.card). Returns an opaque handle to be passed to LockMemory. */
ULONG L_AllocateMemory(REGARG(ULONG size, "d0"), REGARG(ULONG alignment, "d1"), REGARG(ULONG flags, "d2"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;
    ULONG retval = 0;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 9*4;

    FBReq[0] = LE32(4*9);       // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x0003000c);// ALLOCATE_MEMORY
    FBReq[3] = LE32(12);
    FBReq[4] = 0;
    FBReq[5] = LE32(size);
    FBReq[6] = LE32(alignment);
    FBReq[7] = LE32(flags);
    FBReq[8] = 0;

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
