$lines = [System.IO.File]::ReadAllLines('C:\TalesOfPirateDX9\sources\Client\src\Main.cpp', [System.Text.Encoding]::GetEncoding('gb2312'))
for($i=1143; $i -lt 1190 -and $i -lt $lines.Length; $i++) {
    Write-Host ('{0}: {1}' -f ($i+1), $lines[$i])
}
