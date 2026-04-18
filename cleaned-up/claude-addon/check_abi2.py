import subprocess

dump = r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\dumpbin.exe"

# Get raw imports from Game.exe - show MindPower3D section
result = subprocess.run([dump, '/IMPORTS', r'C:\TalesOfPirateDX9\Client\system\Game.exe'],
                       capture_output=True, text=True)
lines = result.stdout.split('\n')

in_mp = False
for line in lines:
    if 'MindPower3D' in line:
        in_mp = True
    if in_mp:
        print(repr(line))
        if in_mp and '.dll' in line.lower() and 'MindPower3D' not in line and line.strip():
            break
        if line.strip() == '' and in_mp and any(l.strip() for l in lines[lines.index(line)+1:lines.index(line)+3]):
            pass  # keep going through blanks

print("\n=== Searching for these symbols in DLL exports ===")
sym1 = '??0CGroup_ParamSet@@QAE@HH@Z'
sym2 = '??0CMPEffectCtrl@@QAE@XZ'

result2 = subprocess.run([dump, '/EXPORTS', r'C:\TalesOfPirateDX9\Client\system\MindPower3D_D8R.dll'],
                        capture_output=True, text=True)
for sym in [sym1, sym2]:
    found = sym in result2.stdout
    print(f"  {sym}: {'FOUND' if found else 'NOT FOUND'} in new DLL")
