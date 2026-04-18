
"""
D3D9 display format diagnostic
Determines what GetAdapterDisplayMode returns and whether
CheckDeviceFormat succeeds for A8R8G8B8 / A4R4G4B4.
"""
import ctypes, ctypes.wintypes, sys, struct

# D3D9 SDK version
D3D_SDK_VERSION = 32

# D3DFORMAT values
D3DFMT_UNKNOWN    = 0
D3DFMT_R8G8B8    = 20
D3DFMT_A8R8G8B8  = 21
D3DFMT_X8R8G8B8  = 22
D3DFMT_R5G6B5    = 23
D3DFMT_X1R5G5B5  = 24
D3DFMT_A1R5G5B5  = 25
D3DFMT_A4R4G4B4  = 26
D3DFMT_A2R10G10B10 = 35

NAMES = {0:'UNKNOWN',20:'R8G8B8',21:'A8R8G8B8',22:'X8R8G8B8',
         23:'R5G6B5',24:'X1R5G5B5',25:'A1R5G5B5',26:'A4R4G4B4',35:'A2R10G10B10'}

D3DDEVTYPE_HAL = 1
D3DADAPTER_DEFAULT = 0
D3DRTYPE_TEXTURE = 3

class D3DDISPLAYMODE(ctypes.Structure):
    _fields_ = [('Width',ctypes.c_uint),('Height',ctypes.c_uint),
                ('RefreshRate',ctypes.c_uint),('Format',ctypes.c_uint)]

# Load d3d9.dll
d3d9 = ctypes.windll.d3d9
d3d9.Direct3DCreate9.restype = ctypes.c_void_p
d3d9.Direct3DCreate9.argtypes = [ctypes.c_uint]

pD3D = d3d9.Direct3DCreate9(D3D_SDK_VERSION)
if not pD3D:
    print("ERROR: Direct3DCreate9 returned NULL")
    sys.exit(1)

print(f"D3D9 object created: 0x{pD3D:016X}")

# IDirect3D9 vtable layout (COM interface)
# vtable[0]=QueryInterface, [1]=AddRef, [2]=Release
# vtable[3]=RegisterSoftwareDevice, [4]=GetAdapterCount
# vtable[5]=GetAdapterIdentifier, [6]=GetAdapterModeCount
# vtable[7]=EnumAdapterModes, [8]=GetAdapterDisplayMode
# vtable[9]=CheckDeviceType, [10]=CheckDeviceFormat
# vtable[11]=CheckDeviceMultiSampleType, [12]=CheckDepthStencilMatch
# vtable[13]=CheckDeviceFormatConversion, [14]=GetDeviceCaps
# vtable[15]=GetAdapterMonitor, [16]=CreateDevice

vtable_ptr = ctypes.cast(pD3D, ctypes.POINTER(ctypes.c_void_p))[0]
vtable = ctypes.cast(vtable_ptr, ctypes.POINTER(ctypes.c_void_p))

# GetAdapterDisplayMode: vtable[8]
GetAdapterDisplayMode = ctypes.WINFUNCTYPE(ctypes.c_long, ctypes.c_void_p, ctypes.c_uint, ctypes.POINTER(D3DDISPLAYMODE))(vtable[8])
mode = D3DDISPLAYMODE()
hr = GetAdapterDisplayMode(pD3D, D3DADAPTER_DEFAULT, ctypes.byref(mode))
print(f"GetAdapterDisplayMode HRESULT: 0x{hr & 0xFFFFFFFF:08X}")
print(f"Display mode: {mode.Width}x{mode.Height} @ {mode.RefreshRate}Hz, Format={mode.Format} ({NAMES.get(mode.Format,'?')})")

# CheckDeviceFormat: vtable[10]
# HRESULT CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat,
#                           DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
CheckDeviceFormat = ctypes.WINFUNCTYPE(
    ctypes.c_long, ctypes.c_void_p,
    ctypes.c_uint, ctypes.c_uint, ctypes.c_uint,
    ctypes.c_ulong, ctypes.c_uint, ctypes.c_uint)(vtable[10])

adapter_fmt = mode.Format
for check_fmt, name in [(D3DFMT_A8R8G8B8,'A8R8G8B8'),(D3DFMT_A4R4G4B4,'A4R4G4B4'),(D3DFMT_X8R8G8B8,'X8R8G8B8'),(D3DFMT_R5G6B5,'R5G6B5')]:
    hr2 = CheckDeviceFormat(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapter_fmt, 0, D3DRTYPE_TEXTURE, check_fmt)
    status = "OK" if hr2 == 0 else f"FAILED(0x{hr2 & 0xFFFFFFFF:08X})"
    print(f"  CheckDeviceFormat({name} as texture): {status}")

# Also try with X8R8G8B8 as adapter format (standard fallback)
if adapter_fmt != D3DFMT_X8R8G8B8:
    print(f"\nRetrying with X8R8G8B8 adapter format:")
    for check_fmt, name in [(D3DFMT_A8R8G8B8,'A8R8G8B8'),(D3DFMT_A4R4G4B4,'A4R4G4B4')]:
        hr2 = CheckDeviceFormat(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, check_fmt)
        status = "OK" if hr2 == 0 else f"FAILED(0x{hr2 & 0xFFFFFFFF:08X})"
        print(f"  CheckDeviceFormat({name} with X8R8G8B8 adapter): {status}")

# Release
Release = ctypes.WINFUNCTYPE(ctypes.c_ulong, ctypes.c_void_p)(vtable[2])
Release(pD3D)
print("\nDone.")
