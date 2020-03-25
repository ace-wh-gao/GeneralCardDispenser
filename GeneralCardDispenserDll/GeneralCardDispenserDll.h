// GeneralCardDispenserDll.h : GeneralCardDispenserDll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <string>
#include <vector>
#include "RetStatus.h"
#include "../SSCReaderFuncs/SSCReaderFuncsDefine.h"

using namespace std;

// CGeneralCardDispenserDllApp
// 有关此类实现的信息，请参阅 GeneralCardDispenserDll.cpp
//

class CGeneralCardDispenserDllApp : public CWinApp
{
public:
	CGeneralCardDispenserDllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
private:
	int initLog();
};

// 打开领卡模块
int WINAPI GCDOpen(RetStatus *stat);

// 关闭领卡模块
int WINAPI GCDClose(RetStatus *stat);

// 发卡箱到小车
int WINAPI GCDInboxToCar(RetStatus *stat);

// 小车到卡库
int WINAPI GCDCarToRepo(int &posit, RetStatus *stat);

// 卡库到小车
int WINAPI GCDRepoToCar(int &posit, RetStatus *stat);

// 小车到出卡箱
int WINAPI GCDCarToOutbox(int &boxnum, RetStatus *stat);

// 读卡器到小车
int WINAPI GCDReaderToCar(int &readerId, RetStatus *stat);

// 小车到读卡器
int WINAPI GCDCarToReader(int &readerId, RetStatus *stat);

// 小车到证卡打印机
int WINAPI GCDCarToCardPrinter(int &port, RetStatus *stat);

// 小车到证卡打印机
int WINAPI GCDCardPrinterToCar(int &port, RetStatus *stat);

// 小车到OCR
int WINAPI GCDCarToOCR(RetStatus *stat);

// OCR到小车
int WINAPI GCDOCRToCar(RetStatus *stat);

// 小车到凸印机
int WINAPI GCDCarToEmboss(RetStatus *stat);

// 凸印机到小车
int WINAPI GCDEmbossToCar(RetStatus *stat);

// 运输小车状态
int WINAPI GCDCarStatus(stCardStatus &cs, RetStatus *stat);

// 发卡箱状态
int WINAPI GCDInboxStatus(stInboxStatus is, RetStatus *stat);

// 卡库状态
int WINAPI GCDRepoStatus(stRepoStatus &reps, RetStatus *stat);

// 出卡箱状态
int WINAPI GCDOutboxStatus(stOutboxStatus &os, RetStatus *stat);

// 证卡打印机状态
int WINAPI GCDCardPrinterStatus(RetStatus *stat);

// 读卡器状态
int WINAPI GCDReaderStatus(int id, stReaderStatus &rs, RetStatus *stat);

// ocr状态
int WINAPI GCDOCRStatus(RetStatus *stat);

// 凸印机状态
int WINAPI GCDEmbossStatus(RetStatus *stat);

// 复位
int WINAPI GCDReset(int type, RetStatus *stat);

// 读社保信息
int WINAPI GCDReaderReadSSInfo(int zone, int readerId, SSCBaseInfo &info, RetStatus *stat);

// 读银行卡号
int WINAPI GCDReaderReadBKNum(int readerId, CString& bknum, RetStatus *stat);

// 已使用卡库容量
int WINAPI GCDRepoUsedNum(int &type, RetStatus *stat);

// 所有已使用的卡库的卡槽号
int WINAPI GCDRepoUsedSlots(vector<UINT16> &slots, RetStatus *stat);

// 读卡器进卡
int WINAPI GCDReaderCardIn(int readerId, int timeout, RetStatus *stat);

// 读卡器出卡
int WINAPI GCDReaderCardOut(int readerId, int timeout, RetStatus *stat);

