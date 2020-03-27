// SSCReaderFuncs.h : SSCReaderFuncs DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif


#include "resource.h"		// 主符号
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
// 有关此类实现的信息，请参阅 SSCReaderFuncs.cpp
//

class CSSCReaderFuncsApp : public CWinApp
{
public:
	CSSCReaderFuncsApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
private:
	int initLog();
};

// 读卡功能类
class SSC_READER_FUNCS_API SSCReaderFuncs
{
public:
	SSCReaderFuncs(std::shared_ptr<SSCApduSender> sender);
	~SSCReaderFuncs();
	// 读社保基本信息
	int ReadCardBaseInfo(SSCBaseInfo &info);
	// 读银行卡号
	int ReadBanknum(CString &bknum);
private:
	std::shared_ptr<SSCApduSender> mSender;
	int SendApduMustOk(std::string send, std::string resp);
	int SendApdu(std::string send, std::string &resp);
	int SendApduSw(CString send, CString &resp, DWORD &SW1SW2);
};
