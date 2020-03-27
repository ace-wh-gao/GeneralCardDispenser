// GeneralCardDispenserDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "GeneralCardDispenserDll.h"
#include <memory>
#include "easylogging++.h"
#include "BaseCardDispenser.h"
#include "CardDispFactory.h"
#include "devcfg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

INITIALIZE_EASYLOGGINGPP;

shared_ptr<BaseCardDispenser> gCardDispenser;

#define CHECK_INIT() 	if (checkInit(stat) != 0)\
{\
	return stat->code;\
}

//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。


 //CGeneralCardDispenserDllApp

BEGIN_MESSAGE_MAP(CGeneralCardDispenserDllApp, CWinApp)
END_MESSAGE_MAP()


// CGeneralCardDispenserDllApp 构造

CGeneralCardDispenserDllApp::CGeneralCardDispenserDllApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CGeneralCardDispenserDllApp 对象

CGeneralCardDispenserDllApp theApp;


// CGeneralCardDispenserDllApp 初始化

BOOL CGeneralCardDispenserDllApp::InitInstance()
{
	CWinApp::InitInstance();
	initLog();
	return TRUE;
}

// 初始化log
int CGeneralCardDispenserDllApp::initLog()
{
	#define	LOG_TAG "GCDDLL"	// 模块标识
	USES_CONVERSION;
	devcfg cfg;
	CString pathCtr= cfg.LogCfgPath();
	if (pathCtr.GetLength() == 0)
	{
		return -1;
	}
	string s=string(T2A(pathCtr));
	el::Configurations conf(s);
	el::Configuration* cf = conf.get(el::Level::Global, el::ConfigurationType::Format);
	// 添加模块tag
	string newVal = cf->value();
	int inde = newVal.find_first_of(' ');
	string add = LOG_TAG;
	add = " " + add + " ";
	newVal.replace(inde, 1, add);
	conf.set(el::Level::Global, el::ConfigurationType::Format, newVal);
	el::Loggers::reconfigureAllLoggers(conf);
	return 0;
}

int checkInit(RetStatus *stat)
{
	if (stat == NULL)
	{
		LOG(ERROR) << "接口调用错误，参数不能为空指针";
		return -1;
	}
	if (gCardDispenser == nullptr)
	{
		LOG(ERROR) << "设备实例未创建";
		stat->code = -1;
		stat->level = (int)EL_SOFT;
		stat->msg = "初始化设备失败";
		return -1;
	}
	return 0;
}

// 打开设备
int WINAPI GCDOpen(RetStatus *stat)
{
	if (stat == NULL)
	{
		LOG(ERROR) << "GCDOpen接口调用错误,参数不能为空指针";
		return -1;
	}
	// 创建设备实例
	CardDispFactory cdf;
	int ret = cdf.CreateDev(gCardDispenser);
	if (ret < 0)
	{
		LOG(ERROR) << "创建设备类实例失败";
		stat->code = ret;
		stat->level = (int)EL_SOFT;
		stat->msg = "初始化设备失败";
		return ret;
	}
	// 打开设备
	if (gCardDispenser == nullptr)
	{
		LOG(ERROR) << "创建设备类实例失败";
		stat->code = -1;
		stat->level = (int)EL_SOFT;
		stat->msg = "创建设备类实例失败";
		return ret;
	}
	gCardDispenser->Open(stat);
	return stat->code;
}

// 关闭设备
int WINAPI GCDClose(RetStatus * stat)
{
	CHECK_INIT()
	gCardDispenser->Close(stat);
	return stat->code;
}

// 发卡箱到小车
int WINAPI GCDInboxToCar(RetStatus * stat)
{
	CHECK_INIT()
	gCardDispenser->InboxToCar(stat);
	return stat->code;
}

// 小车到卡库
int WINAPI GCDCarToRepo(int &posit, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToRepo(posit, stat);
	if (stat->code == 0)
	{
		LOG(INFO) << "入库成功,id: " << posit;
	}
	return stat->code;
}

// 卡库到小车
int WINAPI GCDRepoToCar(int & posit, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->RepoToCar(posit, stat);
	if (stat->code == 0)
	{
		LOG(INFO) << "出库成功,id: " << posit;
	}
	return stat->code;
}

// 小车到出卡箱
int WINAPI GCDCarToOutbox(int & boxnum, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToOutbox(boxnum, stat);
	return stat->code;
}

// 读卡器到小车
int WINAPI GCDReaderToCar(int & readerId, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderToCar(readerId, stat);
	return stat->code;
}

// 小车到读卡器
int WINAPI GCDCarToReader(int & readerId, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToReader(readerId, stat);
	return stat->code;
}

// 小车到证卡打印机
int WINAPI GCDCarToCardPrinter(int & port, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToCardPrinter(port, stat);
	return stat->code;
}

// 小车到证卡打印机
int WINAPI GCDCardPrinterToCar(int & port, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CardPrinterToCar(port, stat);
	return stat->code;
}

// 小车到OCR
int WINAPI GCDCarToOCR(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToOCR(stat);
	return stat->code;
}

// OCR到小车
int WINAPI GCDOCRToCar(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->OCRToCar(stat);
	return stat->code;
}

// 小车到凸印机
int WINAPI GCDCarToEmboss(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToEmboss(stat);
	return stat->code;
}

// 凸印机到小车
int WINAPI GCDEmbossToCar(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->EmbossToCar(stat);
	return stat->code;
}

// 运输小车状态
int WINAPI GCDCarStatus(stCardStatus &cs, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarStatus(cs, stat);
	return stat->code;
}

// 发卡箱状态
int WINAPI GCDInboxStatus(stInboxStatus is, RetStatus *stat) 
{
	CHECK_INIT();
	gCardDispenser->InboxStatus(is, stat);
	return stat->code;
}

// 卡库状态
int WINAPI GCDRepoStatus(stRepoStatus &reps, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->RepoStatus(reps, stat);
	return stat->code;
}

// 出卡箱状态
int WINAPI GCDOutboxStatus(stOutboxStatus &os, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->OutboxStatus(os, stat);
	return stat->code;
}

// 证卡打印机状态
int WINAPI GCDCardPrinterStatus(RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->CardPrinterStatus(stat);
	return stat->code;
}

// 读卡器状态
int WINAPI GCDReaderStatus(int id, stReaderStatus &rs, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderStatus(id, rs, stat);
	return stat->code;
}

// ocr状态
int WINAPI GCDOCRStatus(RetStatus *stat) 
{
	CHECK_INIT();
	gCardDispenser->OCRStatus(stat);
	return stat->code;
}

// 凸印机状态
int WINAPI GCDEmbossStatus(RetStatus *stat) {
	CHECK_INIT();
	gCardDispenser->EmbossStatus(stat);
	return stat->code;
}

// 复位
int WINAPI GCDReset(int type, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->Reset(type, stat);
	return stat->code;
}

// 读社保信息
int WINAPI GCDReaderReadSSInfo(int zone, int readerId, SSCBaseInfo &info, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderReadSSInfo(zone, readerId, info, stat);
	return stat->code;
}

// 读银行卡号
int WINAPI GCDReaderReadBKNum(int readerId, CString &bknum, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderReadBKNum(readerId, bknum, stat);
	return stat->code;
}

// 已使用卡库容量
int WINAPI GCDRepoUsedNum(int &num, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->RepoUsedNum(num, stat);
	return stat->code;
}

// 所有已使用的卡库的卡槽号
int WINAPI GCDRepoUsedSlots(vector<UINT16> &slots, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->RepoUsedSlots(slots, stat);
	return stat->code;
}

// 读卡器进卡
int WINAPI GCDReaderCardIn(int readerId, int timeout, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderCardIn(readerId, timeout, stat);
	return stat->code;
}

// 读卡器出卡
int WINAPI GCDReaderCardOut(int readerId, int timeout, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderCardOut(readerId, timeout, stat);
	return stat->code;
}

