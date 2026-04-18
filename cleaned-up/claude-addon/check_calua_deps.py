import subprocess, re

dump = r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\dumpbin.exe"

# Get exports from new DLL
result = subprocess.run([dump, '/EXPORTS', r'C:\TalesOfPirateDX9\Client\system\MindPower3D_D8R.dll'],
                       capture_output=True, text=True)
dll_exports = set()
for line in result.stdout.split('\n'):
    m = re.search(r'\s+\d+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+(\S+)', line)
    if m:
        dll_exports.add(m.group(1))
print(f"New DLL has {len(dll_exports)} exports")

# Check each DLL that imports from MindPower3D
for dll in ['CaLua.dll', 'PAI.DLL']:
    path = rf'C:\TalesOfPirateDX9\Client\system\{dll}'
    result2 = subprocess.run([dump, '/IMPORTS', path], capture_output=True, text=True)
    lines = result2.stdout.split('\n')
    in_mp = False
    imports = []
    for line in lines:
        if 'MindPower3D' in line:
            in_mp = True
            continue
        if in_mp:
            if '.dll' in line.lower() and 'MindPower3D' not in line and line.strip():
                break
            stripped = line.strip()
            if stripped and not stripped[0].isdigit() and '0x' not in stripped.lower():
                parts = stripped.split()
                if len(parts) >= 2:
                    imports.append(parts[-1])  # last token is symbol name
                elif len(parts) == 1 and '?' in parts[0]:
                    imports.append(parts[0])
    missing = [f for f in imports if f and f not in dll_exports]
    print(f"\n{dll}: {len(imports)} imports from MindPower3D_D8R.dll, {len(missing)} MISSING")
    for m in missing[:10]:
        print(f"  MISSING: {m}")
