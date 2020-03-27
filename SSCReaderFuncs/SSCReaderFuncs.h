// SSCReaderFuncs.h : SSCReaderFuncs DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif


#include "resource.h"		// ������
#include <memory>
#include <string>
#include "SSCReaderFuncsDefine.h"

using namespace std;

#ifdef SSC_READER_FUNCS_EXPORT
#define SSC_READER_FUNCS_API __declspec(dllexport)
#else
#define SSC_READER_FUNCS_API __declspec(dllimport)
#endif

// CSSCReaderFuncsApp
// �йش���ʵ�ֵ���Ϣ������� SSCReaderFuncs.cpp
//

class CSSCReaderFuncsApp : public CWinApp
{
public:
	CSSCReaderFuncsApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
private:
	int initLog();
};

// ����������
class SSC_READER_FUNCS_API SSCReaderFuncs
{
public:
	SSCReaderFuncs(std::shared_ptr<SSCApduSender> sender);
	~SSCReaderFuncs();
	// ���籣������Ϣ
	int ReadCardBaseInfo(SSCBaseInfo &info);
	// �����п���
	int ReadBanknum(CString &bknum);
private:
	std::shared_ptr<SSCApduSender> mSender;
	int SendApduMustOk(std::string send, std::string resp);
	int SendApdu(std::string send, std::string &resp);
	int SendApduSw(CString send, CString &resp, DWORD &SW1SW2);
};
