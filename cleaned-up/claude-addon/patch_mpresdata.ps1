$path = "C:\TalesOfPirateDX9\sources\Engine\include\MPResData.h"
$content = [System.IO.File]::ReadAllText($path, [System.Text.Encoding]::UTF8)

# New AES-GCM implementation
$newFunc = @'
inline BOOL CRawDataSet::_LoadRawDataInfo_Bin(const char *pszFileName)
{
	const unsigned char cluTableKey[] = { 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x2f, 0x41, 0x3f, 0x44, 0x28, 0x47, 0x2b, 0x4b, 0x62, 0x50 };
	const unsigned char cluTableIV[]  = { 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };

	FILE* fp = fopen(pszFileName, "rb");
	char szMsg[MAX_PATH] = { 0 };

	if (fp == NULL)
	{
		LG2("error", "Load Raw Data Info Bin File [%s] Failed!\n", pszFileName);
		sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
		MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	int nSize     = Util_GetFileSize(fp);
	int nInfoSize = _GetRawDataInfoSize();

	LPBYTE pbtResInfo = new BYTE[nSize];
	fread(pbtResInfo, sizeof(char), nSize, fp);
	fclose(fp);

	std::string sink;
	CryptoPP::GCM<CryptoPP::AES>::Decryption d;
	d.SetKeyWithIV(cluTableKey, 16, cluTableIV, 16);

	CryptoPP::AuthenticatedDecryptionFilter df(d, new CryptoPP::StringSink(sink),
		CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, 12);
	CryptoPP::StringSource ss(pbtResInfo, nSize, true, new CryptoPP::Redirector(df));

	memset(pbtResInfo, 0, nSize);
	memcpy(pbtResInfo, sink.c_str(), sink.size());

	int nResCnt = (int)sink.size() / nInfoSize;
	for (int i = 0; i < nResCnt; i++)
	{
		CRawDataInfo *pInfo = (CRawDataInfo*)(pbtResInfo + i * _GetRawDataInfoSize());
		if (pInfo->bExist != 1) continue;
		if (IsValidID(pInfo->nID) == FALSE) continue;
		CRawDataInfo *pCurInfo = _GetRawDataInfo(pInfo->nID);
		memcpy(pCurInfo, pInfo, nInfoSize);
		_IDIdx[pCurInfo->szDataName] = pCurInfo;
		_ProcessRawDataInfo(pCurInfo);
		LG2("debug", "Load Bin RawData [%s] = %d\n", pCurInfo->szDataName, pCurInfo->nID);
	}

	delete[] pbtResInfo;
	return TRUE;
}

'@

# Use regex to replace from the start of _LoadRawDataInfo_Bin up to (not including) _WriteRawDataInfo_Bin
# The (?s) flag makes . match newlines
$pattern = '(?s)inline BOOL CRawDataSet::_LoadRawDataInfo_Bin.*?(?=inline void CRawDataSet::_WriteRawDataInfo_Bin)'

$newContent = [regex]::Replace($content, $pattern, $newFunc)

if ($newContent -eq $content) {
    Write-Host "ERROR: Pattern not found - no replacement made"
    exit 1
}

[System.IO.File]::WriteAllText($path, $newContent, [System.Text.Encoding]::UTF8)
Write-Host "SUCCESS: MPResData.h patched with AES-GCM implementation"

# Verify
$verify = [System.IO.File]::ReadAllText($path, [System.Text.Encoding]::UTF8)
if ($verify -match 'pbtResInfo\[i\] -= 15') {
    Write-Host "WARNING: byte-shift still present!"
} else {
    Write-Host "CONFIRMED: byte-shift removed"
}
if ($verify -match 'GCM<CryptoPP::AES>::Decryption') {
    Write-Host "CONFIRMED: AES-GCM implementation present"
} else {
    Write-Host "WARNING: AES-GCM not found!"
}
