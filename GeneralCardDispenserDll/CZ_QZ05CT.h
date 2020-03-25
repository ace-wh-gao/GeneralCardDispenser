#pragma once
#include "BaseCardDispenser.h"
#include <map>
#include "../SSCReaderFuncs/SSCReaderFuncsDefine.h"
#include "../SSCReaderFuncs/SSCReaderFuncs.h"

using namespace std;

#define READER_ID_CAR 0				// С��������
#define READER_ID_OUTPORT 1			// �����ڶ�����
#define READER_ID_CARD_PRINTER 2	// ֤��������

class CCZ_QZ05CT :
	public BaseCardDispenser
{
public:
	CCZ_QZ05CT();
	~CCZ_QZ05CT();
	// ͨ�� BaseCardDispenser �̳�
	virtual void Open(RetStatus * stat) override;

	virtual void Close(RetStatus * stat) override;

	virtual void InboxToCar(RetStatus * stat) override;

	virtual void CarToRepo(int &posit, RetStatus * stat) override;

	virtual void RepoToCar(int posit, RetStatus * stat) override;

	virtual void CarToOutbox(int boxnum, RetStatus * stat) override;

	virtual void ReaderToCar(int readerId, RetStatus * stat) override;

	virtual void CarToReader(int readerId, RetStatus * stat) override;

	virtual void CarToCardPrinter(int port, RetStatus * stat) override;

	virtual void CardPrinterToCar(int port, RetStatus * stat) override;

	virtual void CarToOCR(RetStatus * stat) override;

	virtual void OCRToCar(RetStatus * stat) override;

	virtual void CarToEmboss(RetStatus * stat) override;

	virtual void EmbossToCar(RetStatus * stat) override;

	virtual void CarStatus(stCardStatus &cs, RetStatus * stat) override;

	virtual void InboxStatus(stInboxStatus &is, RetStatus * stat) override;

	virtual void RepoStatus(stRepoStatus &reps, RetStatus * stat) override;

	virtual void OutboxStatus(stOutboxStatus &os, RetStatus * stat) override;

	virtual void CardPrinterStatus(RetStatus * stat) override;

	virtual void ReaderStatus(int id, stReaderStatus &rs, RetStatus * stat) override;

	virtual void OCRStatus(RetStatus * stat) override;

	virtual void EmbossStatus(RetStatus * stat) override;

	virtual void Reset(int type, RetStatus * stat) override;

	virtual void RepoUsedNum(int & num, RetStatus * stat) override;

	virtual void RepoUsedSlots(vector<UINT16>& slots, RetStatus * stat) override;

	virtual void ReaderCardIn(int readerId, int timeout, RetStatus * stat) override;

	virtual void ReaderCardOut(int readerId, int timeout, RetStatus * stat) override;

	virtual void ReaderReadSSInfo(int zone, int readerId, SSCBaseInfo & info, RetStatus * stat) override;
	
	virtual void ReaderReadBKNum(int readerId, CString & bknum, RetStatus * stat) override;

private:
	bool mIsOpen;
	int GetSlot(int & blkno, int & slotno);
	int BuildSlotId(int blkno, int slotno);	// ������λ�ù����ɿ���id
	int ParseSlotId(int slotid, int &blkno, int &slotno);
	int GetReader(int readerId, shared_ptr<SSCApduSender> & sender);
	int IsRepoFull(bool &f);
	void ActiveCard(int readerId, RetStatus * stat);

	// ͨ�� BaseCardDispenser �̳�
	virtual void LoadCfg(RetStatus * stat) override;
	// ͨ�� BaseCardDispenser �̳�
	// ������id�����ɿ���λ��
};


