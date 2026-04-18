# Thai Language (th_TH) Support — Task Tracker

**Goal:** Make the client support Thai language and successfully build `talesofpirates.sln`.
**Font note:** `Browd.ttf` = BrowalliaDSE, supports Thai glyphs — keep font folder as-is.

---

## Status Key
- [ ] pending
- [x] done
- [~] skipped / N/A

---

## Tasks

### 1. [x] Fix linker error — `Instance()` + `_instance` in `ResourceBundleManage.cpp`
**File:** `sources/Libraries/ICUHelper/src/ResourceBundleManage.cpp`
**Problem:** `CResourceBundleManage::Instance()` declared in header but never implemented → unresolved external symbol.
**Fix:**
- Add `CResourceBundleManage* CResourceBundleManage::_instance = nullptr;` as static definition.
- In the constructor (configFileName overload), add `_instance = this;` so the global `g_ResourceBundleManage("Game.loc")` auto-registers.
- Implement `Instance()` to return `_instance`.

---

### 2. [x] Fix ICU converter — locale-aware codepage
**File:** `sources/Libraries/ICUHelper/src/ResourceBundleManage.cpp` (line ~126)
**Problem:** `ucnv_open(NULL, &status)` uses system default codepage — may not be CP874 on non-Thai Windows.
**Fix:** After reading locale string, if `th_TH` → open `windows-874-2000`; else fall back to NULL (system default).

---

### 3. [x] Update `Game.loc` — switch locale to th_TH
**File:** `Client/system/Game.loc`
**Change:** `locale = en_US` → `locale = th_TH`
**Effect:** ICU ResourceBundle loads `system/th_TH.res` (1.2 MB, already present).

---

### 4. [x] Update `StringSet.txt [0]` — setlocale string for Thai
**File:** `Client/scripts/table/StringSet.txt`
**Change:** `[0]	"english"` → `[0]	"Thai"`
**Effect:** `setlocale(LC_CTYPE, "Thai")` enables Thai character handling in C runtime.
**Note:** No `.bin` regen needed — game falls back to `.txt` when `.bin` is absent.
**⚠ Gotcha:** Edit tool silently failed first time. Used Desktop Commander `edit_block` to confirm write on Windows path.

---

### 5. [x] Fix `font1.xml` resolution — match Client 1
**File:** `Client/font/font1.xml`
**Change:** `NativeHorzRes="800" NativeVertRes="600"` → `NativeHorzRes="1024" NativeVertRes="768"`
**Reason:** Client 1 (K:\pko-advanture) uses 1024x768 for correct font scaling. 800x600 causes Thai glyphs to render at wrong size.

---

## Comparison vs Reference Clients

| Aspect | Client 1 (K:\pko-advanture) | Client 2 (L:\PKO ChaoS) | Our Client |
|--------|-----------------------------|--------------------------|------------|
| Game.loc locale | th_TH | th_TH | th_TH ✅ |
| th_TH.res | 1,236,416 B | 1,238,012 B (older 2021) | 1,236,416 B ✅ identical to C1 |
| StringSet.txt [0] | *no file* → system locale | *no file* → system locale | "Thai" ✅ |
| font1.xml resolution | 1024x768 | 800x600 | 1024x768 ✅ fixed |
| ICU converter | system default | system default | windows-874-2000 ✅ explicit |
| Instance() singleton | implemented | implemented | implemented ✅ fixed |

---

### 6. [x] Build `talesofpirates.sln` — verify no errors
**Tool:** Desktop Commander → MSBuild.exe (VS2022)
**Command:** `MSBuild.exe talesofpirates.sln /p:Configuration=Release /p:Platform=x86 /m /v:minimal`
**Result:** ✅ **0 Errors, 25 Warnings** (all pre-existing: LNK4075, MSB8029 — non-blocking)

---

### 7. [x] Fix engine DLL deployment — `MindPower3D_D8R.dll`
**Problem:** `Client/system/MindPower3D_D8R.dll` was 0 bytes (placeholder). Engine project has no post-build copy step. Game imports this DLL; 0 bytes = unloadable engine.
**Fix:** Copy built DLL: `sources/Engine/lib/MindPower3D_D8R.dll` (4,260,352 B) → `Client/system/MindPower3D_D8R.dll`
**Note:** Must kill Game.exe first (Windows file lock). Use `Remove-Item` then `Copy-Item`.

---

### 8. [x] Encode all Lua `.bin` files from `.clu` sources
**Problem:** All `.bin` files in `Client/scripts/lua/` were either 0 bytes or stale bytecode mismatched to source. `CLU_LoadScript()` decodes bytes with `aux[i] -= 23` then passes to `luaL_loadbuffer`. Encoding = `src[i] + 23`. Old non-zero .bin files (gui.bin, main.bin, etc.) were also stale — gui.bin had line 884 syntax error.
**Fix:** Re-encode ALL .bin files from current .clu sources (PowerShell: `($_ + 23) -band 0xFF`).
**Remaining without .clu:** `missioninfo.bin`, `mission.bin` (forms), `scripts.bin` — no source available.

---

### 9. [x] Game opens and shows login screen
**Result:** Game launches, loads ~105 MB resources, inits in ~7.7 s, renders login background ✅
**Remaining non-fatal issues:**
- `PAI.dll not found` Windows dialog — appears then game continues (non-fatal)
- `NULL GUI` × many in gui.log — missing UI elements from stale/missing forms
- `ui.clu does not have defined interface frmMain800` — login form not wired up
- `CameraConf.bin`: `attempt to call global 'P' (a nil value)` — camera config Lua error
- `missioninfo.bin`: unfinished long comment (no .clu source to re-encode)

**Key architecture note:** `.bin` files are Lua source files encoded with `+23` per-byte (not compiled bytecode). `.luc` files (apple.luc, pear.luc) are the actual compiled bytecode format used by other scripts.