# mailbox.resource

AmigaOS resource for PiStorm/Emu68, exposing the Raspberry Pi VideoCore
mailbox (property channel) as native, semaphore-safe, byte-swapped calls.

## Synopsis

This resource provides :

- **Semaphoring** — every call is serialised internally (`ObtainSemaphore`/
  `ReleaseSemaphore`), so concurrent callers queue up safely instead of
  corrupting each other's mailbox transaction.
- **Byte-swapping** — the VC4 side is little-endian, the m68k side is
  big-endian; every call swaps consistently in both directions, including
  the tricky exceptions (e.g. the board serial number, handed back as a
  raw byte blob that must *not* be swapped, unlike almost everything else).
- **`MB_RawCommand()`**, the generic escape hatch — sends any caller-built
  property-tag buffer for tags that don't (yet) have a dedicated function,
  with the same locking/byte-swapping guarantees as everything else.
- **`MB_StringCommand()`** — a `vcgencmd`-style text interface: send an
  ASCII command (e.g. `"measure_temp"`) and get the firmware's text reply
  back, for anything easier expressed as a command string than as a
  dedicated call or a raw tag buffer.
- **A set of ready-to-use functions** — 56 dedicated calls (clocks,
  voltages, temperature, board identity, framebuffer, GPU memory, EDID,
  ...) instead of hand-building property-tag buffers yourself. See
  `Autodocs/mailbox.doc`.

### Example

```c
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/mailbox.h>
#include <resources/mailbox.h>

APTR MailboxBase = OpenResource(MAILBOXNAME);
if (MailboxBase)
{
    ULONG revision = MB_GetFirmwareRevision();
    Printf("Firmware revision: %ld\n", revision);
}
```

Same thing via `MB_RawCommand()`, for a tag with no dedicated function yet:

```c
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/mailbox.h>
#include <resources/mailbox.h>

APTR MailboxBase = OpenResource(MAILBOXNAME);
if (MailboxBase)
{
    ULONG command[7] =
    {
        7 * 4,       /* [0] total buffer size, in bytes           */
        0,           /* [1] request code (0 = process request)    */
        0x00000001,  /* [2] tag: GET_FIRMWARE_REV                 */
        4,           /* [3] tag value buffer size, in bytes       */
        0,           /* [4] tag request/response indicator        */
        0,           /* [5] tag value: firmware revision (response) */
        0,           /* [6] end tag                               */
    };

    MB_RawCommand(command);
    Printf("Firmware revision: %ld\n", command[5]);
}
```

## Build

Requires an `m68k-amigaos-gcc` cross toolchain and CMake.

```sh
mkdir build && cd build
CC=m68k-amigaos-gcc cmake -G "Unix Makefiles" ..
cmake --build .
```

`build/` then contains:

- `mailbox.resource` — the compiled ROM-resident binary.
- `mailbox.resource.h` — the binary, embedded as a C header (for ROM builds).
- `include/{clib,proto,pragmas,inline}/mailbox*.h` — sfd-generated call stubs.
- `FD/mailbox.fd` — the old-style FD function description (for tools/
  compilers that predate sfd, e.g. classic assemblers or SAS/C).
- `SFD/mailbox_lib.sfd` — a copy of the sfd source these are all generated
  from, matching the NDK's own top-level `FD`/`SFD` layout.
- `Autodocs/mailbox.doc` — a copy of the function reference below.
- `mailbox_test` — an ordinary AmigaOS CLI program exercising every
  function listed in `Autodocs/mailbox.doc`, generated straight from the
  same source as that doc (`test/test.c`). Run it from a Shell on real
  PiStorm/Emu68 hardware to sanity-check a build. Some of its snippets are
  the doc's own illustrative calls run with literal values (e.g. forcing
  1920x1080, blanking the screen) rather than safe round-trips, so expect
  visible display disruption while it runs.

Running `cmake --install . --prefix <dir>` additionally ships a
`Developer/` tree (`sfd/`, `include/` including `resources/mailbox.h`, and
`Autodocs/mailbox.doc`) for client projects, mirroring the layout of
Commodore/Hyperion's own NDK.

See `Autodocs/mailbox.doc` for the full function reference and
`Implementation.md` for the tag-by-tag implementation/test status.

## Deployment

This binary is not something a client program builds or copies itself.
It's ROM-resident: `mailbox.resource.h` (the bin2h-embedded binary) is
meant to be baked into an Emu68/PiStorm firmware build, the same way
`devicetree.resource` already is. A client Amiga program never installs
it — it just calls `OpenResource(MAILBOXNAME)` and expects whatever
Emu68 build is running on the target to already provide it. Building
this repo yourself is only needed when working on the resource itself or
on Emu68's own firmware image, not when writing an application against it.

## Scope

This resource only talks to the VideoCore **property-tag mailbox**
(`/soc/mailbox@7e00b880`, `compatible = "brcm,bcm2835-mbox"`, devicetree
alias `mailbox`, channel 8) — the interface behind every function listed
in `Autodocs/mailbox.doc`, plus `MB_RawCommand()`/`MB_StringCommand()`.

Emu68's device tree exposes a second, entirely separate mailbox-compatible
peripheral at a different hardware address, `/soc/mailbox@7e00b840`
(`compatible = "brcm,bcm2711-vchiq"`, alias `vchiq`) — the doorbell
interface for **VCHIQ**, VideoCore's host interface used by camera, audio
and other GPU services. It speaks a completely different wire protocol
and is **not addressed by this resource at all**; a client needing VCHIQ
services needs its own, separate driver.

## Adding a function (maintainer checklist)

Every new function touches all of these — miss one and either the build
breaks, the ABI is wrong, or a caller can't tell your new function exists:

1. `src/<name>.c` — the `L_<Name>()` implementation (`ObtainSemaphore`/
   `ReleaseSemaphore`, `LE32()` byte-swapping, `mbox_send`/`mbox_recv`;
   copy the shape of an existing file for the same tag family).
2. `src/mailbox.h` — declare `L_<Name>()`, and bump `NUMBER_OF_FUNCTIONS`.
3. `src/init.c` — add it to `relFuncTable[]` **at the next free index,
   appended at the end** — never renumber existing entries, that would
   break every already-compiled client's jump-table offsets.
4. `sfd/mailbox_lib.sfd` — add the `MB_<Name>(...)` entry (matching
   register list), **in the same order as step 3**. Zero-argument
   functions need an explicit empty register list: `MB_<Name>() ()`, not
   bare `MB_<Name>()` — sfdc silently mis-parses consecutive bare ones.
   Also bump the version in its `==id $Id: mailbox_lib.sfd X.Y $` header
   to match step 6's new `MB_VERSION.MB_REVISION` — sfdc embeds this
   string verbatim into every generated `clib`/`pragmas` header and the
   `FD`/`SFD` copies shipped in `build/`, so a stale one is misleading to
   anyone reading those files.
5. `CMakeLists.txt` — add `src/<name>.c` to `mailbox.resource`'s source
   list.
6. `src/mailbox.h` — **bump `MB_REVISION`** (or `MB_VERSION` for a
   breaking change), and update the comment above it describing what
   this revision adds.
7. `include/resources/mailbox.h` — bump its own `$VER: mailbox.h
   X.Y` header line to the same version, like every other generated
   include here.
8. `Autodocs/mailbox.doc` + `Implementation.md` — add the new function
   (with its `Since` version) and mark it `Implemented: Yes` /
   `Tested: No` until verified on real hardware.
9. `test/test.c` — add a case exercising the new function (regenerate
   from the same source used for the doc, rather than hand-editing, to
   keep the two in sync).
10. `CMakeLists.txt`'s top `project(mailbox VERSION ...)` — bump for the
    release/deployment tracking; this is independent of `MB_VERSION`/
    `MB_REVISION` (the real, runtime-checkable ABI version) and only
    affects the sfd/verstring `$VER:` string.

## Bumping the version

A version bump on its own (no new function) still has to propagate to several files, or some of them end up out of sync with each other:

| File | What to update |
|---|---|
| `src/mailbox.h` | `MB_REVISION` (or `MB_VERSION` for a breaking change)<br />Used at runtime in MailboxBase `lib_Version`/`lib_Revision`. |
| `include/resources/mailbox.h` | Its `$VER: mailbox.h X.Y` header line, so it matches. |
| `sfd/mailbox_lib.sfd` | Its `==id $Id: mailbox_lib.sfd X.Y $` header line<br />sfdc use this verbatim into every generated include header and into the `FD`/`SFD`. |
| `Autodocs/mailbox.doc` | Every new function's `NAME` line gets a `(VX.Y)` tag for the revision it was added in. |
| `Implementation.md` | The `Since` column for any newly added function, re-sorted by `Since`, `Function`. |
| `test/test.c` | A new `if (lib->lib_Version == X && lib->lib_Revision == Y) { ... }` block. |
| `CMakeLists.txt` | MAJOR.MINOR in `project(mailbox VERSION MAJOR.MINOR.PATCH)` is used into `cmake/verstring.cmake`'s `VERSTRING` (`$VER: mailbox X.Y (date) git:hash`),<br />compiled into `mailbox.resource`'s own `deviceIdString`.<br />Bump it too, but don't confuse it with `MB_VERSION`/`MB_REVISION` above. |
