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

// 法高打印机到小车的超时时间
#define TIMEOUT_PRINTER_TO_CAR_HD6600 480000

static map<int, string > kErrMsg =
{
	{ ERR_NONE, "正常" },
	{ ERR_NO_ENCODER, "发卡模块不在线" },
	{ ERR_NO_CARD_PRINTER, "平印机不在线" },
	{ ERR_NO_SLIDER, "滑动器不在线" },
	{ ERR_NO_STORAGE, "卡库不在线" },
	{ ERR_NO_CAR_READER, "小车读卡器不在线" },
	{ EER_NO_RW_READER, "读写读卡器不在线" },
	{ EER_NO_PASS_READER, "通道读卡器不在线" },
	{ EER_NO_EMBOSS, "凸烫模块不在线" },
	{ EER_NO_HOT_MODULE, "单烫模块不在线" },
	{ ERR_DEVICE_NOT_OPEN, "设备未打开" },
	{ ERR_COMMUNICATION, "模块通讯错" },
	{ ERR_NEGATIVE_RESP, "模块执行错" },
	{ ERR_NOT_PUT_CARD_BOX, "所选发卡箱未放置" },
	{ ERR_CARD_BOX_EMPTY, "所选发卡箱无卡" },
	{ ERR_TARGET_NO_CARD, "目标位置无卡" },
	{ ERR_TARGET_HAVE_CARD, "目标位置有卡" },
	{ ERR_CARD_AT_EXIT, "出卡嘴有卡" },
	{ ERR_PARAMETER_WRONG, "参数错" },
	{ ERR_LOAD_CONFIG_WRONG, "加载配置失败" },
	{ ERR_INTERNAL_ERR, "内部调用异常" },
	{ ERR_NOT_SUPPORT, "功能或操作暂不支持" },
	{ ERR_MODULE_NO_INIT, "模块未初始化" },
	{ ERR_CARD_NOT_ACTIVE, "卡片未上电激活" },
	{ EER_NO_BIN_BOX, "回收箱满或者不存在" },
	{ EER_NO_WAIT_TIME, "执行超时" },
	{ EER_NO_SCAN_MODULE, "扫描仪模块不在线" },
	{ EER_SCAN_JAM, "扫描仪模块夹卡" },
	{ ERR_OTHER, "其它错" },
};

// 卡库块的有效容量
const int kBlkCapacity = 250;
// 卡库块的有编号范围
const int kMinBlkNum = 1;
const int kMaxBlkNum = 2;

#define INBOX_NUM 6		// 发卡箱数量
#define OUTBOX_NUM 2	// 出卡箱数量

// 获取厂家错误信息
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

	// TODO 第一次打开时加载配置文件
	// 
	LOG(INFO) << "根据配置文件打开设备";
	int ret = OpenDevices(1);
	if (ret > 0)
	{
		LOG(WARNING) << "错误码: " << ret << ".错误信息: " << getErrMsg(ret);
		LOG(WARNING) << "打开设备失败";
		LOG(INFO) << "扫描硬件打开设备";
		ret = OpenDevices(0);
		if (ret > 0)
		{
			LOG(ERROR) << "错误码: " << ret << ".错误信息: " << getErrMsg(ret);
			LOG(WARNING) << "打开设备失败";
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
		LOG(ERROR) << "错误码: " << ret << ".错误信息: " << getErrMsg(ret);
		LOG(WARNING) << "关闭设备失败";
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	mIsOpen = false;
	*stat = RS_OK;
	return;
}

// 发卡箱到小车
// 只要有放置一个卡箱就不会报卡箱未放置错误。
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
	/*参数：
	boxNo：[in] 卡箱号：取值范围1~6*/
	for (boxNo = 1; boxNo <= 6; boxNo++)
	{
		//循环，直到无卡为止；换卡箱发卡
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
			stat->msg = "未放置发卡箱";
		}
		else if (ERR_CARD_BOX_EMPTY == ret2)
		{
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = "发卡箱无卡";
		}
		else {
			stat->code = -1;
			stat->level = EL_HARDWARE;
			stat->msg = "发卡箱未知错误";
			return;
		}
	}
}

//查询卡库中的空卡槽
// -1 未找到空卡槽
// >0 厂家错误码
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

// 创建卡槽号编号
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
	// 块号的有效范围
	if (blkno > kMaxBlkNum || blkno < kMinBlkNum)
	{
		return -1;
	}
	return 0;
}

// TODO 需要区分非接和接触，前提条件：添加移动卡接口
// 返回值 ！= 0 成功。
// > 0 硬件调用错误。
// < 0 逻辑错误
int CCZ_QZ05CT::GetReader(int readerId, shared_ptr<SSCApduSender>& sender)
{
	int ret = 0;
	switch (readerId)
	{
	case READER_ID_CAR:	// 小车读卡器
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
	case READER_ID_OUTPORT: // 卡口读卡器
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
	case READER_ID_CARD_PRINTER: // 证卡读卡器
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
		LOG(ERROR) << "不支持的读卡器id.id=" << readerId;
		ret = -1;
		break;
	}
	return ret;
}

// 卡库是否满
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

// 读卡器激活IC卡
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
		stat->msg = "读卡器激活IC卡失败";
		return;
	}
}

void CCZ_QZ05CT::LoadCfg(RetStatus * stat)
{
	*stat = RS_OK;
}

// 读取社保基本信息
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
		stat->msg = "读卡器传输错误";
	}
	else {
		*stat = RS_UNKOWN;
	}
}

// 读取银行卡信息
// 只实现了小车读卡器
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
		stat->msg = "读卡器传输错误";
	}
	else {
		*stat = RS_UNKOWN;
	}
}

// 小车到卡库
// posit 卡槽id
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
		stat->msg = "卡库无空卡槽";
		return;
	}
	ret = MoveCardToStorage(blkno, slotno);
	int slotid = BuildSlotId(blkno, slotno);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "入库失败。" + getErrMsg(ret);
		stat->msg = ". 错误卡槽id=" + slotid;
		return;
	}
	posit = slotid;
	*stat = RS_OK;
}

// 卡库到小车
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
		stat->msg = "无效卡库卡槽id.id=" + posit;
		return;
	}
	/*参数：
	diskNo：[in] 盘号
	slotNo：[in] 仓位号，取值范围1~250*/
	ret = MoveCardStorage2Slider(blkno, slotno);
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = "入库失败." + getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

bool isBoxnumValid(int boxnum)
{
	//有效出卡箱编号
	return boxnum >= 0 && boxnum <= 1;
}

// 小车到出卡箱
// boxnum 0 废卡箱; 1 清卡箱
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
		stat->msg = "无效出卡箱编号";
		return;
	}
	int ret = 0;
	if (boxnum == 0)
	{
		//废卡箱	0
		ret = MoveCardToWasteCardBox();
	}
	else
	{
		//清分箱	1
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

// 读卡器到小车
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

// 小车到读卡器
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

// 小车到证卡打印机
// 注意：由于证卡打印机需要先读写卡再打印，所以进证卡打印机操作
// 改为进证卡打印机外置读卡器310读卡器
// port 证卡打印机卡口，暂时填默认值0
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
		LOG(DEBUG) << "让出设备给证卡打印机控件失败。";
		return;
	}
	LOG(DEBUG) << "关闭领卡模块.让出设备给证卡打印机控件调用。";
	*stat = RS_OK;
}

// 证卡打印机到小车
// 说明：证卡打印机进小车存在种情况，1.读写卡或者校验制卡数据出错，
// 从外置读卡器进小车.2. 制卡完成，从证卡打印机卡口进小车。
// port 证卡打印机卡口
void CCZ_QZ05CT::CardPrinterToCar(int port, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	// 小车等待进卡
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
			stat->msg = "卡片从证卡打印机移动到小车超时";
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

// 小车到OCR
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

// TODO OCR到小车
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

// 小车到凸印模块
void CCZ_QZ05CT::CarToEmboss(RetStatus * stat)
{
	// 小车到凸印机
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

// 凸印模块到小车
void CCZ_QZ05CT::EmbossToCar(RetStatus * stat)
{
	//凸印机到小车
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

// 小车状态
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
		stat->msg = "小车未知错误";
		return;
	}
	*stat = RS_OK;
}

//  发卡箱状态
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
		stat->msg = "发卡箱异常";
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

// 卡库状态
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
		stat->msg = "卡库异常";
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

// 出卡箱状态
// 卡箱id: 0 废卡箱; 1 清卡箱
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

// TODO 证卡打印机状态
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

// 读卡器状态
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

// TODO ocr状态
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

// TODO 凸印模块状态
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

// 整机复位
void CCZ_QZ05CT::Reset(int type, RetStatus * stat)
{
	ASSERT(stat != nullptr);
	if (!mIsOpen)
	{
		*stat = RS_DISPENSER_NOT_OPEN;
		return;
	}
	/*参数：
	mode：[in] 复位模式：
	0 － 整机复位，1 － 发卡模块，2 － 卡库，3 － 滑动器，4 － RF读卡器，
	5 －读写读卡器，6 － 通道读卡器， 7 － 平印模块，8 － 凸烫模块，9 －单烫模块*/
	int ret = ResetDevices(0); //全面复位
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	*stat = RS_OK;
}

// 卡库使用量
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

// 卡库已使用卡槽id
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

// 读卡器进卡
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
		stat->msg = "此读卡器暂不支持该功能";
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
		// 超时
		stat->code = ERR_TIMEOUT_WAIT_INPUT_CARD;
		stat->level = EL_SOFT;
		stat->msg = "用户进卡超时";
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
		// 超时
		stat->code = ERR_TIMEOUT_WAIT_INPUT_CARD;
		stat->level = EL_SOFT;
		stat->msg = "用户进卡超时";
		return;
	}
}

// 读卡器出卡
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
		stat->msg = "此读卡器暂不支持该功能";
		return;
	}
	int ret = 0;
	DWORD newTickCount = GetTickCount();
	DWORD oldTickCount = newTickCount;
	BYTE status[64] = { 0 };
	while (int(newTickCount - oldTickCount) < timeout)
	{
		newTickCount = GetTickCount();

		// 检查读卡器状态
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
			stat->msg = "读卡器出错。读卡器id=" + readerId;
			return;
		}
		if (1 != status[14])
		{
			//取卡完成
			*stat = RS_OK;
			return;
		}
		Sleep(10);
	}
	ret = MoveCardExit2RWReader();			//出卡嘴到读卡器
	if (ret != 0)
	{
		stat->code = -1;
		stat->level = EL_HARDWARE;
		stat->msg = getErrMsg(ret);
		return;
	}
	stat->code = ERR_TIMEOUT_WAIT_TAKE_CARD;
	stat->level = EL_SOFT;
	stat->msg = "等待用户取卡超时";
}


