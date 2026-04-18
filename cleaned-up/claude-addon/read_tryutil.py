import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
data = open(r'C:\TalesOfPirateDX9\sources\Libraries\Util\include\TryUtil.h', 'rb').read().decode('utf-8', errors='replace')
for i, line in enumerate(data.split('\n'), 1):
    print(f'{i}: {line.rstrip()}')
