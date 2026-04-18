# Plan 2: Enable FontModule / CEGUI for Proper Thai Unicode Rendering

## Overview
PKO ChaoS (`L:\pko-from-nb\zone2\PKO ChaoS`) uses CEGUI + FreeType for proper Unicode font rendering.
This plan enables the same `FontModule` path in our DX9 build for full Thai support without GDI hacks.

## Goal
Replace the GDI-based `CMPFont` renderer with CEGUI FreeType, which handles:
- Thai Unicode combining marks natively (no zero-width hacks needed)
- Correct baseline / vertical metrics automatically
- Proper single-byte Thai CP874 without DBCS collision

---

## Steps

### 1. Copy CEGUI DLLs from PKO ChaoS
Source: `L:\pko-from-nb\zone2\PKO ChaoS\`
Copy to: `C:\TalesOfPirateDX9\Client\`

Files needed:
- `CEGUIBase.dll`
- `CEGUIExpatParser.dll`
- `CEGUIDirect3D9Renderer.dll` (if present)

### 2. Create `scripts/mindpower.cfg`
Path: `C:\TalesOfPirateDX9\Client\scripts\mindpower.cfg`

```ini
FontImplFactory = CEGUI
FontOffsetX = 0
FontOffsetY = 0
```

Reference: `L:\pko-from-nb\zone2\PKO ChaoS\scripts\mindpower.cfg`

### 3. Uncomment FontSystem init in MPGameApp.cpp
Search for: `FontModule::FontSystem::getSingleton().init()`
File: `C:\TalesOfPirateDX9\sources\Client\src\MPGameApp.cpp` (approximate)

Remove the `//` comment prefix to enable it.

### 4. Uncomment D3D reset calls in lwDeviceObject.cpp
File: `C:\TalesOfPirateDX9\sources\Engine\src\lwDeviceObject.cpp`

Search for commented-out FontModule reset/release calls around `OnResetDevice` / `OnLostDevice`.
Uncomment them so CEGUI renderer resets properly on device loss.

### 5. Build CEGUI DX9 Renderer (if DLL missing)
If `CEGUIDirect3D9Renderer.dll` is not in PKO ChaoS, build from source:
- CEGUI 0.6.x or 0.7.x branch (matches PKO era)
- DirectX 9 renderer backend
- Output: `CEGUIDirect3D9Renderer.dll`

### 6. Rebuild Engine + Client
```
C:\TalesOfPirateDX9\sources\Engine\  → Engine.dll
C:\TalesOfPirateDX9\sources\Client\ → PKO.exe (or equivalent)
```

---

## Why Plan 1 First?
Plan 1 (CMPFont Thai single-byte fix) is a surgical in-engine patch that works within the existing GDI pipeline.
Plan 2 replaces the pipeline entirely — higher risk, higher reward.
Do Plan 2 only after confirming Plan 1 still has issues (combining marks, ligatures, vertical metrics).

---

## Risk / Notes
- CEGUI version must match what the engine was compiled against. Check `#include <CEGUI.h>` version guards.
- `mindpower.cfg` key name must match exactly — check `CFG_KEY_FONT_IMPL` or similar define in source.
- If `FontImplFactory` key is not parsed, the engine silently falls back to CMPFont (GDI). Add a log check.
