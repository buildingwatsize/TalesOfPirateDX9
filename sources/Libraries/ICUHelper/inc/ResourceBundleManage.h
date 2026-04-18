#pragma once
#include "unicode/resbund.h"		//��Դ������Ϣ
#include "unicode/ucnv.h"			//�ַ�����ת��
#include "unicode/uclean.h"			//�ַ�����ת��
#include "unicode/msgfmt.h"			//��ʽ���ַ���
	
//#include "pi_Alloc.h"

#include <map>
#include <memory>
// Use to compare the contents of two pointers
struct charValueLess
{
	bool operator()(const char* _Left, const char*  _Right) const
	{
		if(_Left == NULL || _Right == NULL)
		{
			printf("%s\t%s\r\n",_Left,_Right);
			return false;
		}

		int ret = strcmp(_Left, _Right);	

		if(ret < 0)
			return true;
		else
			return false;
	}
};

typedef std::map <const char*, std::unique_ptr<char[]>, charValueLess> StringMap;
//typedef map <const char*, const char*, less<const char*> > StringMap;

class CFormatParameter
{
private:
	icu::Formattable* m_MsgArgs;
	int paraNum;
private:
	CFormatParameter(){}
public:
	CFormatParameter(int paraNum);
	~CFormatParameter();

	icu::Formattable* GetMsgArgs(){ return m_MsgArgs;} 
	int GetParaNum() { return paraNum; }
	void setDouble(int index, double d);
	void  setLong(int index, int32_t l);
	void  setInt64(int index, int64_t ll) ;
	void  setDate(int index, UDate d) ;
	void  setString(int index, const icu::UnicodeString &stringToCopy);
};

class CResourceBundleManage
{

public:
	CResourceBundleManage() {}
	CResourceBundleManage(const char* configFileName);
	virtual ~CResourceBundleManage(void);

private:
	std::unique_ptr<char[]> m_ResDir{};
	std::unique_ptr<char[]> m_ResLocale{};

	icu::Locale* m_pLocale;			// �����趨

	StringMap mapRes;
	icu::ResourceBundle* m_pResourceBundle;
	UConverter *m_pConverter;
	int m_MaxSize;

	int m_LogFlag;

	FILE* m_LogFile;

	static CResourceBundleManage* _instance;

private:
	UErrorCode ToCodePageString(UConverter *conv, UChar* source, char* target, int destCapacity, int& len);
	bool Init();

public:
	static CResourceBundleManage* Instance(const char* configFileName = NULL);
	const icu::Locale& GetLocale() { return *m_pLocale; }
	const UConverter* GetConverter() { return m_pConverter; }

	int GetSize(void);			// ȡ����Դ����
	
	void Release(void);

	const char* LoadResString(const char* key);
	icu::UnicodeString LoadUResString(const char* key);

	int Format(const char* key, CFormatParameter& parameter, char buffer[]);
	int FormatString(const char* formatStr, CFormatParameter& parameter, char buffer[]);
};

/**
 * @class CBreakLine
 * @author Lark.Li
 * @brief ���ַ������ж��д���
*/
class CBreakLine
{
public:
	/**
	* @brief ����
	* @param[in]  const UnicodeString&  Ҫ�������ַ���
	* @param[in]  const Locale&  �趨������
	* @param[in]  int32_t ÿ�е��ַ���
	* @param[out]  int32_t[] ÿ�е���ԭ���е���ʼλ��
	* @param[out]  int32_t[] ÿ�е���ԭ���е���ֹλ��
	* @param[in]  int32_t �������
	* @return int ʵ�ʵ�����
	*/
	static int32_t WrapParagraph(const icu::UnicodeString& s, const icu::Locale& locale, int32_t maxCharsPreLine, int32_t lineStarts[], int32_t lineEnds[], int32_t maxLines);
};
