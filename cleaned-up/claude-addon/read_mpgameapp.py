data = open(r'C:\TalesOfPirateDX9\sources\Engine\src\MPGameApp.cpp', 'rb').read().decode('utf-8', errors='replace')
lines = data.split('\n')
keywords = ['return', '_Init', 'InitInput', 'ShowWindow', 'Init(', 'LG(']
for i, line in enumerate(lines, 1):
    if any(k in line for k in keywords):
        print(f'{i}: {line.rstrip()}')
