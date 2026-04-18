import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')

path = r'C:\TalesOfPirateDX9\sources\Libraries\common\include\TableData.h'
data = open(path, 'rb').read().decode('utf-8', errors='replace')
lines = data.split('\n')

# Find _LoadRawDataInfo_Bin and show the AES/GCM section
in_bin = False
for i, line in enumerate(lines, 1):
    if '_LoadRawDataInfo_Bin' in line:
        in_bin = True
    if in_bin:
        print(f'{i}: {line.rstrip()}')
        if i > 50 and in_bin:  # safety limit
            break
        # Stop after closing brace of function
        if in_bin and line.strip() == '}' and i > 10:
            break
