#include "stdafx.h"

#ifndef DEVICEDEFINE_H
#define DEVICEDEFINE_H

#define PRINT_TEXT_COUNT 100
#define PRINT_IMAGE_COUNT 100

#define OK 0
#define ERR 1  // 一般错误

#define ERR_MODULE_COMM 2  // 通讯不上
#define ERR_MODULE_EXEC 3  // 通讯正常，执行失败

#define DEFAULT_SEPERATOR "#"  // 多组数据返回默认分隔符
#define DEFAULT_CONFIG_STR _T("")  // 默认空字符串


/* 错误码 */
#define ERR_NONE                0    // 正常
#define ERR_NO_ENCODER          1    // 发卡模块不在线
#define ERR_NO_CARD_PRINTER     2    // 打印机不在线
#define ERR_NO_SLIDER           3    // 滑动器不在线
#define ERR_NO_STORAGE          4    // 卡库不在线
#define ERR_NO_CAR_READER       5    // 小车读卡器不在线
#define EER_NO_RW_READER        6    // 读写读卡器不在线
#define EER_NO_PASS_READER      7    // 通道读卡器不在线
#define EER_NO_EMBOSS           8    // 凸烫模块不在线
#define EER_NO_HOT_MODULE       9    // 单烫模块不在线
#define ERR_DEVICE_NOT_OPEN    10    // 设备未打开
#define ERR_COMMUNICATION      11    // 模块通讯错 
#define ERR_NEGATIVE_RESP      12    // 模块执行错
#define ERR_NOT_PUT_CARD_BOX   13    // 所选发卡箱未放置
#define ERR_CARD_BOX_EMPTY     14    // 所选发卡箱无卡
#define ERR_TARGET_NO_CARD     15    // 目标位置无卡
#define ERR_TARGET_HAVE_CARD   16    // 目标位置有卡
#define ERR_CARD_AT_EXIT       17    // 出卡嘴有卡
#define ERR_PARAMETER_WRONG    18    // 参数错
#define ERR_LOAD_CONFIG_WRONG  19    // 加载配置失败
#define ERR_INTERNAL_ERR       20    // 内部调用异常
#define ERR_NOT_SUPPORT        21    // 功能或操作暂不支持
#define ERR_MODULE_NO_INIT     22    // 模块未初始化
#define ERR_CARD_NOT_ACTIVE    23    // 卡片未上电激活
#define EER_NO_BIN_BOX         24    // 回收箱满或者不存在
#define EER_NO_WAIT_TIME       25    // 执行超时
#define EER_NO_SCAN_MODULE     26    // 扫描仪模块不在线
#define EER_SCAN_JAM           27    // 扫描仪模块夹卡
#define ERR_NO_310_READER      28    // 310读卡器不在线
#define ERR_OTHER              99    // 其它错



// RS232串口通讯错误定义
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

/* 设备模块定义 */
typedef enum tagDeviceModule
{
	MODULE_DEFAULT = 0,
	MODULE_ENCODER = 1,   // 发卡模块
	MODULE_STORAGE = 2,   // 卡库 
	MODULE_SLIDER = 3,    // 小车滑动器
	MODULE_CAR_READER = 4, // 小车读卡器
	MODULE_RW_READER = 5, // 读写读卡器
	MODULE_PASS_READER = 6, // 通道读卡器
	MODULE_PRINTER = 7,     // 平印机
	MODULE_EMBOSS = 8,      // 凸烫模块
	MODULE_SINGLE_HOT = 9,  // 单烫模块
	MODULE_SCANNER = 10,    //扫描仪
	MODULE_310_READER = 11,    //310读卡器
	MODULE_MAX
} DeviceModule;

// 设备错误码
typedef struct tagDeviceErrInfo 
{
	unsigned long dwErrCode;
	const TCHAR* pErr;
	const TCHAR* pErrMsg;
} DEVICE_ERR_INFO;

// 卡位置
typedef enum tagCardStopPos 
{
	NO_CARD = 0,	  //无卡
	MID_CARD = 1,     //中间有卡
	FRONT_CARD = 2,   //前端持卡
	BACK_CARD = 3,    //后端持卡
	ERR_CARD = 4,     //其它
} CardStopPos;


// 打印机类型
typedef enum tagPrinterType
{
	PrinterNone,
		PrinterSeaory,  // 赛尔瑞直印机
		PrinterAvansia, // 爱丽丝转印机
		PrinterMatica,  // 玛迪卡转印机
		PrinterDC7600,  // 得实转印机
		PrinterHDP6600, // 法高转印机
		PrinterMax
} PrinterType;


// 打印配置
typedef struct tagDocProp
{
	BYTE	byOrientation;		//打印输出方向：1=纵向, 2=横向
	BYTE	byRibbonType;		//工作使用的色带：0=YMCKO, 1=K, 2=1/2YMCKO, 3=YMCKOK, 4=KO
	BYTE	byPrintSide;		//工作打印面：0x00=自动
	BYTE	byPrintPanelFront;	//正面使用的色带色层：0=YMCKO, 1=YMCO, 2=K
	BYTE	byPrintPanelBack;	//背面使用的色带色层：0=YMCKO, 1=YMCO, 2=K
	BYTE	byResinKfront;		//正面的色带K或KO的打印方法：0=递色, 1=黑/白
	BYTE	byResinKback;		//背面的色带K或KO的打印方法：0=递色, 1=黑/白
	BYTE	byRotate180;		//指定哪一面需要旋转180度：0x00=都不旋转，0x01=正面， 0x10=背面，0x11=双面旋转
	
	BYTE	byInputBin;			//进卡：1=前端手动进卡口,2=后端手动进卡口
	BYTE	byFeedCardMode;		//进卡模式：0x00＝默认
	BYTE	byOutputBin;		//出卡：1=前端手动进卡口,2=后端手动进卡口
	BYTE	byEjectCardMode;	//出卡模式：0=直接出卡, 1=等待取走卡片
	
	BYTE	byReserved[244];    //保留
} DocProp;


// 打印图像
typedef struct tagCardImageProp
{
	long	posX;		// 图像位置X (pixer)
	long	posY;		// 图像位置Y (pixer)
	long	width;		// 图像宽度 (pixer)，设为0则以图像原始宽度打印
	long	hight;	    // 图像高度 (pixer)，设为0则以图像原始高度打印
	TCHAR	imagePath[256];	// 图像路径
} CardImageProp;

typedef enum tagPrinterStatus
{
	PrintStateReady = 0,       //已就绪
		PrintStateDisconnect = 1,  //未连接
		PrintStateBusy = 2,        //设备忙
		PrintStateJamCard = 3,     //夹卡
		PrintStateCoverOpend = 4,  //上盖打开
		PrintStateCardAtEntrance = 5,  //卡在印卡机进卡位置
		PrintStateCardAtPrint = 6,     //卡在打印位置
		PrintStateCardAtMagnetic = 7,  //卡在磁卡位置
		PrintStateCardAtIC = 8,        //卡在IC位置
		PrintStateCardAtRF = 9,        //卡在RF位置
		PrintStateCardAtExit = 10,     //卡在出卡位置
		PrintStateCardAtBody = 11,     //卡在印卡机内部
		PrintStateAbnormal = 254,  //色带状态异常
		PrintStateError = 255      //出错
} PrinterStatus;

typedef struct tagModuleThreadParam
{
	int module; // 模块编号
	int param1; // 参数1
	int param2; // 参数2
	int param3; // 参数3
	int param4; // 参数4
}ModuleThreadParam;

// 事件信号ID
typedef enum tagThreadEvent
{
	ThreadStorageEvent = 1,	    //卡库
		ThreadEncoderEvent = 2,		//发卡箱
		ThreadSliderEvent = 3,		//小车
		ThreadEmbossCardEvent = 4,	//凸烫
		ThreadPrinterEvent = 5,		//打印机
} ThreadEvent;

// 设备模块通讯管道
typedef struct tagDeviceSockets
{
	TCHAR comEncoder[8];   // 发卡模块
	TCHAR comStorage[8];   // 卡库
	TCHAR comSlider[8];    // 小车
	TCHAR comCarReader[8];  // 小车读卡器
	TCHAR com310Reader[8];  // 310读卡器
	TCHAR comRWReader[8];  // 读写读卡器
	TCHAR comPassReader[8]; // 通道读卡器
	TCHAR comEmboss[8];  // 凸烫模块
	TCHAR comSingleHot[8];  // 单烫模块
	TCHAR szPrinterName[16]; // 打印机名称
	TCHAR szScannerName[30]; // 扫描仪名称
} DeviceSockets;


// 凸字属性
typedef struct tagEmbossProp
{
	int posX;   // 起始字符坐标X: 卡片左边沿到字符的距离（单位0.1mm）
	int posY;   // 起始字符坐标Y: 卡片下边沿到字条底边的距离（单位0.1mm）
	int characterType;  // 凸字类型：1 - GOTHIC，2 - OCR7B，3 - REAR
	int contentLen;     // 凸印字符位数
	char content[32];    // 凸字字符内容 
} EmbossProp;

//API执行结果返回值
typedef enum
{
    _IDS_RET_ERR = -50,             //一般错误
		_IDS_RET_ERR_JAM,               //卡住
		_IDS_RET_ERR_MEDIA,             //卡介质不存在
		_IDS_RET_ERR_SIDE,              //不支持的扫描面向类型
		_IDS_RET_ERR_MODE,              //不支持的颜色模式
		_IDS_RET_ERR_TYPE,              //不支持的卡类型
		_IDS_RET_ERR_DPI,               //不支持的分辨率
		_IDS_RET_ERR_DELAY,             //延时时间超出范围
		_IDS_RET_ERR_RF,                //RF读取失败
		_IDS_RET_ERR_UNSUPPORTED,       //不支持的功能
		_IDS_RET_OK = 0,                //状态正常    
		_IDS_RET_BUSY                   //设备正忙
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
