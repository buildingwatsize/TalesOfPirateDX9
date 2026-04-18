import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
data = open(r'C:\TalesOfPirateDX9\sources\Client\src\Main.cpp', 'rb').read().decode('utf-8', errors='replace')
lines = data.split('\n')
# Show lines around T_B, T_E, TRY_END, catch
for i, line in enumerate(lines, 1):
    if any(k in line for k in ['T_B', 'T_E', 'T_FINAL', 'TRY_END', 'catch', 'exception']):
        print(f'{i}: {line.rstrip()}')
