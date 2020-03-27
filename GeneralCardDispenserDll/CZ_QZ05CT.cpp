#include "stdafx.h"
#include "CZ_QZ05CT.h"
#include "CRT_591_QZ05_CT3.h"
#include "easylogging++.h"
#include "DeviceDefine.h"
#include <map>
#include "RetStatus.h"
#include "../SSCReaderFuncs/SSCReaderFuncs.h"
#include "errcode.h"
#include "hexbin.h"

using namespace std;

// ���ߴ�ӡ����С���ĳ�ʱʱ��
#define TIMEOUT_PRINTER_TO_CAR_HD6600 480000

static map<int, string > kErrMsg =
{
	{ ERR_NONE, "����" },
	{ ERR_NO_ENCODER, "����ģ�鲻����" },
	{ ERR_NO_CARD_PRINTER, "ƽӡ��������" },
	{ ERR_NO_SLIDER, "������������" },
	{ ERR_NO_STORAGE, "���ⲻ����" },
	{ ERR_NO_CAR_READER, "С��������������" },
	{ EER_NO_RW_READER, "��д������������" },
	{ EER_NO_PASS_READER, "ͨ��������������" },
	{ EER_NO_EMBOSS, "͹��ģ�鲻����" },
	{ EER_NO_HOT_MODULE, "����ģ�鲻����" },
	{ ERR_DEVICE_NOT_OPEN, "�豸δ��" },
	{ ERR_COMMUNICATION, "ģ��ͨѶ��" },
	{ ERR_NEGATIVE_RESP, "ģ��ִ�д�" },
	{ ERR_NOT_PUT_CARD_BOX, "��ѡ������δ����" },
	{ ERR_CARD_BOX_EMPTY, "��ѡ�������޿�" },
	{ ERR_TARGET_NO_CARD, "Ŀ��λ���޿�" },
	{ ERR_TARGET_HAVE_CARD, "Ŀ��λ���п�" },
	{ ERR_CARD_AT_EXIT, "�������п�" },
	{ ERR_PARAMETER_WRONG, "������" },
	{ ERR_LOAD_CONFIG_WRONG, "��������ʧ��" },
	{ ERR_INTERNAL_ERR, "�ڲ������쳣" },
	{ ERR_NOT_SUPPORT, "���ܻ�����ݲ�֧��" },
	{ ERR_MODULE_NO_INIT, "ģ��δ��ʼ��" },
	{ ERR_CARD_NOT_ACTIVE, "��Ƭδ�ϵ缤��" },
	{ EER_NO_BIN_BOX, "�����������߲�����" },
	{ EER_NO_WAIT_TIME, "ִ�г�ʱ" },
	{ EER_NO_SCAN_MODULE, "ɨ����ģ�鲻����" },
	{ EER_SCAN_JAM, "ɨ����ģ��п�" },
	{ ERR_OTHER, "������" },
};

// ��������Ч����
const int kBlkCapacity = 250;
// �������б�ŷ�Χ
const int kMinBlkNum = 1;
const int kMaxBlkNum = 2;

#define INBOX_NUM 6		// ����������
#define OUTBOX_NUM 2	// ����������

// ��ȡ���Ҵ�����Ϣ
string getErrMsg(int code)
{
	int c = kErrMsg.count(code);
	if (c == 1)
	{
		return kErrMsg[code];
	}
	return kErrMsg[ERR_OTHER];
}

CCZ_QZ05CT::CCZ_QZ05CT()
{
	mIsOpen = false;
}


CCZ_QZ05CT::~CCZ_QZ05CT()
{
}

void CCZ_QZ05CT::Open(RetStatus * stat)
{
	ASSERT(stat != nullptr);

	// TODO ��һ�δ�ʱ���������ļ�
	// 
	LOG(INFO) << "���������ļ����豸";
	int ret = OpenDevices(1);
	if (ret > 0)
	{
		LOG(WARNING) << "������: " << ret << ".������Ϣ: " << getErrMsg(ret);
		LOG(WARNING) << "���豸ʧ��";
		LOG(INFO) << "ɨ��Ӳ�����豸";
		ret = OpenDevices(0);
		if (ret > 0)
		{
			LOG(ERROR) << "������: " << ret << ".������Ϣ: " << getErrMsg(ret);
			LOG(WARNING) << "���豸ʧ��";
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return ;
		}
	}
	mIsOpen = true;
	*stat = RS_OK;
	return ;
}

void CCZ_QZ05CT::Close(RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = CloseDevices();
	if (ret > 0)
	{
		LOG(ERROR) << "������: " << ret << ".������Ϣ: " << getErrMsg(ret);
		LOG(WARNING) << "�ر��豸ʧ��";
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	mIsOpen = false;
	*stat = RS_OK;
	return;
}

// �����䵽С��
// ֻҪ�з���һ������Ͳ��ᱨ����δ���ô���
void CCZ_QZ05CT::InboxToCar(RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return ;
	}
	int ret2 = 0;
	int ret = 0;
	int boxNo;
	/*������
	boxNo��[in] ����ţ�ȡֵ��Χ1~6*/
	for (boxNo = 1; boxNo <= 6; boxNo++)
	{
		//ѭ����ֱ���޿�Ϊֹ�������䷢��
		ret2 = MoveCardToSlider(boxNo);
		if (ret2 == 0)
		{
			*stat = RS_OK;
			return ;
		}
		else if (ERR_NOT_PUT_CARD_BOX == ret2)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = "δ���÷�����";
		}
		else if (ERR_CARD_BOX_EMPTY == ret2)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = "�������޿�";
		}
		else {
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = "������δ֪����";
			return;
		}
	}
}

//��ѯ�����еĿտ���
// -1 δ�ҵ��տ���
// >0 ���Ҵ�����
int CCZ_QZ05CT::GetSlot(int & blkno, int & slotno)
{
	int ret = 0;
	BYTE slots[256] = { 0 };
	int len = 0;
	int bn = 0;
	int sn = 0;
	BOOL end = FALSE;
	for (int i = 1; i <= 2; i++)
	{
		ret = GetStorageSlots(i, slots, &len);
		if (ret != 0)
		{
			return ret;
		}
		for (int j = 0; j < len; j++)
		{
			if (slots[j] == 0)
			{
				sn = j + 1;
				bn = i;
				end = TRUE;
				break;
			}
		}
		if (end) {
			break;
		}
	}
	if (0 == bn) {
		return -1;
	}
	blkno = bn;
	slotno = sn;
	return 0;
}

// �������ۺű��
int CCZ_QZ05CT::BuildSlotId(int blkno, int slotno)
{
	return blkno*kBlkCapacity + slotno;
}

int CCZ_QZ05CT::ParseSlotId(int slotid, int & blkno, int & slotno)
{
	if (slotid < 0)
	{
		return -1;
	}
	blkno = slotid / kBlkCapacity;
	slotno = slotid % kBlkCapacity;
	// ��ŵ���Ч��Χ
	if (blkno > kMaxBlkNum || blkno < kMinBlkNum)
	{
		return -1;
	}
	return 0;
}

// TODO ��Ҫ���ַǽӺͽӴ���ǰ������������ƶ����ӿ�
// ����ֵ ��= 0 �ɹ���
// > 0 Ӳ�����ô���
// < 0 �߼�����
int CCZ_QZ05CT::GetReader(int readerId, shared_ptr<SSCApduSender>& sender)
{
	int ret = 0;
	switch (readerId)
	{
	case READER_ID_CAR:	// С��������
		sender->isContactless = true;
		sender->apdu = [](CString send, CString &resp, DWORD &SW1SW2)->int {
			BYTE s[1024] = { 0 };
			BYTE r[1024] = { 0 };
			int slen = 0;
			int rlen = 0;
			HexBin hb;
			int ret = hb.Decode(send, s, slen);
			if (ret < 0)
			{
				return -1;
			}
			ret = RFCardSendAPDU310_004(0, s, slen, r, &rlen);
			if (ret != 0)
			{
				return ERR_AS_TRANS;
			}
			if (rlen == 2)
			{
				resp = _T("");
			}
			else
			{
				hb.Encode(r, rlen - 2, resp);
				
			}
			SW1SW2 = r[0] << 8 + r[1];
			return 0;
		};
		break;
	case READER_ID_OUTPORT: // ���ڶ�����
		sender->isContactless = false;
		sender->apdu = [](CString send, CString &resp, DWORD &SW1SW2)->int {
			BYTE s[1024] = { 0 };
			BYTE r[1024] = { 0 };
			int slen = 0;
			int rlen = 0;
			HexBin hb;
			int ret = hb.Decode(send, s, slen);
			if (ret < 0)
			{
				return -1;
			}
			ret = ICCardSendAPDU310_004(1, 0, s, slen, r, &rlen);
			if (ret != 0)
			{
				return ERR_AS_TRANS;
			}
			if (rlen == 2)
			{
				resp = _T("");
			}
			else
			{
				hb.Encode(r, rlen - 2, resp);

			}
			SW1SW2 = r[0] << 8 + r[1];
			return 0;
		};
		break;
	case READER_ID_CARD_PRINTER: // ֤��������
		sender->isContactless = false;
		sender->apdu = [](CString send, CString &resp, DWORD &SW1SW2)->int {
			BYTE s[1024] = { 0 };
			BYTE r[1024] = { 0 };
			int slen = 0;
			int rlen = 0;
			HexBin hb;
			int ret = hb.Decode(send, s, slen);
			if (ret < 0)
			{
				return -1;
			}
			ret = RWICSendAPDU(s, slen, r, &rlen);
			if (ret != 0)
			{
				return ERR_AS_TRANS;
			}
			if (rlen == 2)
			{
				resp = _T("");
			}
			else
			{
				hb.Encode(r, rlen - 2, resp);

			}
			SW1SW2 = r[0] << 8 + r[1];
			return 0;
		};
		break;
	default:
		LOG(ERROR) << "��֧�ֵĶ�����id.id=" << readerId;
		ret = -1;
		break;
	}
	return ret;
}

// �����Ƿ���
int CCZ_QZ05CT::IsRepoFull(bool &f)
{
	int ret = 0;
	BYTE slots[512] = { 0 };
	int len = 0;
	f = true;
	for (int diskno = 1; diskno <= 2; diskno++)
	{
		ret = GetStorageSlots(diskno, slots, &len);
		if (ret != 0)
		{
			return ret;
		}
		for (int i = 0; i < len; i++)
		{
			if (slots[i] != 1)
			{
				f = false;
				break;
			}
		}
	}
	return 0;
}

// ����������IC��
void CCZ_QZ05CT::ActiveCard(int readerId, RetStatus * stat)
{
	BYTE atr[128] = { 0 };
	int alen = 0;
	int ret = 0;
	if (readerId == READER_ID_CAR)
	{
		ret = RFCardActive310_004(0, 5, atr, &alen);
	}
	else if (readerId == READER_ID_CARD_PRINTER)
	{
		ret = IcCardPowerOnOff310_004(1, 1);
		if (ret == 0)
		{
			int ictype = 0;
			ret = ICCardReset310_004(1, 1, atr, &alen, &ictype);
		}
	}
	else if (readerId == READER_ID_OUTPORT)
	{
		ret = RWICCardPowerOn(atr, &alen);
	}
	else
	{
		*stat = RS_PARAM_NOT_SUPPORT;
		return;
	}
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "����������IC��ʧ��";
		return;
	}
}

void CCZ_QZ05CT::LoadCfg(RetStatus * stat)
{
	*stat = RS_OK;
}

// ��ȡ�籣������Ϣ
void CCZ_QZ05CT::ReaderReadSSInfo(int zone, int readerId, SSCBaseInfo & info, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	ActiveCard(readerId, stat);
	if (stat->code != 0)
	{
		return;
	}
	shared_ptr<SSCApduSender> sender=make_shared<SSCApduSender>();
	int ret = GetReader(readerId, sender);
	SSCReaderFuncs readerFuns(sender);
	ret = readerFuns.ReadCardBaseInfo(info);
	if (ret == 0)
	{
		*stat = RS_OK;
	}
	else if (ret == ERR_AS_TRANS)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "�������������";
	}
	else {
		*stat = RS_UNKOWN;
	}
}

// ��ȡ���п���Ϣ
// ֻʵ����С��������
void CCZ_QZ05CT::ReaderReadBKNum(int readerId, CString & bknum, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	ActiveCard(readerId, stat);
	if (stat->code != 0)
	{
		return;
	}
	shared_ptr<SSCApduSender> sender = make_shared<SSCApduSender>();
	int ret = GetReader(readerId, sender);
	if (ret != 0)
	{
		*stat = RS_PARAM_NOT_SUPPORT;
		return;
	}
	SSCReaderFuncs readerFuns(sender);
	ret = readerFuns.ReadBanknum(bknum);
	if (ret == 0)
	{
		*stat = RS_OK;
	}
	else if (ret == ERR_AS_TRANS)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "�������������";
	}
	else {
		*stat = RS_UNKOWN;
	}
}

// С��������
// posit ����id
void CCZ_QZ05CT::CarToRepo(int & posit, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int blkno = 0;
	int slotno = 0;
	int ret = GetSlot(blkno, slotno);
	if (ret > 0)
	{
		stat->code=-ret;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	else if (ret < 0)
	{
		stat->code = -1;
		stat->level = EL_SOFT;
		stat->msg = "�����޿տ���";
		return;
	}
	ret = MoveCardToStorage(blkno, slotno);
	int slotid = BuildSlotId(blkno, slotno);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "���ʧ�ܡ�" + getErrMsg(ret);
		stat->msg = ". ���󿨲�id=" + slotid;
		return;
	}
	posit = slotid;
	*stat = RS_OK;
}

// ���⵽С��
void CCZ_QZ05CT::RepoToCar(int posit, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int blkno = 0;
	int slotno = 0;
	int ret = ParseSlotId(posit, blkno, slotno);
	if (ret < 0)
	{
		stat->code = -1;
		stat->level = EL_SOFT;
		stat->msg = "��Ч���⿨��id.id=" + posit;
		return;
	}
	/*������
	diskNo��[in] �̺�
	slotNo��[in] ��λ�ţ�ȡֵ��Χ1~250*/
	ret = MoveCardStorage2Slider(blkno, slotno);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "���ʧ��." + getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

bool isBoxnumValid(int boxnum)
{
	//��Ч��������
	return boxnum >= 0 && boxnum <= 1;
}

// С����������
// boxnum 0 �Ͽ���; 1 �忨��
void CCZ_QZ05CT::CarToOutbox(int boxnum, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	if (!isBoxnumValid(boxnum))
	{
		stat->code = -1;
		stat->level = EL_SOFT;
		stat->msg = "��Ч��������";
		return;
	}
	int ret = 0;
	if (boxnum == 0)
	{
		//�Ͽ���	0
		ret = MoveCardToWasteCardBox();
	}
	else
	{
		//�����	1
		ret = MoveCardToClearingBox();
	}
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

// ��������С��
void CCZ_QZ05CT::ReaderToCar(int readerId, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = 0;
	if (readerId == READER_ID_CAR)
	{
		*stat = RS_OK;
	}
	else if (readerId == READER_ID_CARD_PRINTER)
	{
		ret = MoveCard310ReaderToSlider();
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		*stat = RS_OK;
	}
	else if (readerId == READER_ID_OUTPORT)
	{
		ret = MoveCardRWReader2Slider();
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		*stat = RS_OK;
	}
	else
	{
		*stat = RS_UNKOWN;
	}
}

// С����������
void CCZ_QZ05CT::CarToReader(int readerId, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = 0;
	if (readerId == READER_ID_CAR)
	{
		*stat = RS_OK;
	}
	else if (readerId == READER_ID_CARD_PRINTER)
	{
		ret = MoveCardTo310Reader();
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		*stat = RS_OK;
	}
	else if(readerId == READER_ID_OUTPORT)
	{
		ret = MoveCardToRWReader();
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		*stat = RS_OK;
	}
	else
	{
		*stat = RS_UNKOWN;
	}
}

// С����֤����ӡ��
// ע�⣺����֤����ӡ����Ҫ�ȶ�д���ٴ�ӡ�����Խ�֤����ӡ������
// ��Ϊ��֤����ӡ�����ö�����310������
// port ֤����ӡ�����ڣ���ʱ��Ĭ��ֵ0
void CCZ_QZ05CT::CarToCardPrinter(int port, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = MoveCardTo310Reader();
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	Close(stat);
	if (stat->code != 0)
	{
		LOG(DEBUG) << "�ó��豸��֤����ӡ���ؼ�ʧ�ܡ�";
		return;
	}
	LOG(DEBUG) << "�ر��쿨ģ��.�ó��豸��֤����ӡ���ؼ����á�";
	*stat = RS_OK;
}

// ֤����ӡ����С��
// ˵����֤����ӡ����С�������������1.��д������У���ƿ����ݳ���
// �����ö�������С��.2. �ƿ���ɣ���֤����ӡ�����ڽ�С����
// port ֤����ӡ������
void CCZ_QZ05CT::CardPrinterToCar(int port, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	// С���ȴ�����
	int ret = 0;
	bool isIn = false;
	RetStatus rs;
	stReaderStatus reas;
	DWORD st = GetTickCount();
	while (true)
	{
		if (GetTickCount() - st > TIMEOUT_PRINTER_TO_CAR_HD6600)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = "��Ƭ��֤����ӡ���ƶ���С����ʱ";
			return;
		}
		ReaderStatus(READER_ID_CAR, reas, stat);
		if (rs.code != 0)
		{
			return;
		}
		if (reas.hasCard)
		{
			*stat = RS_OK;
			return;
		}
		Sleep(300);
	}
}

// С����OCR
void CCZ_QZ05CT::CarToOCR(RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = MoveCardScanner2Slider();
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

// TODO OCR��С��
void CCZ_QZ05CT::OCRToCar(RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	*stat = RS_OK;
}

// С����͹ӡģ��
void CCZ_QZ05CT::CarToEmboss(RetStatus * stat)
{
	// С����͹ӡ��
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = MoveCardToEmboss();
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

// ͹ӡģ�鵽С��
void CCZ_QZ05CT::EmbossToCar(RetStatus * stat)
{
	//͹ӡ����С��
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = MoveCardEmbossToSlider();
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

// С��״̬
void CCZ_QZ05CT::CarStatus(stCardStatus &cs, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	BYTE s[32] = { 0 };
	int ret = GetCommonStatus(s);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	if (s[8] == 0)
	{
		cs.hasCard = FALSE;
	}
	else if (s[8] == 1)
	{
		cs.hasCard = TRUE;
	}
	else
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "С��δ֪����";
		return;
	}
	*stat = RS_OK;
}

//  ������״̬
void CCZ_QZ05CT::InboxStatus(stInboxStatus &is, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	BYTE s[32] = { 0 };
	int ret = GetCommonStatus(s);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	if (s[0] == 1)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "�������쳣";
		return;
	}
	is.num = INBOX_NUM;
	for (int i = 0; i < is.num; i++)
	{
		is.stat[i] = i + 1;
		is.stat[i] = s[i + 1];
	}
	*stat = RS_OK;
}

// ����״̬
void CCZ_QZ05CT::RepoStatus(stRepoStatus &reps, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	BYTE s[32] = { 0 };
	int ret = GetCommonStatus(s);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	if (s[0] == 3 || s[11] != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "�����쳣";
		return;
	}
	bool b;
	ret = IsRepoFull(b);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	reps.isFull = b;
	*stat = RS_OK;
}

// ������״̬
// ����id: 0 �Ͽ���; 1 �忨��
void CCZ_QZ05CT::OutboxStatus(stOutboxStatus &os, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	BYTE s[32] = { 0 };
	int ret = GetCommonStatus(s);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	os.num = OUTBOX_NUM;
	os.ids[0] = 0;
	os.stat[0] = s[9];
	os.ids[1] = 1;
	os.stat[1] = s[10];
	*stat = RS_OK;
}

// TODO ֤����ӡ��״̬
void CCZ_QZ05CT::CardPrinterStatus(RetStatus * stat)
{
	//TODO
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	*stat = RS_OK;
}

// ������״̬
void CCZ_QZ05CT::ReaderStatus(int id, stReaderStatus &rs, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	BYTE cs[64] = { 0 };
	int ret = GetCommonStatus(cs);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	if (id == READER_ID_CAR)
	{
		if (cs[8] == 0)
		{
			rs.hasCard = false;
			rs.cardPosit = READER_STAT_CARD_POSIT_UNKNOWN;
		}
		else if (cs[8] == 1)
		{
			rs.hasCard = true;
			rs.cardPosit = READER_STAT_CARD_POSIT_UNKNOWN;
		}
		else
		{
			*stat = RS_UNKOWN;
			return;
		}
	}
	else if (id == READER_ID_CARD_PRINTER)
	{
		if (cs[26] == 0)
		{
			rs.hasCard = false;
			rs.cardPosit = READER_STAT_CARD_POSIT_UNKNOWN;
		}
		else if (cs[26] == 1)
		{
			rs.hasCard = true;
			rs.cardPosit = READER_STAT_CARD_POSIT_UNKNOWN;
		}
		else
		{
			*stat = RS_UNKOWN;
			return;
		}
	}
	else if (id == READER_ID_OUTPORT)
	{
		if (cs[14] == 0 && cs[16] == 0)
		{
			rs.hasCard = false;
			rs.cardPosit = READER_STAT_CARD_POSIT_UNKNOWN;
		}
		else if (cs[14] == 1 || cs[16] == 1)
		{
			rs.hasCard = true;
			rs.cardPosit = READER_STAT_CARD_POSIT_UNKNOWN;
		}
		else
		{
			*stat = RS_UNKOWN;
			return;
		}
	}
	else
	{
		*stat = RS_PARAM_NOT_SUPPORT;
		return;
	}
	*stat = RS_OK;
}

// TODO ocr״̬
void CCZ_QZ05CT::OCRStatus(RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	*stat = RS_MODULE_NOT_EXIST;
}

// TODO ͹ӡģ��״̬
void CCZ_QZ05CT::EmbossStatus(RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	BYTE es[5] = { 0 };
	int ret = GetEmbossStatus(es);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

// ������λ
void CCZ_QZ05CT::Reset(int type, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	/*������
	mode��[in] ��λģʽ��
	0 �� ������λ��1 �� ����ģ�飬2 �� ���⣬3 �� ��������4 �� RF��������
	5 ����д��������6 �� ͨ���������� 7 �� ƽӡģ�飬8 �� ͹��ģ�飬9 ������ģ��*/
	int ret = ResetDevices(0); //ȫ�渴λ
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

// ����ʹ����
void CCZ_QZ05CT::RepoUsedNum(int & num, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = 0;
	BYTE slots[512] = { 0 };
	int len = 0;
	int cnt = 0;
	for (int diskno = 1; diskno <= 2; diskno++)
	{
		ret = GetStorageSlots(diskno, slots, &len);
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		for (int i = 0; i < len; i++)
		{
			if (slots[i] == 1)
			{
				++cnt;
			}
		}
	}
	num = cnt;
	*stat = RS_OK;
}

// ������ʹ�ÿ���id
void CCZ_QZ05CT::RepoUsedSlots(vector<UINT16>& slots, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	int ret = 0;
	BYTE tslots[512] = { 0 };
	int len = 0;
	int cnt = 0;
	for (int diskno = 1; diskno <= 2; diskno++)
	{
		ret = GetStorageSlots(diskno, tslots, &len);
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		for (int i = 0; i < len; i++)
		{
			if (tslots[i] == 1)
			{
				slots.push_back(BuildSlotId(diskno, i));
			}
		}
	}
	*stat = RS_OK;
}

// ����������
void CCZ_QZ05CT::ReaderCardIn(int readerId, int timeout, RetStatus * stat)
{ 
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	if (readerId != READER_ID_OUTPORT)
	{
		stat->code = -1;
		stat->level = EL_SOFT;
		stat->msg = "�˶������ݲ�֧�ָù���";
		return;
	}
	int ret = 0;
	ret = SetFrontCardInMode(1);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	DWORD s = GetTickCount();
	DWORD c = GetTickCount();
	int st;
	BOOL to = TRUE;
	int cnt = 0;
	while (c - s < 12000)
	{
		ret = GetPassageCardStatus(&st);
		if (ret < 0)
		{
			SetFrontCardInMode(0);
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		if (st == 2 || st == 1)
		{
			if (cnt > 4)
			{
				to = FALSE;
				break;
			}
			cnt++;
		}
		else
		{
			cnt = 0;
		}
		c = GetTickCount();
		Sleep(300);
	}
	SetFrontCardInMode(0);
	if (to)
	{
		// ��ʱ
		stat->code = ERR_TIMEOUT_WAIT_INPUT_CARD;
		stat->level = EL_SOFT;
		stat->msg = "�û�������ʱ";
		return;
	}
	ret = GetPassageCardStatus(&st);
	if (ret < 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	if (st == 2 || st == 1)
	{
		ret = MoveCardPass2RWReader();
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		*stat = RS_OK;
		return;
	}
	else
	{
		// ��ʱ
		stat->code = ERR_TIMEOUT_WAIT_INPUT_CARD;
		stat->level = EL_SOFT;
		stat->msg = "�û�������ʱ";
		return;
	}
}

// ����������
void CCZ_QZ05CT::ReaderCardOut(int readerId, int timeout, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	if (readerId != READER_ID_OUTPORT)
	{
		stat->code = -1;
		stat->level = EL_SOFT;
		stat->msg = "�˶������ݲ�֧�ָù���";
		return;
	}
	int ret = 0;
	DWORD newTickCount = GetTickCount();
	DWORD oldTickCount = newTickCount;
	BYTE status[64] = { 0 };
	while (int(newTickCount - oldTickCount) < timeout)
	{
		newTickCount = GetTickCount();

		// ��������״̬
		ret = GetCommonStatus(status);
		if (ret != 0)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = getErrMsg(ret);
			return;
		}
		if (2 == status[12]) {
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = "����������������id=" + readerId;
			return;
		}
		if (1 != status[14])
		{
			//ȡ�����
			*stat = RS_OK;
			return;
		}
		Sleep(10);
	}
	ret = MoveCardExit2RWReader();			//�����쵽������
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	stat->code = ERR_TIMEOUT_WAIT_TAKE_CARD;
	stat->level = EL_SOFT;
	stat->msg = "�ȴ��û�ȡ����ʱ";
}


