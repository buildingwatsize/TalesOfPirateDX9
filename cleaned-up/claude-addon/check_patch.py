import sys

# Check if [DBG] is in the Game.exe binary
with open(r'C:\TalesOfPirateDX9\Client\system\Game.exe', 'rb') as f:
    exe = f.read()
dbg_in_exe = b'[DBG]' in exe
print(f'[DBG] in Game.exe binary: {dbg_in_exe}')

# Check source
with open(r'C:\TalesOfPirateDX9\sources\Client\src\GameAppInit.cpp', 'rb') as f:
    src = f.read()
dbg_in_src = b'[DBG]' in src
print(f'[DBG] in GameAppInit.cpp source: {dbg_in_src}')

# Check debug_early in main
with open(r'C:\TalesOfPirateDX9\sources\Client\src\Main.cpp', 'rb') as f:
    main = f.read()
early_in_main = b'debug_early' in main
print(f'debug_early patch in Main.cpp: {early_in_main}')

# Show the exact lines with DBG in source
if dbg_in_src:
    for i, line in enumerate(src.split(b'\n'), 1):
        if b'[DBG]' in line or b'debug_early' in line:
            print(f'  src line {i}: {line.decode("utf-8", errors="replace").strip()}')
