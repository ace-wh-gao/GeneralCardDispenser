#include "stdafx.h"

#ifndef DEVICEDEFINE_H
#define DEVICEDEFINE_H

#define PRINT_TEXT_COUNT 100
#define PRINT_IMAGE_COUNT 100

#define OK 0
#define ERR 1  // һ�����

#define ERR_MODULE_COMM 2  // ͨѶ����
#define ERR_MODULE_EXEC 3  // ͨѶ������ִ��ʧ��

#define DEFAULT_SEPERATOR "#"  // �������ݷ���Ĭ�Ϸָ���
#define DEFAULT_CONFIG_STR _T("")  // Ĭ�Ͽ��ַ���


/* ������ */
#define ERR_NONE                0    // ����
#define ERR_NO_ENCODER          1    // ����ģ�鲻����
#define ERR_NO_CARD_PRINTER     2    // ��ӡ��������
#define ERR_NO_SLIDER           3    // ������������
#define ERR_NO_STORAGE          4    // ���ⲻ����
#define ERR_NO_CAR_READER       5    // С��������������
#define EER_NO_RW_READER        6    // ��д������������
#define EER_NO_PASS_READER      7    // ͨ��������������
#define EER_NO_EMBOSS           8    // ͹��ģ�鲻����
#define EER_NO_HOT_MODULE       9    // ����ģ�鲻����
#define ERR_DEVICE_NOT_OPEN    10    // �豸δ��
#define ERR_COMMUNICATION      11    // ģ��ͨѶ�� 
#define ERR_NEGATIVE_RESP      12    // ģ��ִ�д�
#define ERR_NOT_PUT_CARD_BOX   13    // ��ѡ������δ����
#define ERR_CARD_BOX_EMPTY     14    // ��ѡ�������޿�
#define ERR_TARGET_NO_CARD     15    // Ŀ��λ���޿�
#define ERR_TARGET_HAVE_CARD   16    // Ŀ��λ���п�
#define ERR_CARD_AT_EXIT       17    // �������п�
#define ERR_PARAMETER_WRONG    18    // ������
#define ERR_LOAD_CONFIG_WRONG  19    // ��������ʧ��
#define ERR_INTERNAL_ERR       20    // �ڲ������쳣
#define ERR_NOT_SUPPORT        21    // ���ܻ�����ݲ�֧��
#define ERR_MODULE_NO_INIT     22    // ģ��δ��ʼ��
#define ERR_CARD_NOT_ACTIVE    23    // ��Ƭδ�ϵ缤��
#define EER_NO_BIN_BOX         24    // �����������߲�����
#define EER_NO_WAIT_TIME       25    // ִ�г�ʱ
#define EER_NO_SCAN_MODULE     26    // ɨ����ģ�鲻����
#define EER_SCAN_JAM           27    // ɨ����ģ��п�
#define ERR_NO_310_READER      28    // 310������������
#define ERR_OTHER              99    // ������



// RS232����ͨѶ������
#define RS232_Bad_Open  -1
#define RS232_Bad_Close -2
#define RS232_Bad_CommSet -3
#define RS232_Bad_CommTimeouts -4
#define RS232_Bad_ReceiveData -5
#define RS232_Bad_ReceiveLen -6
#define RS232_Bad_SendData -7
#define RS232_Bad_SendLen -8
#define RS232_Bad_CommQueue -9
#define RS232_Timeout -10
#define RS232_CRC_ERR -11
#define RS232_ACK_TIMEOUT -12
#define RS232_NAK_RESP -13
#define RS232_INVALID_RESP -14

/* �豸ģ�鶨�� */
typedef enum tagDeviceModule
{
	MODULE_DEFAULT = 0,
	MODULE_ENCODER = 1,   // ����ģ��
	MODULE_STORAGE = 2,   // ���� 
	MODULE_SLIDER = 3,    // С��������
	MODULE_CAR_READER = 4, // С��������
	MODULE_RW_READER = 5, // ��д������
	MODULE_PASS_READER = 6, // ͨ��������
	MODULE_PRINTER = 7,     // ƽӡ��
	MODULE_EMBOSS = 8,      // ͹��ģ��
	MODULE_SINGLE_HOT = 9,  // ����ģ��
	MODULE_SCANNER = 10,    //ɨ����
	MODULE_310_READER = 11,    //310������
	MODULE_MAX
} DeviceModule;

// �豸������
typedef struct tagDeviceErrInfo 
{
	unsigned long dwErrCode;
	const TCHAR* pErr;
	const TCHAR* pErrMsg;
} DEVICE_ERR_INFO;

// ��λ��
typedef enum tagCardStopPos 
{
	NO_CARD = 0,	  //�޿�
	MID_CARD = 1,     //�м��п�
	FRONT_CARD = 2,   //ǰ�˳ֿ�
	BACK_CARD = 3,    //��˳ֿ�
	ERR_CARD = 4,     //����
} CardStopPos;


// ��ӡ������
typedef enum tagPrinterType
{
	PrinterNone,
		PrinterSeaory,  // ������ֱӡ��
		PrinterAvansia, // ����˿תӡ��
		PrinterMatica,  // ��Ͽ�תӡ��
		PrinterDC7600,  // ��ʵתӡ��
		PrinterHDP6600, // ����תӡ��
		PrinterMax
} PrinterType;


// ��ӡ����
typedef struct tagDocProp
{
	BYTE	byOrientation;		//��ӡ�������1=����, 2=����
	BYTE	byRibbonType;		//����ʹ�õ�ɫ����0=YMCKO, 1=K, 2=1/2YMCKO, 3=YMCKOK, 4=KO
	BYTE	byPrintSide;		//������ӡ�棺0x00=�Զ�
	BYTE	byPrintPanelFront;	//����ʹ�õ�ɫ��ɫ�㣺0=YMCKO, 1=YMCO, 2=K
	BYTE	byPrintPanelBack;	//����ʹ�õ�ɫ��ɫ�㣺0=YMCKO, 1=YMCO, 2=K
	BYTE	byResinKfront;		//�����ɫ��K��KO�Ĵ�ӡ������0=��ɫ, 1=��/��
	BYTE	byResinKback;		//�����ɫ��K��KO�Ĵ�ӡ������0=��ɫ, 1=��/��
	BYTE	byRotate180;		//ָ����һ����Ҫ��ת180�ȣ�0x00=������ת��0x01=���棬 0x10=���棬0x11=˫����ת
	
	BYTE	byInputBin;			//������1=ǰ���ֶ�������,2=����ֶ�������
	BYTE	byFeedCardMode;		//����ģʽ��0x00��Ĭ��
	BYTE	byOutputBin;		//������1=ǰ���ֶ�������,2=����ֶ�������
	BYTE	byEjectCardMode;	//����ģʽ��0=ֱ�ӳ���, 1=�ȴ�ȡ�߿�Ƭ
	
	BYTE	byReserved[244];    //����
} DocProp;


// ��ӡͼ��
typedef struct tagCardImageProp
{
	long	posX;		// ͼ��λ��X (pixer)
	long	posY;		// ͼ��λ��Y (pixer)
	long	width;		// ͼ���� (pixer)����Ϊ0����ͼ��ԭʼ��ȴ�ӡ
	long	hight;	    // ͼ��߶� (pixer)����Ϊ0����ͼ��ԭʼ�߶ȴ�ӡ
	TCHAR	imagePath[256];	// ͼ��·��
} CardImageProp;

typedef enum tagPrinterStatus
{
	PrintStateReady = 0,       //�Ѿ���
		PrintStateDisconnect = 1,  //δ����
		PrintStateBusy = 2,        //�豸æ
		PrintStateJamCard = 3,     //�п�
		PrintStateCoverOpend = 4,  //�ϸǴ�
		PrintStateCardAtEntrance = 5,  //����ӡ��������λ��
		PrintStateCardAtPrint = 6,     //���ڴ�ӡλ��
		PrintStateCardAtMagnetic = 7,  //���ڴſ�λ��
		PrintStateCardAtIC = 8,        //����ICλ��
		PrintStateCardAtRF = 9,        //����RFλ��
		PrintStateCardAtExit = 10,     //���ڳ���λ��
		PrintStateCardAtBody = 11,     //����ӡ�����ڲ�
		PrintStateAbnormal = 254,  //ɫ��״̬�쳣
		PrintStateError = 255      //����
} PrinterStatus;

typedef struct tagModuleThreadParam
{
	int module; // ģ����
	int param1; // ����1
	int param2; // ����2
	int param3; // ����3
	int param4; // ����4
}ModuleThreadParam;

// �¼��ź�ID
typedef enum tagThreadEvent
{
	ThreadStorageEvent = 1,	    //����
		ThreadEncoderEvent = 2,		//������
		ThreadSliderEvent = 3,		//С��
		ThreadEmbossCardEvent = 4,	//͹��
		ThreadPrinterEvent = 5,		//��ӡ��
} ThreadEvent;

// �豸ģ��ͨѶ�ܵ�
typedef struct tagDeviceSockets
{
	TCHAR comEncoder[8];   // ����ģ��
	TCHAR comStorage[8];   // ����
	TCHAR comSlider[8];    // С��
	TCHAR comCarReader[8];  // С��������
	TCHAR com310Reader[8];  // 310������
	TCHAR comRWReader[8];  // ��д������
	TCHAR comPassReader[8]; // ͨ��������
	TCHAR comEmboss[8];  // ͹��ģ��
	TCHAR comSingleHot[8];  // ����ģ��
	TCHAR szPrinterName[16]; // ��ӡ������
	TCHAR szScannerName[30]; // ɨ��������
} DeviceSockets;


// ͹������
typedef struct tagEmbossProp
{
	int posX;   // ��ʼ�ַ�����X: ��Ƭ����ص��ַ��ľ��루��λ0.1mm��
	int posY;   // ��ʼ�ַ�����Y: ��Ƭ�±��ص������ױߵľ��루��λ0.1mm��
	int characterType;  // ͹�����ͣ�1 - GOTHIC��2 - OCR7B��3 - REAR
	int contentLen;     // ͹ӡ�ַ�λ��
	char content[32];    // ͹���ַ����� 
} EmbossProp;

//APIִ�н������ֵ
typedef enum
{
    _IDS_RET_ERR = -50,             //һ�����
		_IDS_RET_ERR_JAM,               //��ס
		_IDS_RET_ERR_MEDIA,             //�����ʲ�����
		_IDS_RET_ERR_SIDE,              //��֧�ֵ�ɨ����������
		_IDS_RET_ERR_MODE,              //��֧�ֵ���ɫģʽ
		_IDS_RET_ERR_TYPE,              //��֧�ֵĿ�����
		_IDS_RET_ERR_DPI,               //��֧�ֵķֱ���
		_IDS_RET_ERR_DELAY,             //��ʱʱ�䳬����Χ
		_IDS_RET_ERR_RF,                //RF��ȡʧ��
		_IDS_RET_ERR_UNSUPPORTED,       //��֧�ֵĹ���
		_IDS_RET_OK = 0,                //״̬����    
		_IDS_RET_BUSY                   //�豸��æ
}_IDS_RET;

typedef enum tagHDPrinterState
{
	SDKCurrentActivityUnmapped = -2, 
		SDKCurrentActivityUnknown = -1, 
		SDKCurrentActivityIdle = 0,
		SDKCurrentActivityReady,
		SDKCurrentActivityPrinting,
		SDKCurrentActivityPaused,
		SDKCurrentActivityWaitingForEject,
		SDKCurrentActivityCancel,
		SDKCurrentActivityException,
		SDKCurrentActivityCardHopper = 20,
		SDKCurrentActivityCardFeeding,
		SDKCurrentActivityCardFlipping,
		SDKCurrentActivityCardEjecting,
		SDKCurrentActivityCardHandoff,
		SDKCurrentActivityEncodeWait = 30,
		SDKCurrentActivityEncodeMagStrip,
		SDKCurrentActivityEncodeContact,
		SDKCurrentActivityEncodeContactless,
		SDKCurrentActivityEncodeProx,
		SDKCurrentActivityTransferHeating = 70,
		SDKCurrentActivityTransferCooling,
		SDKCurrentActivityTransferBusy,
		SDKCurrentActivityLamWaiting = 90
} HDPrinterState;

#endif
