import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')

# Check TableData.cpp
path = r'C:\TalesOfPirateDX9\sources\Libraries\common\src\TableData.cpp'
data = open(path, 'rb').read().decode('utf-8', errors='replace')
print("=== TableData.cpp ===")
for i, line in enumerate(data.split('\n'), 1):
    print(f'{i}: {line.rstrip()}')

# Also check MPResData.h implementation
path2 = r'C:\TalesOfPirateDX9\sources\Engine\include\MPResData.h'
data2 = open(path2, 'rb').read().decode('utf-8', errors='replace')
print("\n=== MPResData.h lines 370-430 ===")
for i, line in enumerate(data2.split('\n')[369:430], 370):
    print(f'{i}: {line.rstrip()}')
