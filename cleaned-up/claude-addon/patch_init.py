path = r'C:\TalesOfPirateDX9\sources\Client\src\GameAppInit.cpp'
with open(path, 'rb') as f:
    data = f.read()

# Patch 1: LoadTerrainSet
old1 = b'\tif(!LoadTerrainSet("scripts/table/TerrainInfo", FALSE))\r\n        return 0;\r\n'
new1 = (b'\tif(!LoadTerrainSet("scripts/table/TerrainInfo", FALSE))\r\n'
        b'\t{\r\n'
        b'\t\tLG("init", "[DBG] LoadTerrainSet FAILED");\r\n'
        b'        return 0;\r\n'
        b'\t}\r\n'
        b'\tLG("init", "[DBG] LoadTerrainSet OK");\r\n')

# Patch 2: LoadResourceSet
old2 = (b'\tif(!LoadResourceSet("scripts/table/ResourceInfo", g_Config.m_nMaxResourceNum, FALSE))\r\n'
        b'\t\treturn 0;\r\n')
new2 = (b'\tif(!LoadResourceSet("scripts/table/ResourceInfo", g_Config.m_nMaxResourceNum, FALSE))\r\n'
        b'\t{\r\n'
        b'\t\tLG("init", "[DBG] LoadResourceSet FAILED");\r\n'
        b'\t\treturn 0;\r\n'
        b'\t}\r\n'
        b'\tLG("init", "[DBG] LoadResourceSet OK");\r\n')

# Patch 3: LoadResource/LoadRes2
old3 = (b'\tif (!LoadResource() || !LoadRes2() /*|| !LoadRes3()*/)\r\n'
        b'\t{\r\n'
        b'\t\treturn 0;\r\n'
        b'\t}\r\n')
new3 = (b'\tif (!LoadResource() || !LoadRes2() /*|| !LoadRes3()*/)\r\n'
        b'\t{\r\n'
        b'\t\tLG("init", "[DBG] LoadResource/LoadRes2 FAILED");\r\n'
        b'\t\treturn 0;\r\n'
        b'\t}\r\n'
        b'\tLG("init", "[DBG] LoadResource/LoadRes2 OK");\r\n')

for old, new, name in [(old1, new1, 'TerrainSet'), (old2, new2, 'ResourceSet'), (old3, new3, 'LoadResource')]:
    if old in data:
        data = data.replace(old, new, 1)
        print(f'{name} patch OK')
    else:
        print(f'{name} pattern NOT FOUND')

with open(path, 'wb') as f:
    f.write(data)
print('Done.')
