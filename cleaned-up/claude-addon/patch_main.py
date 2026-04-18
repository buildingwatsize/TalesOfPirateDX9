path = r'C:\TalesOfPirateDX9\sources\Client\src\Main.cpp'
with open(path, 'rb') as f:
    data = f.read()

# Find the WinMain entry point line and add raw file write immediately after
old = b'int APIENTRY _tWinMain(HINSTANCE hInstance,\r\n                     HINSTANCE hPrevInstance,\r\n                     LPTSTR    lpCmdLine,\r\n                     int       nCmdShow)\r\n{'
new = (b'int APIENTRY _tWinMain(HINSTANCE hInstance,\r\n'
       b'                     HINSTANCE hPrevInstance,\r\n'
       b'                     LPTSTR    lpCmdLine,\r\n'
       b'                     int       nCmdShow)\r\n'
       b'{\r\n'
       b'\t{ FILE* _dbgf=fopen("log\\\\debug_early.log","w"); if(_dbgf){fprintf(_dbgf,"WinMain entered\\n");fclose(_dbgf);} }')

if old in data:
    data = data.replace(old, new, 1)
    print('Main.cpp patch OK')
else:
    print('Pattern not found, showing lines 110-115:')
    lines = data.split(b'\n')
    for i,l in enumerate(lines[109:116], 110):
        print(i, repr(l))

with open(path, 'wb') as f:
    f.write(data)
