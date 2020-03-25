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
	// ���쿨ģ��
	virtual void Open(RetStatus *stat) = 0;

	// �ر��쿨ģ��
	virtual void Close(RetStatus *stat) = 0;

	// �����䵽С��
	virtual void InboxToCar(RetStatus *stat) = 0;

	// С��������
	virtual void CarToRepo(int &posit, RetStatus *stat) = 0;

	// ���⵽С��
	virtual void RepoToCar(int posit, RetStatus *stat) = 0;

	// С����������
	virtual void CarToOutbox(int boxnum, RetStatus *stat) = 0;

	// ��������С��
	virtual void ReaderToCar(int readerId, RetStatus *stat) = 0;

	// С����������
	virtual void CarToReader(int readerId, RetStatus *stat) = 0;

	// С����֤����ӡ��
	virtual void CarToCardPrinter(int port, RetStatus *stat) = 0;

	// С����֤����ӡ��
	virtual void CardPrinterToCar(int port, RetStatus *stat) = 0;

	// С����OCR
	virtual void CarToOCR(RetStatus *stat) = 0;

	// OCR��С��
	virtual void OCRToCar(RetStatus *stat) = 0;

	// С����͹ӡ��
	virtual void CarToEmboss(RetStatus *stat) = 0;

	// ͹ӡ����С��
	virtual void EmbossToCar(RetStatus *stat) = 0;

	// ����С��״̬
	virtual void CarStatus(stCardStatus &cs, RetStatus *stat) = 0;

	// ������״̬
	virtual void InboxStatus(stInboxStatus &is, RetStatus *stat) = 0;

	// ����״̬
	virtual void RepoStatus(stRepoStatus &reps, RetStatus *stat) = 0;

	// ������״̬
	virtual void OutboxStatus(stOutboxStatus &os, RetStatus *stat) = 0;

	// ֤����ӡ��״̬
	virtual void CardPrinterStatus(RetStatus *stat) = 0;

	// ������״̬
	virtual void ReaderStatus(int id, stReaderStatus &rs, RetStatus *stat) = 0;

	// ocr״̬
	virtual void OCRStatus(RetStatus *stat) = 0;

	// ͹ӡ��״̬
	virtual void EmbossStatus(RetStatus *stat) = 0;

	// ��λ
	virtual void Reset(int type, RetStatus *stat) = 0;

	//// ���籣��Ϣ
	virtual void ReaderReadSSInfo(int zone, int readerId, SSCBaseInfo &info, RetStatus *stat) = 0;

	//// �����п���
	virtual void ReaderReadBKNum(int readerId, CString& bknum, RetStatus *stat) = 0;

	// ��ʹ�ÿ�������
	virtual void RepoUsedNum(int &type, RetStatus *stat) = 0;

	// ������ʹ�õĿ���Ŀ��ۺ�
	virtual void RepoUsedSlots(vector<UINT16> &slots, RetStatus *stat) = 0;

	// ����������
	virtual void ReaderCardIn(int readerId, int timeout, RetStatus *stat) = 0;

	// ����������
	virtual void ReaderCardOut(int readerId, int timeout, RetStatus *stat) = 0;

	// ��������
	virtual void LoadCfg(RetStatus *stat) = 0;
};

