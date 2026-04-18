
# TalesOfPirateDX9

**About This Repository**
**This repository contains the source code for _Tales of Pirate_ (also known as King of Pirates / 海盗王), updated to DirectX 9. Over the past few years, we collaborated with several freelancers to modernize the engine from its original DX8 foundation.**

**In response to certain groups within the community profiting from leaked versions of this code, we have decided to make the source public for the benefit of all developers. Please note that we do not offer support for future bugs or technical issues.**

While the core DX9 upgrade is functional, it originated from a separate development branch; you may encounter minor inconsistencies. This remains an excellent starting point for new projects. Finally, we are not responsible for any legacy bugs or exploits present in the original code, as these were inherited from previous owners.

### **Credits & Copyright**

**Ownership & Contributions** All rights to the engine upgrades and source modifications belong to **[SatisfyTeam](https://satisfy.live/)**, **Mothannkh**, and the private benefactors who funded the project’s development but chose to remain anonymous.

**Terms of Use** This source is provided to the community for development purposes. While we encourage innovation, we ask that users respect the work put in by the team and the contributors who made this progress possible.

### CURRENT SOURCE SPECIFICATIONS

**Core Engine & Networking**

-   Upgraded from DirectX 8 to DirectX 9
    
-   Client and Server built with VS 2022 (C++14)
    
-   TopNet source integration
    
-   RSA/AES connection encryption (Client/Server)
    
-   Encrypted .clu files
    
-   Core packets updated to use Smart Pointers
    
-   Anti-WPE and Anti-DDoS protection
    
-   Anti-Dupe system
    

**Gameplay Systems**

-   Offline Stalls and Offline Mode
    
-   Full Mount System
    
-   Guild Bank with transaction logs
    
-   AuthorizedGM system
    
-   Reworked Jackpot system
    
-   In-game Shop remade (Classic style, 9 items per page)
    

**User Interface & Features**

-   Multi-resolution support
    
-   60 FPS toggle option
    
-   HP and SP bars displayable as values or percentages
    
-   World Map with Zoom In/Out
    
-   Drop Info with integrated Drop Filter
    
-   Right-click inventory menu options
    
-   Friend/Enemy combat modes
    
-   Visibility toggles for Effects, Apps, and Mounts
    

**Scripting & Structure**

-   Restructured LUA folders for better organization
    
-   Balanced Vanilla LUA setup
    
-   Simplified file structure with intuitive variable and function names
    
-   Modular design for easy code migration (copy/paste friendly)
vs config file 
**[Here](https://drive.google.com/file/d/1GR8GdSNe-UocUNpkvChoHBaqZJaTcuuS/view)**


--------- BELOW THIS IS MY CHANGES -------------
---

```markdown
## Upgraded Note:

- some fonts
- lvl 130 limit
- items in game & mall
- fairy
- rate (s): forge, exp,
- config
- 64-bit
- ICU lib 78.3
- npc (s)
- minor Eng translated
- DXVK
- add mempalace
- stuffs that made by Claude and Cursor
```

---

## Summary: Tales of Pirate DX9 - x64 Migration Session

### Successes (What We Fixed)
1. x64 Client/Server Build Pipeline

- Migrated the entire client (Game.exe), engine (MindPower3D_D8R.dll), and all four servers (AccountServer, GroupServer, GateServer, GameServer) from x86 to x64 using MSVC 2022.
- Fixed pointer truncation, calling convention, and ABI mismatches throughout the codebase.

2. Window Size / DPI Scaling

- Game window was rendering at ~50% size. Fixed by adding SetProcessDpiAwarenessContext((HANDLE)-1) in GameAppInit.cpp.

3. Blue Background Screen

- Scene was rendering as solid blue. Root cause: camera configuration values in CameraConf.clu were wrong. Fixed by porting values from a working client.

4. Client Disconnect After Entering Map

- Server-side CParser::DoString had a re-entrancy bug where lua_settop(m_pSLua, 0) cleared the entire Lua stack during nested calls. Fixed by saving/restoring the stack base.
- Also disabled the aggressive ping/keepalive timeout that was kicking the client.

5. CaLua Float/Double ABI Mismatch

- On x64, CLU_RegisterFunction passed double values from Lua but C++ functions received them as float, corrupting all floating-point config values. Fixed CaLua's dispatch to correctly handle double parameters.

6. Font Rendering - General Readability

- Fonts were completely unreadable (invisible/black). Bypassed the D3DX effect file system (eff.fx) for font rendering and directly set all D3D9 render states (alpha blending, texture modulation, etc.) via a new ApplyFontRenderStates() function in MPFont.cpp.

7. UI Color Overflow (x64 ABI - DWORD Colors)

- All UI text colors (character names, chat content, labels) appeared black. Root cause: Lua color constants like COLOR_WHITE = 4294967295 exceed INT_MAX. When passed as double to C++ functions expecting int, x64's cvttsd2si saturated them to 0x80000000 (semi-transparent black).
- Fixed by changing ~7 functions in UIScript.cpp (UI_SetChatColor, UI_SetTextColor, UI_SetHeadSayBkgColor, UI_SetLabelExFont, UI_SetEditCursorColor, UI_ComboSetTextColor, UI_SetListFontColor) to accept double params and cast to (DWORD)(unsigned int).

8. Chat Input / Channel Color Fixes

- Modified main.clu to set edtSay (chat input) and cboChannel (channel dropdown) text and cursor colors to COLOR_WHITE instead of COLOR_BLACK.

9. ICU Library Upgrade

- Upgraded ICU from v38 to v78 for Unicode support on x64. Deployed new DLLs (icudt78.dll, icuin78.dll, icuuc78.dll, icuio78.dll).

10. DXVK / SDL Integration

- Deployed DXVK (d3d9.dll) as the D3D9-to-Vulkan translation layer.
- Deployed SDL2/SDL3 DLLs for audio via AudioSDL.

### Remaining Issues (TODO)
1. Missing Buildings / Fences / Ladders

- Large scene objects (RBO system) are still not rendering. Needs investigation into the RBO loading path, frustum culling logic (CullPrimitive, IsSphereInFrustum), and lwStateCtrl object states. This is the biggest unresolved visual issue.

2. Thai Font Rendering (FreeType Integration)

- Thai text renders but with incorrect combining vowel/tone mark placement. Current solution uses GDI-based clustering, but the user explicitly rejected GDI in favor of FreeType (which other working Thai clients use). Needs FreeType library integration into the engine.

3. Font Size / Line-Height

- Some inconsistency in font sizing and line-height spacing across UI elements. May be related to the GDI-to-FreeType transition or the font config in font.clu.

4. Character Name Color on Top-Left Panel

- Was addressed in the DWORD color overflow fix. Needs user verification that the deployed fix actually resolved it.

### Files Cleaned Up

Removed from this session: MIGRATION_PLAN_LangRec_to_RES_STRING.md, ToP_Build_Progress.md, summarize-2026-04-09-22-06-10.md, dll_exports.txt, exports.txt, x64_patch.log.err, sources/mapping_report.txt, sources/mapping_summary.txt, claude-transcript/, Client/Game.exe (stale copy in wrong dir), Client/MindPower3D_D8R.dll (stale), Client/scripts/table/character_lvup.txt.100, debug logs (font_trace.log, model_trace.log, render_diag.log).

### Build Scripts Created

| Script | Purpose |
| :--- | :--- |
| `build_all_client.bat` | Full pipeline: kill game, build libs, engine, client, deploy, compile scripts |
| `build_libs.bat` | Build `lua50`, `CaLua`, `AudioSDL` |
| `build_engine.bat` | Build `MindPower3D_D8R.dll` |
| `build_client.bat` | Build `Game.exe` |
| `deploy_client.bat` | Kill game + copy `Game.exe`, `MindPower3D_D8R.dll`, `CaLua.dll` to `Client\system\` |
| `compile_scripts.bat` | Run CLU and Table compilation via `Game.exe pk cfT0PCaX clu_bin/table_bin` |
| `build_servers.bat` | Build all 4 server executables |
| `deploy_servers.bat` | Kill servers + copy all server EXEs to `server\` dirs |
| `launch_client.bat` | Quick-launch the game client |

#### Typical workflow after code changes:

1. Run build_all_client.bat (does everything in order), OR
2. Run individual scripts if you only changed one layer (e.g., just build_engine.bat + deploy_client.bat if you only touched MPFont.cpp)


---

## Watsize to-do

- [ ] the RES_STRING that prefix with CMISS -> try to rename it for easy maintainability, including somekind of `CL_LANGUAGE_MATCH_253`, should we add more context like the `CL_LANGUAGE_MATCH_253_STUFF_FOO_BAR`
- [ ] determine to remove trace log file, including LG func that using for tracing log, `fopen` into .log, also `printf` on template
- [ ] what's the _InstallVEH? 
- [ ] do we need a stub? for what? e.g. `pai_stub.cpp`
- [ ] implement the micro-transaction strategy, in-game purchases.

### In-game micro-transaction

*PAI (Player Authentication Interface)* handles player login/authentication against the publisher's billing servers. Looking at the API surface in `PAI.h`:


PAI.h: Lines 24-29
```
VOID   ipPAI_Init(BSTR *Location);
LONG   ipPAI_Login(LONG HWnd,LONG Code,LONG Port,BSTR *USER,BSTR *PASS,BSTR *GSVR);
LPCSTR ipPAI_Data();
LPCSTR ipPAI_Reply();
LPCSTR ipPAI_Passport();
LONG   ipPAI_Version();
```

- `ipPAI_Init` - Initializes the PAI client, connecting to the billing server IP (the commented-out 202.159.139.231 on line 21 was likely an Indonesian server)
- `ipPAI_Login` - Authenticates a player with username/password against the remote billing server, supporting different encryption modes (NONE, OSBP, DES, RSA as defined in the header)
- `ipPAI_Passport` - Returns a "passport" token after successful login, used as proof-of-authentication

*BTI (Billing Transaction Interface)* is the companion library that handles in-game microtransaction operations: purchases (`BUYS`), sales (`SALE`), trades between players (`DEAL`), credits (`GIVE`/`BILL`), and player data queries.

In this project, both PAI and BTI have been *stubbed out* in `sources/Libraries/Ipss/src/pai_stub.cpp` -- all functions are no-ops returning empty strings or failure codes. The `CPAI` constructor at line 22 also has `ipPAI_Init` commented out. The original `PAI.DLL` that shipped with the game client (`Client/system/PAI.DLL`) has been deleted per the git status. This all makes sense since the original iPartment billing servers are long dead, so the authentication and billing are now handled differently (likely via the custom `AccountServer` and HTTP-based login that the rest of `bill.cpp` implements).

---

### UI Script

What is `UIScript.cpp`?

`UIScript.cpp` is the **Lua-to-C++ GUI binding layer** for the game client in "Tales of Pirate" (also known as "King of Pirates" / KOP). It serves as the bridge that **exposes the native C++ GUI widget system to Lua scripts**, enabling the game's entire user interface to be built and controlled from Lua rather than hardcoded in C++.

#### Architecture at a Glance

The file has three logical sections:

**1. C++ Helper Functions** (lines 57-239)

- `EncryptAndRenameToWsd()` -- converts image filenames from `.png`/`.tga`/`.bmp`/`.dds` to an encrypted `.wsd` format using AES-GCM (via Crypto++). This is part of the asset protection pipeline.
- `CryptImage()` -- the actual AES-GCM encrypt/decrypt implementation for image assets using hardcoded keys.
- `GetList()` -- a utility that resolves a GUI ID into a `CList*`, handling both direct `CList` and `CListView`-wrapped lists.

**2. The `UI_*` Function Library** (lines 101-1812)

This is the core of the file -- roughly **100+ C-style functions**, each wrapping a specific GUI operation. They all follow a consistent pattern:
- Accept an integer `id` (widget handle) + parameters
- Look up the widget via `CGuiData::GetGui(id)`
- `dynamic_cast` to the expected type
- Call the underlying C++ method
- Return `R_OK` / `R_FAIL`

The widget types supported are defined in the `eCompentType` enum (lines 112-150):

```101:150:sources/Client/src/UIScript.cpp
int UI_LoadScript( char * file)
{
	CLU_LoadScript(file, 0); 
	// ...
}

enum eCompentType
{
	LABEL_TYPE		= 0,
	LABELEX_TYPE	= 1,
	BUTTON_TYPE		= 2,
	COMBO_TYPE		= 3,
	EDIT_TYPE		= 4,
	IMAGE_TYPE		= 5,
	LIST_TYPE		= 6,
	PROGRESS_TYPE	= 7,
	// ... through GUI_END
};
```

These functions cover:

| Category | Examples |
|---|---|
| **Form management** | `UI_CreateForm`, `UI_ShowForm`, `UI_SetFormStyle`, templates |
| **Widget creation** | `UI_CreateCompent` (factory for 31+ widget types), `UI_CreateListView` |
| **Image loading** | `UI_LoadImage`, `UI_LoadButtonImage`, `UI_LoadScaleImage` (all go through `EncryptAndRenameToWsd`) |
| **Layout/styling** | `UI_SetPos`, `UI_SetSize`, `UI_SetMargin`, `UI_SetAlpha`, `UI_SetTextColor` |
| **Lists & trees** | `UI_AddListText`, `UI_SetListRowHeight`, `UI_CreateSingleNode`, `UI_CreateGridNode` |
| **Grids** | `UI_SetGridUnitSize`, `UI_GridLoadSelectImage`, `UI_AddFaceToGrid` |
| **Editors & memos** | `UI_SetEditMaxNum`, `UI_SetMemoMaxNumPerRow`, `UI_RichSetMaxLine` |
| **Chat** | `UI_SetChatColor` (8 channel colors) |
| **HeadSay** | NPC speech bubble images (`UI_LoadHeadSayFaceImage`, life/mana bars) |
| **Menus** | `UI_MenuLoadImage`, `UI_MenuAddText` |
| **Text filters** | `UI_AddFilterTextToNameTable`, `UI_AddFilterTextToDialogTable` |
| **Fonts** | `UI_CreateFont` |
| **Localization** | `UI_ResString` (loads strings via `CResourceBundleManage`) |

**3. `MPInitLua_Gui()` -- The Registration Table** (lines 1820-1988)

This is called at startup to register every `UI_*` function with the Lua VM via `CLU_RegisterFunction()`. Each call maps a Lua-callable name to its C++ implementation with explicit type signatures:

```1834:1835:sources/Client/src/UIScript.cpp
CLU_RegisterFunction("UI_LoadScript", "int", "char*", CLU_CDECL, CLU_CAST(UI_LoadScript));
CLU_RegisterFunction("UI_SetDragSnapToGrid", "int", "int,int", CLU_CDECL, CLU_CAST(UI_SetDragSnapToGrid));
```

#### How It Fits in the Game

The `.clu` / `.bin` files you see in `Client/scripts/lua/forms/` (e.g., `login.clu`, `chat.clu`, `main.clu`) are compiled Lua scripts that call these `UI_*` functions to build the game's entire interface -- login screen, chat window, inventory, minimap, equipment panels, etc. The flow is:

1. Engine starts and calls `MPInitLua_Gui()` to register all bindings
2. Lua scripts (`.clu` files) are loaded and executed
3. Lua calls `UI_CreateForm()`, `UI_CreateCompent()`, `UI_LoadImage()`, etc. to build the UI
4. The C++ GUI framework (`CForm`, `CGuiData`, `CFormMgr`, etc.) manages rendering and input

#### Notable Details

- **Asset encryption**: All image loading paths go through `EncryptAndRenameToWsd()`, transparently converting `.png`/`.tga`/`.bmp`/`.dds` to AES-GCM encrypted `.wsd` files. There's also a `UI_LoadImageUnencrypted` variant for assets that skip encryption.
- **CaLua**: The Lua binding system (`CLU_RegisterFunction`, `CLU_LoadScript`, `CLU_CallScriptFunction`) is a custom C-based Lua bridge library, not standard LuaBridge/Sol.
- The file dates back to **2005/4/13** based on the header comment by "Arcol".

---

