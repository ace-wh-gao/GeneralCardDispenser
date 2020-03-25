#pragma once
#include "stdafx.h"
#include <string>

using namespace std;

//社保基本数据数据
typedef struct {
	CString ksbm;    //卡识别码
	CString name;    //姓名
	CString sex;     //性别
	CString ssno;    //社保卡号
	CString idnum;  //身份证号
	CString fkrq;    //发卡日期
	CString yxrq;    //卡有效期
	CString random;  //卡随机数
} SSCBaseInfo;

// CTDApduSend 函数定义的返回码
#define ERR_AS_NOT_OPEN -201	// 读卡器未打开
#define ERR_AS_TRANS -202    // 通讯错误
#define ERR_AS_PARAM -203		// 参数错误
typedef int (*CTDApduSend)(CString send, CString &resp, DWORD &SW1SW2);

// 错误码
#define ERR_APDU_SW -203		// APDU状态码错误

// 读卡器读卡结构体
typedef struct {
	bool isContactless;
	CTDApduSend apdu;
} SSCApduSender;

///////设备状态
// 小车状态
typedef struct {
	BOOL hasCard;
} stCardStatus;

//发卡箱状态
#define INBOX_STAT_NO_CARD 0		// 无卡
#define INBOX_STAT_CARD_SHORT 1		// 少卡
#define INBOX_STAT_NO_BOX 3			// 未放置
#define INBOX_STAT_UNKNOWN	4		// 未知状态
typedef struct {
	int num;	// 卡箱数量
	BYTE ids[16];	// 卡箱编号，与stat对应
	BYTE stat[16];
} stInboxStatus;

// 卡库状态
typedef struct {
	bool isFull;
} stRepoStatus;

// 出卡箱状态
#define OUT_BOX_STAT_NOT_FULL 1		// 回收箱没满
#define OUT_BOX_STAT_FULL 2		// 回收箱满
#define OUT_BOX_STAT_NO_BOX 3		// 未放置
typedef struct {
	int num;	// 卡箱数量
	BYTE ids[16];
	BYTE stat[16];
} stOutboxStatus;

// 读卡器状态
//IC卡在读卡器中的位置
#define READER_STAT_CARD_POSIT_UNKNOWN 0		// 未知
#define READER_STAT_CARD_POSIT_OUT_PORT 1		// 卡口
#define READER_STAT_CARD_POSIT_IC 2				// IC位
#define READER_STAT_CARD_POSIT_RF 3				// 非接触位
typedef struct {
	bool hasCard;
	int cardPosit;	// IC卡在读卡器中的位置
} stReaderStatus;