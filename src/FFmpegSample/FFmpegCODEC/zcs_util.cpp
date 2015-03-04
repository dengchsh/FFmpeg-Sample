#include "stdafx.h"
#include "zcs_util.h"


// bool util::writeInit( QString path, QString root_key,QString user_key, QString user_value,QString def_value )
// {
// 	if(path.isEmpty() || user_key.isEmpty())
// 	{
// 		return false;
// 	}
// 	else
// 	{
// 		//���������ļ���������
// 		QSettings *config = new QSettings(path, QSettings::IniFormat);
// 
// 		//����Ϣд�������ļ�
// 		config->beginGroup(root_key);
// 		user_value == ""?def_value:user_value;
// 		config->setValue(user_key, user_value);
// 		config->endGroup();
// 
// 		return true;
// 	} 
// }
// 
// bool util::readInit( QString path, QString root_key,QString user_key, QString &user_value ,QString def_value)
// {
// 	user_value = QString("");
// 	if(path.isEmpty() || user_key.isEmpty())
// 	{
// 		return false;
// 	}
// 	else
// 	{
// 		//���������ļ���������
// 		QSettings *config = new QSettings(path, QSettings::IniFormat);
// 
// 		//��ȡ�û�������Ϣ
// 		user_value = config->value(root_key+"/"+ user_key).toString();
// 		user_value == ""?def_value:user_value;
// 		return true;
// 	}  
// }

void DayBreak::util::UTF_8ToUnicode(wchar_t* pOut,char *pText){
	char* uchar = (char *)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	return;
}

void DayBreak::util::UnicodeToUTF_8(char* pOut,wchar_t* pText){
	// ע�� WCHAR�ߵ��ֵ�˳��,���ֽ���ǰ�����ֽ��ں�z
	char* pchar = (char *)pText;
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
	return;
}

void DayBreak::util::UnicodeToGB2312(char* pOut,wchar_t uData){
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
	return;
}    

void DayBreak::util::Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer){
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return ;
}

void DayBreak::util::GB2312ToUTF_8(std::string& pOut,char *pText, int pLen){
	char buf[4];
	int nLength = pLen* 3;
	char* rst = new char[nLength];

	memset(buf,0,4);
	memset(rst,0,nLength);

	int i = 0;
	int j = 0;     
	while(i < pLen){
		//�����Ӣ��ֱ�Ӹ��ƾ���
		if( *(pText + i) >= 0){
			rst[j++] = pText[i++];
		}else{
			wchar_t pbuffer;
			Gb2312ToUnicode(&pbuffer,pText+i);
			UnicodeToUTF_8(buf,&pbuffer);
			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];   
			j += 3;

			i += 2;

		}

	}

	rst[j] ='\0';
	//���ؽ��
	pOut = rst;            
	delete []rst;    
	return;
}

void DayBreak::util::UTF_8ToGB2312(std::string &pOut, char *pText, int pLen){
	char * newBuf = new char[pLen];
	char Ctemp[4];
	memset(Ctemp,0,4);
	int i =0;
	int j = 0;

	while(i < pLen){

		if(pText > 0){
			newBuf[j++] = pText[i++];                      
		}else{
			WCHAR Wtemp;
			UTF_8ToUnicode(&Wtemp,pText + i);
			UnicodeToGB2312(Ctemp,Wtemp);
			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];

			i += 3;   
			j += 2;  
		}
	}

	newBuf[j] ='\0';
	pOut = newBuf;
	delete []newBuf;
	return;
}

WCHAR* DayBreak::util::AnsiToUnicode( const char* szStr )
{
	int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
	if (nLen == 0)
	{
		return NULL;
	}
	WCHAR* pResult = new wchar_t[nLen];
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
	return pResult;
}

char* DayBreak::util::UnicodeToAnsi( const WCHAR* szStr )
{
	int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
	return pResult;
}

char* DayBreak::util::UnicodeToUTF8( const WCHAR* szStr )
{
	int nLen = WideCharToMultiByte( CP_UTF8, 0, szStr, -1, NULL, 0, NULL, NULL );
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte( CP_UTF8, 0, szStr, -1, pResult, nLen, NULL, NULL );
	return pResult;
}

wchar_t* DayBreak::util::Utf8ToUnicode( const char* utf )
{
	if(!utf || !strlen(utf))  
	{  
		return NULL;  
	}  
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,utf,-1,NULL,0);  
	size_t num = dwUnicodeLen*sizeof(wchar_t);  
	wchar_t *pwText = (wchar_t*)malloc(num);  
	memset(pwText,0,num);  
	MultiByteToWideChar(CP_UTF8,0,utf,-1,pwText,dwUnicodeLen);   
	return pwText;  
}