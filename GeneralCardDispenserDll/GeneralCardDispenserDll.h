// GeneralCardDispenserDll.h : GeneralCardDispenserDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <string>
#include <vector>
#include "RetStatus.h"
#include "../SSCReaderFuncs/SSCReaderFuncsDefine.h"

using namespace std;

// CGeneralCardDispenserDllApp
// �йش���ʵ�ֵ���Ϣ������� GeneralCardDispenserDll.cpp
//

class CGeneralCardDispenserDllApp : public CWinApp
{
public:
	CGeneralCardDispenserDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
private:
	int initLog();
};

// ���쿨ģ��
int WINAPI GCDOpen(RetStatus *stat);

// �ر��쿨ģ��
int WINAPI GCDClose(RetStatus *stat);

// �����䵽С��
int WINAPI GCDInboxToCar(RetStatus *stat);

// С��������
int WINAPI GCDCarToRepo(int &posit, RetStatus *stat);

// ���⵽С��
int WINAPI GCDRepoToCar(int &posit, RetStatus *stat);

// С����������
int WINAPI GCDCarToOutbox(int &boxnum, RetStatus *stat);

// ��������С��
int WINAPI GCDReaderToCar(int &readerId, RetStatus *stat);

// С����������
int WINAPI GCDCarToReader(int &readerId, RetStatus *stat);

// С����֤����ӡ��
int WINAPI GCDCarToCardPrinter(int &port, RetStatus *stat);

// С����֤����ӡ��
int WINAPI GCDCardPrinterToCar(int &port, RetStatus *stat);

// С����OCR
int WINAPI GCDCarToOCR(RetStatus *stat);

// OCR��С��
int WINAPI GCDOCRToCar(RetStatus *stat);

// С����͹ӡ��
int WINAPI GCDCarToEmboss(RetStatus *stat);

// ͹ӡ����С��
int WINAPI GCDEmbossToCar(RetStatus *stat);

// ����С��״̬
int WINAPI GCDCarStatus(stCardStatus &cs, RetStatus *stat);

// ������״̬
int WINAPI GCDInboxStatus(stInboxStatus is, RetStatus *stat);

// ����״̬
int WINAPI GCDRepoStatus(stRepoStatus &reps, RetStatus *stat);

// ������״̬
int WINAPI GCDOutboxStatus(stOutboxStatus &os, RetStatus *stat);

// ֤����ӡ��״̬
int WINAPI GCDCardPrinterStatus(RetStatus *stat);

// ������״̬
int WINAPI GCDReaderStatus(int id, stReaderStatus &rs, RetStatus *stat);

// ocr״̬
int WINAPI GCDOCRStatus(RetStatus *stat);

// ͹ӡ��״̬
int WINAPI GCDEmbossStatus(RetStatus *stat);

// ��λ
int WINAPI GCDReset(int type, RetStatus *stat);

// ���籣��Ϣ
int WINAPI GCDReaderReadSSInfo(int zone, int readerId, SSCBaseInfo &info, RetStatus *stat);

// �����п���
int WINAPI GCDReaderReadBKNum(int readerId, CString& bknum, RetStatus *stat);

// ��ʹ�ÿ�������
int WINAPI GCDRepoUsedNum(int &type, RetStatus *stat);

// ������ʹ�õĿ���Ŀ��ۺ�
int WINAPI GCDRepoUsedSlots(vector<UINT16> &slots, RetStatus *stat);

// ����������
int WINAPI GCDReaderCardIn(int readerId, int timeout, RetStatus *stat);

// ����������
int WINAPI GCDReaderCardOut(int readerId, int timeout, RetStatus *stat);

