#pragma once
#include "stdafx.h"
#include <string>

using namespace std;

//�籣������������
typedef struct {
	CString ksbm;    //��ʶ����
	CString name;    //����
	CString sex;     //�Ա�
	CString ssno;    //�籣����
	CString idnum;  //���֤��
	CString fkrq;    //��������
	CString yxrq;    //����Ч��
	CString random;  //�������
} SSCBaseInfo;

// CTDApduSend ��������ķ�����
#define ERR_AS_NOT_OPEN -201	// ������δ��
#define ERR_AS_TRANS -202    // ͨѶ����
#define ERR_AS_PARAM -203		// ��������
typedef int (*CTDApduSend)(CString send, CString &resp, DWORD &SW1SW2);

// ������
#define ERR_APDU_SW -203		// APDU״̬�����

// �����������ṹ��
typedef struct {
	bool isContactless;
	CTDApduSend apdu;
} SSCApduSender;

///////�豸״̬
// С��״̬
typedef struct {
	BOOL hasCard;
} stCardStatus;

//������״̬
#define INBOX_STAT_NO_CARD 0		// �޿�
#define INBOX_STAT_CARD_SHORT 1		// �ٿ�
#define INBOX_STAT_NO_BOX 3			// δ����
#define INBOX_STAT_UNKNOWN	4		// δ֪״̬
typedef struct {
	int num;	// ��������
	BYTE ids[16];	// �����ţ���stat��Ӧ
	BYTE stat[16];
} stInboxStatus;

// ����״̬
typedef struct {
	bool isFull;
} stRepoStatus;

// ������״̬
#define OUT_BOX_STAT_NOT_FULL 1		// ������û��
#define OUT_BOX_STAT_FULL 2		// ��������
#define OUT_BOX_STAT_NO_BOX 3		// δ����
typedef struct {
	int num;	// ��������
	BYTE ids[16];
	BYTE stat[16];
} stOutboxStatus;

// ������״̬
//IC���ڶ������е�λ��
#define READER_STAT_CARD_POSIT_UNKNOWN 0		// δ֪
#define READER_STAT_CARD_POSIT_OUT_PORT 1		// ����
#define READER_STAT_CARD_POSIT_IC 2				// ICλ
#define READER_STAT_CARD_POSIT_RF 3				// �ǽӴ�λ
typedef struct {
	bool hasCard;
	int cardPosit;	// IC���ڶ������е�λ��
} stReaderStatus;