#include <exec/types.h>
#include <proto/exec.h>
#include <common/compiler.h>

#include "mailbox.h"

/* Reads one 128-byte EDID block (block_number 0 = base block, 1+ = CEA
   extension blocks announced in byte 126 of block 0). Unlike most other
   tags, the EDID payload itself is a raw byte blob and must NOT be
   byte-swapped - only the leading status word is a real 32-bit value.
   edid_out, if non-NULL, must point to a 128-byte buffer. Returns 0 on
   success (status word == 0), matching the convention already used by
   Emu68DisplayDevice's own EDID_GetBlock() built on top of MB_RawCommand. */
ULONG L_GetEDIDBlock(REGARG(ULONG block_number, "d0"), REGARG(UBYTE *edid_out, "a0"), REGARG(struct MailboxBase *MBBase, "a6"))
{
    struct ExecBase *SysBase = MBBase->mb_ExecBase;
    ULONG status = -1;

    ObtainSemaphore(&MBBase->mb_Lock);

    ULONG *FBReq = MBBase->mb_Request;
    ULONG len = 40*4;

    FBReq[0] = LE32(4*40);      // Length
    FBReq[1] = 0;               // Request
    FBReq[2] = LE32(0x00030020);// GET_EDID_BLOCK
    FBReq[3] = LE32((2 + 32)*4);
    FBReq[4] = 0;
    FBReq[5] = LE32(block_number);
    FBReq[6] = 0;                // status (response)
    for (ULONG i = 0; i < 32; i++)
        FBReq[7 + i] = 0;
    FBReq[39] = 0;               // end tag

    CachePreDMA(FBReq, &len, 0);
    mbox_send(8, (ULONG)FBReq, MBBase);
    ULONG resp = mbox_recv(8, MBBase);
    CachePostDMA(FBReq, &len, 0);

    if (resp != 0xffffffff)
    {
        status = LE32(FBReq[6]);

        if (status == 0 && edid_out)
        {
            CopyMem(&FBReq[7], edid_out, 128);
        }
    }

    ReleaseSemaphore(&MBBase->mb_Lock);

    return status;
}
