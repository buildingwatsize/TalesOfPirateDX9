import subprocess, re

dump = r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\dumpbin.exe"

# Get imports from Game.exe
result = subprocess.run([dump, '/IMPORTS', r'C:\TalesOfPirateDX9\Client\system\Game.exe'],
                       capture_output=True, text=True)
lines = result.stdout.split('\n')

# Extract only the section for MindPower3D_D8R.dll
in_mp_section = False
game_imports = []
for line in lines:
    if 'MindPower3D_D8R.dll' in line:
        in_mp_section = True
        continue
    if in_mp_section:
        if '.dll' in line.lower() and 'MindPower3D' not in line:
            break  # next DLL section
        # function names are indented, contain ?
        stripped = line.strip()
        if stripped and not stripped.startswith('Section') and not stripped[0].isdigit():
            # it's a function name
            game_imports.append(stripped)

print(f"Game.exe imports {len(game_imports)} functions from MindPower3D_D8R.dll")

# Get exports from new DLL
result2 = subprocess.run([dump, '/EXPORTS', r'C:\TalesOfPirateDX9\Client\system\MindPower3D_D8R.dll'],
                        capture_output=True, text=True)
dll_exports = set()
for line in result2.stdout.split('\n'):
    m = re.search(r'\s+\d+\s+[0-9A-F]+\s+[0-9A-F]+\s+(\S+)', line)
    if m:
        dll_exports.add(m.group(1))

print(f"New DLL exports {len(dll_exports)} functions")

# Find missing
missing = [f for f in game_imports if f not in dll_exports]
print(f"Missing from new DLL: {len(missing)}")
if missing:
    for m in missing[:20]:
        print(f"  MISSING: {m}")
