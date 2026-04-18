#include <TableData.h>

BOOL CRawDataSet::_LoadRawDataInfo_Bin(const char* pszFileName)
{
	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] enter file=%s\n",pszFileName);fflush(_tf);fclose(_tf);}}

	const unsigned char cluTableKey[] = { 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x2f, 0x41, 0x3f, 0x44, 0x28, 0x47, 0x2b, 0x4b, 0x62, 0x50 };
	const unsigned char cluTableIV[] =	{ 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };

	FILE* fp = fopen(pszFileName, "rb");
	char szMsg[MAX_PATH] = { 0 };

	if (fp == NULL)
	{
		LG2("error", "Load Raw Data Info Bin File [%s] Failed!\n", pszFileName);
		//sprintf(szMsg, "打开表格文件失败：%s\n程序即将退出!\n", pszFileName);
		//MessageBox(NULL, szMsg, "错误", MB_OK | MB_ICONERROR);
        sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
        MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] file not found: %s\n",pszFileName);fflush(_tf);fclose(_tf);}}
		return FALSE;
	}

	int nSize = Util_GetFileSize(fp);
	int nInfoSize = _GetRawDataInfoSize();

	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] fileSize=%d infoSize=%d idStart=%d idCnt=%d\n",nSize,nInfoSize,_nIDStart,_nIDCnt);fflush(_tf);fclose(_tf);}}

	LPBYTE pbtResInfo = new BYTE[nSize];
	fread(pbtResInfo, sizeof(char), nSize, fp);
	fclose(fp);

	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] pre decrypt nSize=%d\n",nSize);fflush(_tf);fclose(_tf);}}

	std::string* pSink = new(std::nothrow) std::string();
	if (!pSink) {
		{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] FAILED new string\n");fflush(_tf);fclose(_tf);}}
		delete[] pbtResInfo;
		return FALSE;
	}

	try { pSink->reserve(nSize); }
	catch (...) {}

	auto* pDecryptor = new(std::nothrow) CryptoPP::GCM<CryptoPP::AES>::Decryption();
	if (!pDecryptor) { delete pSink; delete[] pbtResInfo; return FALSE; }
	pDecryptor->SetKeyWithIV(cluTableKey, 16, cluTableIV, 16);

	auto* pDF = new(std::nothrow) CryptoPP::AuthenticatedDecryptionFilter(
		*pDecryptor, new CryptoPP::StringSink(*pSink),
		CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, 12);
	if (!pDF) { delete pDecryptor; delete pSink; delete[] pbtResInfo; return FALSE; }

	auto* pSS = new(std::nothrow) CryptoPP::StringSource(
		pbtResInfo, nSize, true, new CryptoPP::Redirector(*pDF));

	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] decrypted sinkSize=%llu\n",(unsigned long long)pSink->size());fflush(_tf);fclose(_tf);}}

	memset(pbtResInfo, 0, nSize);
	memcpy(pbtResInfo, pSink->c_str(), pSink->size());
	size_t decryptedSize = pSink->size();
	delete pSink;

	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] sink freed ok\n");fflush(_tf);fclose(_tf);}}

	int nResCnt = (int)decryptedSize / nInfoSize;
	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] resCnt=%d loop begin\n",nResCnt);fflush(_tf);fclose(_tf);}}

	int nLoaded = 0;
	for (int i = 0; i < nResCnt; i++)
	{
		CRawDataInfo* pInfo = (CRawDataInfo*)(pbtResInfo + i * nInfoSize);
		if (pInfo->bExist != 1) continue;
		if (IsValidID(pInfo->nID) == FALSE) continue;

		CRawDataInfo* pCurInfo = _GetRawDataInfo(pInfo->nID);
		memcpy(pCurInfo, pInfo, nInfoSize); // 替代原有的信息
		_IDIdx[pCurInfo->szDataName] = pCurInfo;
		_ProcessRawDataInfo(pCurInfo);
		nLoaded++;
		LG2("debug", "Load Bin RawData [%s] = %d\n", pCurInfo->szDataName, pCurInfo->nID);
	}

	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] loop done loaded=%d pre-delete\n",nLoaded);fflush(_tf);fclose(_tf);}}

	delete[] pbtResInfo;

	{FILE*_tf=fopen("log\\table_load.log","a");if(_tf){fprintf(_tf,"[BIN] post-delete returning TRUE\n");fflush(_tf);fclose(_tf);}}

	return TRUE;
}

void CRawDataSet::_WriteRawDataInfo_Bin(const char* pszFileName)
{

	const unsigned char cluTableKey[] = { 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x2f, 0x41, 0x3f, 0x44, 0x28, 0x47, 0x2b, 0x4b, 0x62, 0x50 };
	const unsigned char cluTableIV[] =	{ 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };
	FILE* fp = fopen(pszFileName, "wb");
	if (fp == NULL) return;
	char szMsg[MAX_PATH] = { 0 };
	DWORD dwInfoSize = _GetRawDataInfoSize();

	//fwrite(&dwInfoSize, 4, 1, fp);
	auto buffer = std::make_unique<BYTE[]>(dwInfoSize * _nIDCnt);
	for (int i = 0; i < _nIDCnt; i++) {
		CRawDataInfo* pInfo = (CRawDataInfo*)((LPBYTE)_RawDataArray + i * _GetRawDataInfoSize());
		if (pInfo->bExist)
		{
			memcpy(buffer.get() + (i * dwInfoSize), pInfo, dwInfoSize);
		}

	}

	CryptoPP::GCM<CryptoPP::AES>::Encryption e;
	std::string sink;
	std::string base64;
	e.SetKeyWithIV(cluTableKey, 16, cluTableIV, 16);
	CryptoPP::StringSource ss(buffer.get(), (size_t)(dwInfoSize * _nIDCnt), true, new CryptoPP::AuthenticatedEncryptionFilter(e, new CryptoPP::StringSink(sink), false, 12));
	//CryptoPP::StringSource(sink, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(base64), false));
	fwrite(sink.c_str(), sizeof(char), (dwInfoSize*_nIDCnt) + 12, fp);

	//if (sink.size() != (dwInfoSize * _nIDCnt) + 12) {
	//	sprintf(szMsg, "Size:%d\n, expected:%d\n", sink.size(), (dwInfoSize * _nIDCnt) + 12);
	//	MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
	//}
	fclose(fp);
}