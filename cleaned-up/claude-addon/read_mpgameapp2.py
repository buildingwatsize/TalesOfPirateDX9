import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
data = open(r'C:\TalesOfPirateDX9\sources\Engine\src\MPGameApp.cpp', 'rb').read().decode('utf-8', errors='replace')
lines = data.split('\n')
for i, line in enumerate(lines[76:155], 77):
    print(f'{i}: {line.rstrip()}')
