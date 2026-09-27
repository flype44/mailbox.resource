
#include <exec/types.h>
#include <exec/execbase.h>
#include <proto/exec.h>
#include <proto/expansion.h>
#include <proto/devicetree.h>

#include <libraries/configregs.h>
#include <libraries/configvars.h>

#include <common/compiler.h>

#include "mailbox.h"

extern UBYTE rom_end;
extern const char deviceName[];
extern const char deviceIdString[];

/*
    Some properties, like e.g. #size-cells, are not always available in a key, but in that case the properties
    should be searched for in the parent. The process repeats recursively until either root key is found
    or the property is found, whichever occurs first
*/
CONST_APTR GetPropValueRecursive(APTR key, CONST_STRPTR property, APTR DeviceTreeBase)
{
    do {
        /* Find the property first */
        APTR prop = DT_FindProperty(key, property);

        if (prop)
        {
            /* If property is found, get its value and exit */
            return DT_GetPropValue(prop);
        }
        
        /* Property was not found, go to the parent and repeat */
        key = DT_GetParent(key);
    } while (key);

    return NULL;
}

APTR Init(REGARG(struct ExecBase *SysBase, "a6"))
{
    struct MailboxBase *MailboxBase = NULL;
    struct ExpansionBase *ExpansionBase = NULL;
    struct CurrentBinding binding;

    APTR base_pointer = NULL;
    
    ExpansionBase = (struct ExpansionBase *)OpenLibrary("expansion.library", 0);
    GetCurrentBinding(&binding, sizeof(binding));

    base_pointer = AllocMem(BASE_NEG_SIZE + BASE_POS_SIZE, MEMF_PUBLIC | MEMF_CLEAR);

    bug("[mailbox] Init\n");

    if (base_pointer)
    {
        ULONG relFuncTable[NUMBER_OF_FUNCTIONS + 1];

        relFuncTable[0] = (ULONG)&L_RawCommand;
        relFuncTable[1] = (ULONG)&L_StringCommand;
        relFuncTable[2] = (ULONG)&L_GetClockRate;
        relFuncTable[3] = (ULONG)&L_SetClockRate;
        relFuncTable[4] = (ULONG)&L_GetClockState;
        relFuncTable[5] = (ULONG)&L_SetClockState;
        relFuncTable[6] = (ULONG)&L_GetPowerState;
        relFuncTable[7] = (ULONG)&L_SetPowerState;
        relFuncTable[8] = (ULONG)&L_GetGPIOState;
        relFuncTable[9] = (ULONG)&L_SetGPIOState;
        relFuncTable[10] = (ULONG)&L_GetFirmwareRevision;
        relFuncTable[11] = (ULONG)&L_GetBoardModel;
        relFuncTable[12] = (ULONG)&L_GetBoardRevision;
        relFuncTable[13] = (ULONG)&L_GetBoardMACAddress;
        relFuncTable[14] = (ULONG)&L_GetBoardSerial;
        relFuncTable[15] = (ULONG)&L_GetARMMemory;
        relFuncTable[16] = (ULONG)&L_GetVCMemory;
        relFuncTable[17] = (ULONG)&L_GetClockRateMeasured;
        relFuncTable[18] = (ULONG)&L_GetClockRateMin;
        relFuncTable[19] = (ULONG)&L_GetClockRateMax;
        relFuncTable[20] = (ULONG)&L_GetTemperature;
        relFuncTable[21] = (ULONG)&L_GetTemperatureMax;
        relFuncTable[22] = (ULONG)&L_GetVoltage;
        relFuncTable[23] = (ULONG)&L_GetVoltageMin;
        relFuncTable[24] = (ULONG)&L_GetVoltageMax;
        relFuncTable[25] = (ULONG)&L_GetTurbo;
        relFuncTable[26] = (ULONG)&L_SetTurbo;
        relFuncTable[27] = (ULONG)&L_SetDisplayNum;
        relFuncTable[28] = (ULONG)&L_GetPhysicalSize;
        relFuncTable[29] = (ULONG)&L_SetPhysicalSize;
        relFuncTable[30] = (ULONG)&L_GetVirtualSize;
        relFuncTable[31] = (ULONG)&L_SetVirtualSize;
        relFuncTable[32] = (ULONG)&L_GetDepth;
        relFuncTable[33] = (ULONG)&L_SetDepth;
        relFuncTable[34] = (ULONG)&L_GetPitch;
        relFuncTable[35] = (ULONG)&L_AllocateBuffer;
        relFuncTable[36] = (ULONG)&L_ReleaseBuffer;
        relFuncTable[37] = (ULONG)&L_BlankScreen;
        relFuncTable[38] = (ULONG)&L_AllocateMemory;
        relFuncTable[39] = (ULONG)&L_LockMemory;
        relFuncTable[40] = (ULONG)&L_UnlockMemory;
        relFuncTable[41] = (ULONG)&L_ReleaseMemory;
        relFuncTable[42] = (ULONG)&L_SetVoltage;
        relFuncTable[43] = (ULONG)&L_GetPixelOrder;
        relFuncTable[44] = (ULONG)&L_SetPixelOrder;
        relFuncTable[45] = (ULONG)&L_GetAlphaMode;
        relFuncTable[46] = (ULONG)&L_SetAlphaMode;
        relFuncTable[47] = (ULONG)&L_GetVirtualOffset;
        relFuncTable[48] = (ULONG)&L_SetVirtualOffset;
        relFuncTable[49] = (ULONG)&L_GetOverscan;
        relFuncTable[50] = (ULONG)&L_SetOverscan;
        relFuncTable[51] = (ULONG)&L_GetDMAChannels;
        relFuncTable[52] = (ULONG)&L_GetEDIDBlock;
        relFuncTable[53] = (ULONG)&L_GetCommandLine;
        relFuncTable[54] = (ULONG)&L_GetPalette;
        relFuncTable[55] = (ULONG)&L_SetPalette;
        relFuncTable[NUMBER_OF_FUNCTIONS] = (ULONG)-1;

        MailboxBase = (struct MailboxBase *)((UBYTE *)base_pointer + BASE_NEG_SIZE);
        MakeFunctions(MailboxBase, relFuncTable, 0);
        
        MailboxBase->mb_Node.lib_Node.ln_Type = NT_RESOURCE;
        MailboxBase->mb_Node.lib_Node.ln_Pri = 120;
        MailboxBase->mb_Node.lib_Node.ln_Name = (STRPTR)deviceName;

        MailboxBase->mb_Node.lib_NegSize = BASE_NEG_SIZE;
        MailboxBase->mb_Node.lib_PosSize = BASE_POS_SIZE;
        MailboxBase->mb_Node.lib_Version = MB_VERSION;
        MailboxBase->mb_Node.lib_Revision = MB_REVISION;
        MailboxBase->mb_Node.lib_IdString = (STRPTR)deviceIdString;

        MailboxBase->mb_RequestBase = AllocMem(512*4, MEMF_FAST);
        MailboxBase->mb_Request = (ULONG *)(((ULONG)MailboxBase->mb_RequestBase + 127) & ~127);

        bug("[mailbox] Request buffer @ %08lx\n", (ULONG)MailboxBase->mb_Request);

        MailboxBase->mb_ExecBase = SysBase;

        InitSemaphore(&MailboxBase->mb_Lock);

        APTR DeviceTreeBase = OpenResource("devicetree.resource");

        if (DeviceTreeBase != NULL)
        {
            /* Get VC4 physical address of mailbox interface. Subsequently it will be translated to m68k physical address */
            APTR key = DT_OpenKey("/aliases");

            if (key)
            {
                CONST_STRPTR mbox_alias = DT_GetPropValue(DT_FindProperty(key, "mailbox"));

                DT_CloseKey(key);
               
                if (mbox_alias != NULL)
                {
                    key = DT_OpenKey(mbox_alias);

                    if (key)
                    {
                        int size_cells = 1;
                        int address_cells = 1;

                        const ULONG * siz = GetPropValueRecursive(key, "#size_cells", DeviceTreeBase);
                        const ULONG * addr = GetPropValueRecursive(key, "#address-cells", DeviceTreeBase);

                        if (siz != NULL)
                            size_cells = *siz;
                        
                        if (addr != NULL)
                            address_cells = *addr;

                        const ULONG *reg = DT_GetPropValue(DT_FindProperty(key, "reg"));

                        MailboxBase->mb_MailBox = (APTR)reg[address_cells - 1];

                        DT_CloseKey(key);
                    }
                }
                DT_CloseKey(key);
            }

            /* Open /soc key and learn about VC4 to CPU mapping. Use it to adjust the addresses obtained above */
            key = DT_OpenKey("/soc");
            if (key)
            {
                int size_cells = 1;
                int address_cells = 1;
                int cpu_address_cells = 1;

                const ULONG * siz = GetPropValueRecursive(key, "#size_cells", DeviceTreeBase);
                const ULONG * addr = GetPropValueRecursive(key, "#address-cells", DeviceTreeBase);
                const ULONG * cpu_addr = DT_GetPropValue(DT_FindProperty(DT_OpenKey("/"), "#address-cells"));
            
                if (siz != NULL)
                    size_cells = *siz;
                
                if (addr != NULL)
                    address_cells = *addr;

                if (cpu_addr != NULL)
                    cpu_address_cells = *cpu_addr;

                const ULONG *reg = DT_GetPropValue(DT_FindProperty(key, "ranges"));

                ULONG phys_vc4 = reg[address_cells - 1];
                ULONG phys_cpu = reg[address_cells + cpu_address_cells - 1];

                MailboxBase->mb_MailBox = (APTR)((ULONG)MailboxBase->mb_MailBox - phys_vc4 + phys_cpu);

                bug("[mailbox] Pi mailbox @ %08lx\n", (ULONG)MailboxBase->mb_MailBox);

                DT_CloseKey(key);
            }

            SumLibrary((struct Library*)MailboxBase);
            AddResource(MailboxBase);

            binding.cb_ConfigDev->cd_Flags &= ~CDF_CONFIGME;
            binding.cb_ConfigDev->cd_Driver = MailboxBase;
        }
        else
        {
            FreeMem(base_pointer, BASE_NEG_SIZE + BASE_POS_SIZE);
            MailboxBase = NULL;
        }
    }

    CloseLibrary((struct Library*)ExpansionBase);

    return MailboxBase;
}

static void putch(REGARG(UBYTE data, "d0"), REGARG(APTR ignore, "a3"))
{
    (void)ignore;
    *(UBYTE*)0xdeadbeef = data;
}

void kprintf(REGARG(const char * msg, "a0"), REGARG(void * args, "a1")) 
{
    struct ExecBase *SysBase = *(struct ExecBase **)4UL;
    RawDoFmt(msg, args, (APTR)putch, NULL);
}
