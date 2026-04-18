$lines = [System.IO.File]::ReadAllLines('C:\TalesOfPirateDX9\Client\scripts\table\StringSet.txt', [System.Text.Encoding]::GetEncoding('gb2312'))
for($i=63; $i -lt 70 -and $i -lt $lines.Length; $i++) {
    Write-Host ('{0}: {1}' -f ($i+1), $lines[$i])
}
