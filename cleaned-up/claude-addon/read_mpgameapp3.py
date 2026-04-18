import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
data = open(r'C:\TalesOfPirateDX9\sources\Engine\src\MPGameApp.cpp', 'rb').read().decode('utf-8', errors='replace')
lines = data.split('\n')
# Show lines 405-460
print("=== Lines 405-460 (LoadTerrainSet/LoadResourceSet) ===")
for i, line in enumerate(lines[404:460], 405):
    print(f'{i}: {line.rstrip()}')
