#pragma once
#include "stdafx.h"
#include "RetStatus.h"
#include <vector>
#include "../SSCReaderFuncs/SSCReaderFuncsDefine.h"

using namespace std;
class BaseCardDispenser
{
public:
	virtual ~BaseCardDispenser();
	// 打开领卡模块
	virtual void Open(RetStatus *stat) = 0;

	// 关闭领卡模块
	virtual void Close(RetStatus *stat) = 0;

	// 发卡箱到小车
	virtual void InboxToCar(RetStatus *stat) = 0;

	// 小车到卡库
	virtual void CarToRepo(int &posit, RetStatus *stat) = 0;

	// 卡库到小车
	virtual void RepoToCar(int posit, RetStatus *stat) = 0;

	// 小车到出卡箱
	virtual void CarToOutbox(int boxnum, RetStatus *stat) = 0;

	// 读卡器到小车
	virtual void ReaderToCar(int readerId, RetStatus *stat) = 0;

	// 小车到读卡器
	virtual void CarToReader(int readerId, RetStatus *stat) = 0;

	// 小车到证卡打印机
	virtual void CarToCardPrinter(int port, RetStatus *stat) = 0;

	// 小车到证卡打印机
	virtual void CardPrinterToCar(int port, RetStatus *stat) = 0;

	// 小车到OCR
	virtual void CarToOCR(RetStatus *stat) = 0;

	// OCR到小车
	virtual void OCRToCar(RetStatus *stat) = 0;

	// 小车到凸印机
	virtual void CarToEmboss(RetStatus *stat) = 0;

	// 凸印机到小车
	virtual void EmbossToCar(RetStatus *stat) = 0;

	// 运输小车状态
	virtual void CarStatus(stCardStatus &cs, RetStatus *stat) = 0;

	// 发卡箱状态
	virtual void InboxStatus(stInboxStatus &is, RetStatus *stat) = 0;

	// 卡库状态
	virtual void RepoStatus(stRepoStatus &reps, RetStatus *stat) = 0;

	// 出卡箱状态
	virtual void OutboxStatus(stOutboxStatus &os, RetStatus *stat) = 0;

	// 证卡打印机状态
	virtual void CardPrinterStatus(RetStatus *stat) = 0;

	// 读卡器状态
	virtual void ReaderStatus(int id, stReaderStatus &rs, RetStatus *stat) = 0;

	// ocr状态
	virtual void OCRStatus(RetStatus *stat) = 0;

	// 凸印机状态
	virtual void EmbossStatus(RetStatus *stat) = 0;

	// 复位
	virtual void Reset(int type, RetStatus *stat) = 0;

	//// 读社保信息
	virtual void ReaderReadSSInfo(int zone, int readerId, SSCBaseInfo &info, RetStatus *stat) = 0;

	//// 读银行卡号
	virtual void ReaderReadBKNum(int readerId, CString& bknum, RetStatus *stat) = 0;

	// 已使用卡库容量
	virtual void RepoUsedNum(int &type, RetStatus *stat) = 0;

	// 所有已使用的卡库的卡槽号
	virtual void RepoUsedSlots(vector<UINT16> &slots, RetStatus *stat) = 0;

	// 读卡器进卡
	virtual void ReaderCardIn(int readerId, int timeout, RetStatus *stat) = 0;

	// 读卡器出卡
	virtual void ReaderCardOut(int readerId, int timeout, RetStatus *stat) = 0;

	// 加载配置
	virtual void LoadCfg(RetStatus *stat) = 0;
};

