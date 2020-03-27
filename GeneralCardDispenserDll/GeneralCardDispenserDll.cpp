// GeneralCardDispenserDll.cpp : ���� DLL �ĳ�ʼ�����̡�
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

//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��


 //CGeneralCardDispenserDllApp

BEGIN_MESSAGE_MAP(CGeneralCardDispenserDllApp, CWinApp)
END_MESSAGE_MAP()


// CGeneralCardDispenserDllApp ����

CGeneralCardDispenserDllApp::CGeneralCardDispenserDllApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGeneralCardDispenserDllApp ����

CGeneralCardDispenserDllApp theApp;


// CGeneralCardDispenserDllApp ��ʼ��

BOOL CGeneralCardDispenserDllApp::InitInstance()
{
	CWinApp::InitInstance();
	initLog();
	return TRUE;
}

// ��ʼ��log
int CGeneralCardDispenserDllApp::initLog()
{
	#define	LOG_TAG "GCDDLL"	// ģ���ʶ
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
	// ���ģ��tag
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
		LOG(ERROR) << "�ӿڵ��ô��󣬲�������Ϊ��ָ��";
		return -1;
	}
	if (gCardDispenser == nullptr)
	{
		LOG(ERROR) << "�豸ʵ��δ����";
		stat->code = -1;
		stat->level = (int)EL_SOFT;
		stat->msg = "��ʼ���豸ʧ��";
		return -1;
	}
	return 0;
}

// ���豸
int WINAPI GCDOpen(RetStatus *stat)
{
	if (stat == NULL)
	{
		LOG(ERROR) << "GCDOpen�ӿڵ��ô���,��������Ϊ��ָ��";
		return -1;
	}
	// �����豸ʵ��
	CardDispFactory cdf;
	int ret = cdf.CreateDev(gCardDispenser);
	if (ret < 0)
	{
		LOG(ERROR) << "�����豸��ʵ��ʧ��";
		stat->code = ret;
		stat->level = (int)EL_SOFT;
		stat->msg = "��ʼ���豸ʧ��";
		return ret;
	}
	// ���豸
	if (gCardDispenser == nullptr)
	{
		LOG(ERROR) << "�����豸��ʵ��ʧ��";
		stat->code = -1;
		stat->level = (int)EL_SOFT;
		stat->msg = "�����豸��ʵ��ʧ��";
		return ret;
	}
	gCardDispenser->Open(stat);
	return stat->code;
}

// �ر��豸
int WINAPI GCDClose(RetStatus * stat)
{
	CHECK_INIT()
	gCardDispenser->Close(stat);
	return stat->code;
}

// �����䵽С��
int WINAPI GCDInboxToCar(RetStatus * stat)
{
	CHECK_INIT()
	gCardDispenser->InboxToCar(stat);
	return stat->code;
}

// С��������
int WINAPI GCDCarToRepo(int &posit, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToRepo(posit, stat);
	if (stat->code == 0)
	{
		LOG(INFO) << "���ɹ�,id: " << posit;
	}
	return stat->code;
}

// ���⵽С��
int WINAPI GCDRepoToCar(int & posit, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->RepoToCar(posit, stat);
	if (stat->code == 0)
	{
		LOG(INFO) << "����ɹ�,id: " << posit;
	}
	return stat->code;
}

// С����������
int WINAPI GCDCarToOutbox(int & boxnum, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToOutbox(boxnum, stat);
	return stat->code;
}

// ��������С��
int WINAPI GCDReaderToCar(int & readerId, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderToCar(readerId, stat);
	return stat->code;
}

// С����������
int WINAPI GCDCarToReader(int & readerId, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToReader(readerId, stat);
	return stat->code;
}

// С����֤����ӡ��
int WINAPI GCDCarToCardPrinter(int & port, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToCardPrinter(port, stat);
	return stat->code;
}

// С����֤����ӡ��
int WINAPI GCDCardPrinterToCar(int & port, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CardPrinterToCar(port, stat);
	return stat->code;
}

// С����OCR
int WINAPI GCDCarToOCR(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToOCR(stat);
	return stat->code;
}

// OCR��С��
int WINAPI GCDOCRToCar(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->OCRToCar(stat);
	return stat->code;
}

// С����͹ӡ��
int WINAPI GCDCarToEmboss(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarToEmboss(stat);
	return stat->code;
}

// ͹ӡ����С��
int WINAPI GCDEmbossToCar(RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->EmbossToCar(stat);
	return stat->code;
}

// ����С��״̬
int WINAPI GCDCarStatus(stCardStatus &cs, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->CarStatus(cs, stat);
	return stat->code;
}

// ������״̬
int WINAPI GCDInboxStatus(stInboxStatus is, RetStatus *stat) 
{
	CHECK_INIT();
	gCardDispenser->InboxStatus(is, stat);
	return stat->code;
}

// ����״̬
int WINAPI GCDRepoStatus(stRepoStatus &reps, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->RepoStatus(reps, stat);
	return stat->code;
}

// ������״̬
int WINAPI GCDOutboxStatus(stOutboxStatus &os, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->OutboxStatus(os, stat);
	return stat->code;
}

// ֤����ӡ��״̬
int WINAPI GCDCardPrinterStatus(RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->CardPrinterStatus(stat);
	return stat->code;
}

// ������״̬
int WINAPI GCDReaderStatus(int id, stReaderStatus &rs, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderStatus(id, rs, stat);
	return stat->code;
}

// ocr״̬
int WINAPI GCDOCRStatus(RetStatus *stat) 
{
	CHECK_INIT();
	gCardDispenser->OCRStatus(stat);
	return stat->code;
}

// ͹ӡ��״̬
int WINAPI GCDEmbossStatus(RetStatus *stat) {
	CHECK_INIT();
	gCardDispenser->EmbossStatus(stat);
	return stat->code;
}

// ��λ
int WINAPI GCDReset(int type, RetStatus * stat)
{
	CHECK_INIT();
	gCardDispenser->Reset(type, stat);
	return stat->code;
}

// ���籣��Ϣ
int WINAPI GCDReaderReadSSInfo(int zone, int readerId, SSCBaseInfo &info, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderReadSSInfo(zone, readerId, info, stat);
	return stat->code;
}

// �����п���
int WINAPI GCDReaderReadBKNum(int readerId, CString &bknum, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderReadBKNum(readerId, bknum, stat);
	return stat->code;
}

// ��ʹ�ÿ�������
int WINAPI GCDRepoUsedNum(int &num, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->RepoUsedNum(num, stat);
	return stat->code;
}

// ������ʹ�õĿ���Ŀ��ۺ�
int WINAPI GCDRepoUsedSlots(vector<UINT16> &slots, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->RepoUsedSlots(slots, stat);
	return stat->code;
}

// ����������
int WINAPI GCDReaderCardIn(int readerId, int timeout, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderCardIn(readerId, timeout, stat);
	return stat->code;
}

// ����������
int WINAPI GCDReaderCardOut(int readerId, int timeout, RetStatus *stat)
{
	CHECK_INIT();
	gCardDispenser->ReaderCardOut(readerId, timeout, stat);
	return stat->code;
}

