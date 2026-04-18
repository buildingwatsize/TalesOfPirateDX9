import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
data = open(r'C:\TalesOfPirateDX9\sources\Client\src\Main.cpp', 'rb').read().decode('utf-8', errors='replace')
lines = data.split('\n')
# Show lines 520-560 and 700-730
print("=== Lines 520-560 ===")
for i, line in enumerate(lines[519:560], 520):
    print(f'{i}: {line.rstrip()}')
print("\n=== Lines 705-730 ===")
for i, line in enumerate(lines[704:731], 705):
    print(f'{i}: {line.rstrip()}')
