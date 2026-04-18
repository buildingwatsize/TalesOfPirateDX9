path = r'C:\TalesOfPirateDX9\sources\Client\src\GameAppInit.cpp'
with open(path, 'rb') as f:
    data = f.read()

# Wrap entire _Init body with try-catch that logs exception to a file
# Add try block right after the opening brace of _Init
old = b'BOOL CGameApp::_Init()\r\n{\r\n\t_AniClock = new CAniClock[MAX_ANI_CLOCK];\r\n'
new = (b'BOOL CGameApp::_Init()\r\n'
       b'{\r\n'
       b'\ttry {\r\n'
       b'\t_AniClock = new CAniClock[MAX_ANI_CLOCK];\r\n')

if old in data:
    data = data.replace(old, new, 1)
    print('Opening brace patch OK')
else:
    print('Opening pattern NOT found, showing context:')
    lines = data.split(b'\n')
    for i, l in enumerate(lines[187:195], 188):
        print(i, repr(l))

# Find the return 0 lines and add a closing catch before them
# Actually, better: add early-catch before LoadTerrainSet
# Let's just add a separate try-catch around LoadTerrainSet call

with open(path, 'wb') as f:
    f.write(data)

# Try different approach - add try-catch just around LoadTerrainSet  
with open(path, 'rb') as f:
    data = f.read()

old2 = (b'\tif(!LoadTerrainSet("scripts/table/TerrainInfo", FALSE))\r\n'
        b'\t{\r\n'
        b'\t\tLG("init", "[DBG] LoadTerrainSet FAILED");\r\n')
new2 = (b'\ttry {\r\n'
        b'\tif(!LoadTerrainSet("scripts/table/TerrainInfo", FALSE))\r\n'
        b'\t{\r\n'
        b'\t\tLG("init", "[DBG] LoadTerrainSet FAILED");\r\n')

if old2 in data:
    print('LoadTerrainSet try-wrap patch would apply')
else:
    print('LoadTerrainSet try-wrap pattern not found')
    
print('Done - no changes for try-wrap (separate approach)')
