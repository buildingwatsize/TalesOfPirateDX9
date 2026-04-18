Add-Type -AssemblyName System.Drawing
$fc = New-Object System.Drawing.Text.PrivateFontCollection
$fc.AddFontFile("C:\TalesOfPirateDX9\thai\MiSansThai.ttf")
foreach($f in $fc.Families) { Write-Host $f.Name }
