# mailbox.resource — implemented functions

| Function | Mailbox tag | Implemented | Since | Tested | Description |
|---|---|---|---|---|---|
| MB_GetClockRate | 0x00030002 | Yes | V1.0 | Yes | Reads a clock's configured rate |
| MB_GetClockState | 0x00030001 | Yes | V1.0 | No | Reads a clock's enabled/disabled state |
| MB_GetGPIOState | 0x00030041 | Yes | V1.0 | No | Reads a GPIO/LED expander pin state (aka `MB_GET_LED_STATUS`) |
| MB_GetPowerState | 0x00020001 | Yes | V1.0 | No | Reads a device's power on/off state |
| MB_RawCommand | *(generic)* | Yes | V1.0 | Yes | Sends a caller-built raw property-tag buffer as-is (byte-swaps + locks internally) |
| MB_SetClockRate | 0x00038002 | Yes | V1.0 | No | Sets a clock's rate |
| MB_SetClockState | 0x00038001 | Yes | V1.0 | No | Sets a clock's enabled/disabled state |
| MB_SetGPIOState | 0x00038041 | Yes | V1.0 | No | Sets a GPIO/LED expander pin state (aka `MB_SET_LED_STATUS`) |
| MB_SetPowerState | 0x00028001 | Yes | V1.0 | No | Sets a device's power on/off state |
| MB_StringCommand | 0x00030080 | Yes | V1.0 | No | Sends a `vcgencmd`-style text command, returns the text reply (aka `MB_GET_GENCMD_RESULT`) |
| MB_AllocateBuffer | 0x00040001 | Yes | V1.1 | No | Allocates the framebuffer (call after Set(Physical\|Virtual)Size/SetDepth); returns base + size |
| MB_AllocateMemory | 0x0003000c | Yes | V1.1 | No | Generic VC4 GPU memory allocator; returns an opaque handle |
| MB_BlankScreen | 0x00040002 | Yes | V1.1 | No | Blanks/unblanks the screen |
| MB_GetAlphaMode | 0x00040007 | Yes | V1.1 | No | Reads the framebuffer alpha channel mode |
| MB_GetARMMemory | 0x00010005 | Yes | V1.1 | No | Reads the ARM-side memory base/size split |
| MB_GetBoardMACAddress | 0x00010003 | Yes | V1.1 | Yes | Reads the board's MAC address (48-bit, fully byte-swapped) |
| MB_GetBoardModel | 0x00010001 | Yes | V1.1 | No | Reads the board model code |
| MB_GetBoardRevision | 0x00010002 | Yes | V1.1 | Yes | Reads the raw 32-bit board revision code |
| MB_GetBoardSerial | 0x00010004 | Yes | V1.1 | Yes | Reads the board's 64-bit serial number (raw blob, **not** byte-swapped) |
| MB_GetClockRateMax | 0x00030004 | Yes | V1.1 | Yes | Reads a clock's maximum allowed rate |
| MB_GetClockRateMeasured | 0x00030047 | Yes | V1.1 | Yes | Reads a clock's actual measured rate |
| MB_GetClockRateMin | 0x00030007 | Yes | V1.1 | No | Reads a clock's minimum allowed rate |
| MB_GetCommandLine | 0x00050001 | Yes | V1.1 | No | Reads the firmware's boot command line string |
| MB_GetDepth | 0x00040005 | Yes | V1.1 | No | Reads the framebuffer colour depth |
| MB_GetDMAChannels | 0x00060001 | Yes | V1.1 | No | Reads the bitmask of DMA channels available to the ARM |
| MB_GetEDIDBlock | 0x00030020 | Yes | V1.1 | No | Reads one 128-byte EDID block (base or CEA extension) |
| MB_GetFirmwareRevision | 0x00000001 | Yes | V1.1 | Yes | Reads the VideoCore firmware revision |
| MB_GetOverscan | 0x0004000a | Yes | V1.1 | No | Reads the framebuffer overscan margins (top/bottom/left/right) |
| MB_GetPalette | 0x0004000b | Yes | V1.1 | No | Reads the 256-entry RGBA palette *(structure per official spec, not yet hardware-validated)* |
| MB_GetPhysicalSize | 0x00040003 | Yes | V1.1 | Yes | Reads the framebuffer physical (display) size |
| MB_GetPitch | 0x00040008 | Yes | V1.1 | No | Reads the framebuffer's bytes-per-line pitch |
| MB_GetPixelOrder | 0x00040006 | Yes | V1.1 | No | Reads the framebuffer pixel colour order (BGR/RGB) |
| MB_GetTemperature | 0x00030006 | Yes | V1.1 | Yes | Reads the SoC temperature |
| MB_GetTemperatureMax | 0x0003000a | Yes | V1.1 | No | Reads the SoC's maximum safe temperature |
| MB_GetTurbo | 0x00030009 | Yes | V1.1 | Yes | Reads the turbo/overclock level |
| MB_GetVCMemory | 0x00010006 | Yes | V1.1 | Yes | Reads the VideoCore-side memory base/size split |
| MB_GetVirtualOffset | 0x00040009 | Yes | V1.1 | No | Reads the framebuffer virtual pan/scroll offset |
| MB_GetVirtualSize | 0x00040004 | Yes | V1.1 | No | Reads the framebuffer virtual (buffer) size |
| MB_GetVoltage | 0x00030003 | Yes | V1.1 | Yes | Reads a rail's voltage offset |
| MB_GetVoltageMax | 0x00030005 | Yes | V1.1 | No | Reads a rail's maximum allowed voltage offset |
| MB_GetVoltageMin | 0x00030008 | Yes | V1.1 | No | Reads a rail's minimum allowed voltage offset |
| MB_LockMemory | 0x0003000d | Yes | V1.1 | No | Locks a GPU memory handle and returns its VPU bus address |
| MB_ReleaseBuffer | 0x00048001 | Yes | V1.1 | No | Releases the framebuffer allocated by AllocateBuffer |
| MB_ReleaseMemory | 0x0003000f | Yes | V1.1 | No | Releases a GPU memory handle |
| MB_SetAlphaMode | 0x00048007 | Yes | V1.1 | No | Sets the framebuffer alpha channel mode |
| MB_SetDepth | 0x00048005 | Yes | V1.1 | No | Sets the framebuffer colour depth |
| MB_SetDisplayNum | 0x00048013 | Yes | V1.1 | No | Selects the target display (0=HDMI0, 1=HDMI1, BCM2711-only) for the framebuffer tags that follow |
| MB_SetOverscan | 0x0004800a | Yes | V1.1 | No | Sets the framebuffer overscan margins (in/out array) |
| MB_SetPalette | 0x0004800b | Yes | V1.1 | No | Sets a range of the 256-entry RGBA palette *(structure per official spec, not yet hardware-validated)* |
| MB_SetPhysicalSize | 0x00048003 | Yes | V1.1 | No | Sets the framebuffer physical (display) size |
| MB_SetPixelOrder | 0x00048006 | Yes | V1.1 | No | Sets the framebuffer pixel colour order (BGR/RGB) |
| MB_SetTurbo | 0x00038009 | Yes | V1.1 | No | Sets the turbo/overclock level |
| MB_SetVirtualOffset | 0x00048009 | Yes | V1.1 | No | Sets the framebuffer virtual pan/scroll offset |
| MB_SetVirtualSize | 0x00048004 | Yes | V1.1 | No | Sets the framebuffer virtual (buffer) size |
| MB_SetVoltage | 0x00038003 | Yes | V1.1 | No | Sets a rail's voltage offset |
| MB_UnlockMemory | 0x0003000e | Yes | V1.1 | No | Unlocks a GPU memory handle |
| MB_ExecuteCode | 0x00030010 | No | - | No | Niche, no reference implementation found, not implemented |
| MB_GetClocks | 0x00010007 | No | - | No | Known gap: request/response layout unconfirmed anywhere, not implemented |
| MB_GetDispmanxHandle | 0x00030014 | No | - | No | Niche, no reference implementation found, not implemented |
| MB_GetPowerTiming | 0x00020002 | No | - | No | Niche, no reference implementation found, not implemented |
| MB_SetCursorInfo | 0x00008010 | No | - | No | Niche, no reference implementation found, not implemented |
| MB_SetCursorState | 0x00008011 | No | - | No | Niche, no reference implementation found, not implemented |
| MB_SetScreenGamma | 0x00008012 | No | - | No | Niche, no reference implementation found, not implemented |
| MB_TestAlphaMode | 0x00044007 | No | - | No | Dry-run variant, not implemented |
| MB_TestDepth | 0x00044005 | No | - | No | Dry-run variant, not implemented |
| MB_TestLEDStatus | 0x00034041 | No | - | No | Dry-run variant, not implemented |
| MB_TestOverscan | 0x0004400a | No | - | No | Dry-run variant, not implemented |
| MB_TestPalette | 0x0004400b | No | - | No | Dry-run variant, not implemented |
| MB_TestPhysicalSize | 0x00044003 | No | - | No | Dry-run variant, not implemented |
| MB_TestPixelOrder | 0x00044006 | No | - | No | Dry-run variant, not implemented |
| MB_TestVirtualOffset | 0x00044009 | No | - | No | Dry-run variant, not implemented |
| MB_TestVirtualSize | 0x00044004 | No | - | No | Dry-run variant, not implemented |
