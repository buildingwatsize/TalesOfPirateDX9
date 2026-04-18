# Tales of Pirate — x64 Upgrade Plan

**Source**: `C:\TalesOfPirate2016\Source` (read-only reference, DO NOT EDIT)
**Target**: `C:\TalesOfPirateDX9\sources\` (all changes here)
**Current**: Win32/x86 only, VS2022 v143, DX9, C++17

---

## Phase 0 — Setup (Day 1)

| Step | Task | Details |
|------|------|---------|
| 0.1 | Copy original to working branch | `git checkout -b x64-upgrade` in TalesOfPirateDX9 repo |
| 0.2 | Add x64 platform to .sln | Add `Debug|x64` and `Release|x64` configs to `talesofpirates.sln` |
| 0.3 | Add x64 platform to all 22 .vcxproj | Clone Win32 configs → x64, set `<TargetMachine>MachineX64</TargetMachine>` |
| 0.4 | Update build_all.bat | Add `/p:Platform=x64` variant |

---

## Phase 1 — Third-Party Libraries (Days 2–4)

Must acquire or build x64 versions. **None of the current .lib files are x64.**

| Library | Action | Risk |
|---------|--------|------|
| **DirectX 9 SDK** | Use x64 libs from June 2010 SDK (`Lib/x64/`) | Low — MS provides both |
| **SDL 1.2.7** | Replace with **SDL2** (SDL 1.2 has no x64). Build from source | Medium — API changes |
| **SDL_mixer** | Replace with **SDL2_mixer** | Medium — follows SDL |
| **Discord RPC** | Build from GitHub source for x64, or use discord-game-sdk x64 | Low |
| **Crypto++ (cryptlib)** | Already has x64 config in .vcxproj — just build | Low |
| **ICU** | Download x64 prebuilt from ICU project | Low |
| **LuaJIT** | Build x64 from source (`msvcbuild.bat`) | Low — native x64 support |
| **Lua 5.0** | Build from source with x64 | Low |
| **BASS audio** | Download x64 DLL from un4seen.com | Low |

**Output**: All third-party x64 `.lib` + `.dll` files in `Libraries/*/lib/x64/`

---

## Phase 2 — Critical Code Fixes (Days 5–12)

### 2.1 DWORD ↔ Pointer Casts — **~100+ instances, HIGHEST PRIORITY**

**Pattern**: `(DWORD)ptr` or `(Type*)(DWORD)value`
**Fix**: Replace with `DWORD_PTR`, `UINT_PTR`, `uintptr_t`

| Component | File Count | Est. Fixes |
|-----------|-----------|------------|
| GameServer | CharScript.cpp, Mission.cpp, Character.cpp, NpcScript.cpp, GameDB.cpp, lua_gamectrl.h, Expand.h | ~80 |
| GroupServer | LuaFunc.cpp, GroupServerApp.cpp | ~5 |
| Engine | (fewer, mostly in resource mgr) | ~5 |
| Libraries/Util | Stacktrace.cpp, sym_engine.cpp, logutil.cpp, util2.h | ~10 |
| Client | Connection, CharacterPacket | ~5 |

**Search regex**: `\(DWORD\)\s*[&*a-zA-Z]` and `(Type*)\s*\(DWORD\)`

### 2.2 Inline Assembly — **~30 instances, MUST REPLACE**

MSVC x64 does **not** support `__asm` blocks. Every instance must be replaced.

| File | Purpose | x64 Replacement |
|------|---------|-----------------|
| `Engine/src/lwResourceMgr.cpp` (3) | Float/math tricks | SSE2 intrinsics (`_mm_*`) |
| `Engine/src/lwPhysique.cpp` (1) | Matrix math | SSE2 intrinsics |
| `Engine/src/lwNodeObject.cpp` (3) | Vector math | SSE2 intrinsics |
| `Engine/src/lwModelObject.cpp` (2) | Float tricks | `_mm_cvtss_si32` etc. |
| `Engine/src/lwAnimCtrlObj.cpp` (1) | Math | SSE2 intrinsics |
| `Engine/include/MPEffectAsmMath.h` (1) | SIMD math | Full rewrite to intrinsics |
| `Client/src/Algo.cpp` (4) | Float→int fast cast | `_mm_cvttss_si32` |
| `Client/src/Scene.cpp` (1) | Float trick | Intrinsics |
| `Client/src/GameAppMsg.cpp` (1) | Float trick | Intrinsics |
| `Client/src/EffectObj.cpp` (1) | Float trick | Intrinsics |
| `Client/src/CharacterModel.cpp` (1) | Float trick | Intrinsics |
| `Libraries/Util/src/algo.cpp` (4) | Float→int | Intrinsics |
| `Libraries/Util/include/kbase.h` (1) | `int 3` debug break | `__debugbreak()` |
| `Server/GameServer/src/StackWalker.h` (1) | Stack capture | `RtlCaptureContext()` |

### 2.3 AtlAux2.h Thunking — **BLOCKER**

File: `Libraries/Util/include/AtlAux2.h`
- `CAuxThunk<T>` and `CAuxStdThunk<T>` use hardcoded x86 opcodes
- Stores `this` pointer as `DWORD` (32-bit truncation)

**Fix**: Replace with `ATL::CStdCallThunk` from modern ATL, or use `std::function` + static dispatch map with `SetWindowLongPtr(GWLP_USERDATA)`.

### 2.4 StackWalker / Stack Trace (Server)

- `StackWalker.h`: x86 asm for context capture → use `RtlCaptureContext()`
- `StackWalker.cpp`: DWORD for addresses → `DWORD64` (already partially supported in dbghelp x64)
- `Stacktrace.cpp`, `sym_engine.cpp`: pointer-as-DWORD → `DWORD_PTR`

### 2.5 long* Pointer Storage

- `LogvwrDlg.cpp:871` — `long* ptr = (long*)md` → `LONG_PTR*`
- `AccountServer2.cpp:1433` — same pattern

---

## Phase 3 — Network Protocol Compatibility (Days 13–15)

**Critical**: Server and client communicate via packed binary structs.

| Concern | Action |
|---------|--------|
| `#pragma pack` structs (50+ headers) | Audit all — ensure no pointer/size_t members in wire structs |
| Packet.h, cfl_pkt.h, DBCCommon.h | Verify fixed-width types (`uint32_t`) not platform-dependent types |
| Server↔Client ABI | If server stays x86 and client goes x64 (or vice versa), wire format must use fixed-size types only |

**Decision needed**: Will servers also be x64, or only client?
- **Recommended**: Upgrade both to x64 (servers benefit from >4GB address space for large maps/player counts)

---

## Phase 4 — Build & Link (Days 16–18)

| Step | Task |
|------|------|
| 4.1 | Build all libraries in x64 Release |
| 4.2 | Fix linker errors (unresolved symbols from lib mismatch) |
| 4.3 | Update library search paths: `$(ProjectDir)..\..\Libraries\*/lib/x64/` |
| 4.4 | Update DX9 lib path to x64 SDK libs |
| 4.5 | Resolve any remaining warnings: `/W4` + treat `C4311` (pointer truncation) as error |

**Compiler flags to add for x64**:
```
/Wp64          (deprecated but catches issues)
/we4302        (truncation warning as error)
/we4311        (pointer to DWORD truncation as error)
/we4312        (DWORD to pointer conversion as error)
```

---

## Phase 5 — Runtime Testing (Days 19–22)

| Test | What to Verify |
|------|---------------|
| Client launch | DX9 x64 rendering, no crashes |
| Server startup | All 4 servers (Account, Gate, Game, Group) start and connect to MSSQL |
| Login flow | Client → GateServer → AccountServer → GameServer |
| Gameplay | Character creation, movement, combat, NPC interaction |
| Lua scripts | All game scripts work under x64 Lua/LuaJIT |
| Stress test | Memory >4GB usage, no 32-bit overflow in IDs/handles |
| Network | x64 server ↔ x64 client packet integrity |

---

## Phase 6 — Cleanup (Days 23–25)

- Remove Win32-only `#ifdef` dead code
- Update `build_all.bat` / `rebuild_engine.bat` for x64 default
- Document x64 build requirements
- Tag release: `v2.0-x64`

---

## Risk Matrix

| Risk | Severity | Mitigation |
|------|----------|------------|
| SDL 1.2→2 API breakage | High | Isolate in AudioSDL wrapper, minimal surface |
| Inline asm rewrite bugs | High | Unit test each math function before/after |
| Wire protocol breakage | Critical | Use `static_assert(sizeof(packet) == N)` |
| DX9 x64 driver issues | Medium | DX9 x64 is well-supported on modern Windows |
| Lua pointer handling | Medium | LuaJIT x64 uses different pointer model — test `lua_touserdata` paths |
| Third-party lib version mismatch | Medium | Pin exact versions, build from source where possible |

---

## Estimated Effort

| Phase | Days | Complexity |
|-------|------|-----------|
| 0 — Setup | 1 | Low |
| 1 — Third-party libs | 3 | Medium |
| 2 — Code fixes | 8 | **High** |
| 3 — Protocol audit | 3 | Medium |
| 4 — Build & link | 3 | Medium |
| 5 — Testing | 4 | Medium |
| 6 — Cleanup | 3 | Low |
| **Total** | **~25 days** | |

---

## File Inventory — All 22 .vcxproj Needing x64 Config

1. `Libraries/AudioSDL/proj/AudioSDL.vcxproj`
2. `Libraries/common/Proj/Common.vcxproj`
3. `Libraries/Cryptopp/cryptlib.vcxproj` *(already has x64)*
4. `Libraries/EncLib/proj/EncLib.vcxproj`
5. `Libraries/ICUHelper/proj/ICUHelper.vcxproj`
6. `Libraries/InfoNet/proj/InfoNet.vcxproj`
7. `Libraries/Server/sdk/Proj/LIBDBC.vcxproj`
8. `Libraries/Util/src/logutil.vcxproj`
9. `Libraries/Status/Proj/Status.vcxproj`
10. `Libraries/CaLua/CaLua/CaLua.vcxproj`
11. `Libraries/CaLua/Lua/lua50.vcxproj`
12. `Libraries/CaLua/Lua/lualib.vcxproj`
13. `Client/proj/kop.vcxproj`
14. `Engine/proj/MindPower3D.vcxproj`
15. `Server/AccountServer/Proj/AccountServer.vcxproj`
16. `Server/GateServer/Proj/GateServer.vcxproj`
17. `Server/GameServer/Proj/GameServer.vcxproj` *(if exists)*
18. `Server/GroupServer/Proj/GroupServer.vcxproj`

---

## Key Principle

> **Original source at `C:\TalesOfPirate2016\Source` is READ-ONLY reference.**
> All x64 work happens in `C:\TalesOfPirateDX9\sources\`.
> Use the 2016 source only to compare logic when unclear about intent.
