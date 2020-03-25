/**
  * @file       CRT_591_QZ05_CT3.h
  
  * @author     深圳市创自技术有限公司
  
  * @date       2020/03/09
  
  * @version    2, 0, 2, 309
  
  * @remarks    CRT_591_QZ05_CT3制卡一体机接口文件
  
  */
#include "DeviceDefine.h"

#ifdef CRT_591_QZ05_CT3_EXPORTS
#define CARDPROCESS_API __declspec(dllexport)
#else
#define CARDPROCESS_API __declspec(dllimport)
#endif


// 返回错误码 11 、12时，调用GetDeviceErrInfo()可获取具体模块的统一错误码及错误描述

#ifdef __cplusplus
extern "C" {
#endif

/*
	打开设备
	参数：
		config：[in]  配置类型：0 － 自动检测各模块打开设备； 1 － 从配置文件中加载参数打开设备
	返回值：
		0 - 成功；>0 - 失败，见错误码
*/
CARDPROCESS_API int APIENTRY OpenDevices(int config);


/*
	关闭设备
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY CloseDevices();

/*
	释放动态库（仅上位机软件退出时候调用）
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY ReleaseDll();

/*
	日志开关
	参数:
		level：0 - 不启用日志，1 - 启用日志
		path：日志存储路径，为空则默认路径
	返回值：
			无
  */
CARDPROCESS_API void APIENTRY SetTraceLog(int level, char *path);

/*
	清理设备内的卡
	参数：
	mode：[in] 清理模式：
			0 － 将设备内的卡清理进废卡箱，1 － 将设备内的卡清理进清分箱
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY ClearDeviceCard(int mode);
///////////////////////////////////////////////////////////////////////////////////////

/*
	获取固件版本号
	参数：
		index: [in] 模块索引：	
				0 - 所有模块，以"#"分隔，1 － 发卡模块，2 － 卡库，3 － 小车，4 － 小车读卡器，5 －读写读卡器，
				6 － 通道读卡器， 7 － 打印机，8 － 凸烫模块，9 －单烫模块, 10 - 扫描仪，11 - 310读卡器
		firmware：[out] 输出对应的固件版本信息，缓冲区大小至少250字节
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY GetDeviceFWVersion(int index, char *firmware);

/*
	获取动态库版本号
	参数：
		dllVer：[out] 输出对应的版本号信息，缓冲区大小至少25字节
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY GetDllVersion(char *dllVer);

/*
	获取设备序列号
	参数：
		SN：[out] 输出设备序列号，缓冲区大小至少30字节
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY GetDeviceSN(char *SN);

/*
	设备复位
	参数：
		mode：[in] 复位模式：
			0 － 整机复位，1 － 发卡模块，2 － 卡库，3 － 滑动器，4 － 小车读卡器，5 －读写读卡器，
			6 － 通道读卡器， 7 － 打印模块，8 － 凸烫模块，9 －单烫模块，10 - 扫描仪，11 - 310读卡器
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY ResetDevices(int mode);

/*
获取当前连接的打印机类型
	参数：
		nType:[out] 输出打印机类型   0，无打印机连接   1，赛尔瑞直印机   2，爱丽丝转印机   3，玛迪卡转印机   4，得实转印机  5，法高转印机
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY GetDevicePrinterType(int *nType);

/*
	查询设备状态
	参数：
		dStatus：[out] 设备状态，缓冲区大小至少32字节
			dStatus[0]    设备状态：0 － 正常，1 - 忙碌，2 － 发卡模块异常，3 - 卡库异常，4 - 滑动器异常，5 - 小车读卡器异常，6 - 读写读卡器异常，
			7 - 通道读卡器异常，8 - 平印模块异常，9 - 凸烫模块异常，10 - 单烫模块异常， 11 - 扫描仪异常， 12 - 310读卡器异常
			dStatus[1]    发卡箱1状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			dStatus[2]    发卡箱2状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			dStatus[3]    发卡箱3状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			dStatus[4]    发卡箱4状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			dStatus[5]    发卡箱5状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			dStatus[6]    发卡箱6状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			dStatus[7]    发卡箱滑动器状态：0 － 无卡，1 － 卡片在两端，2 － 卡片在滑动器内, 3 － 未放置，4 － N/A
			dStatus[8]    小车读卡器状态：0 － 无卡，1 － 有卡，2 － 有错误
			dStatus[9]    废卡箱状态： 0 － 无卡/未满，1 － 有卡，2 － 回收箱满，3 － 未放置
			dStatus[10]    清分箱状态： 0 － 无卡/未满，1 － 有卡，2 － 回收箱满，3 － 未放置
			dStatus[11]   卡库状态： 0 － 正常，1 － 有错误
			dStatus[12]   凸烫模块状态： 0 － 正常，1 － 有卡，2 - 有错误
			dStatus[13]   单烫模块状态： 0 － 正常，1 － 有卡，2 - 有错误
			dStatus[14]   读写读卡器状态： 0 － 正常，1 － 有卡，2 - 有错误
			dStatus[15]   打印机状态：0 － 就绪，1 － 打印机出错
			dStatus[16]   出卡嘴是否有卡：0 - 无卡， 1 - 有卡，2 - 未知
			dStatus[17]   通道状态：0 - 正常，1 - 有卡，2 - 有错误
			dStatus[18]   烫金带状态：0 - 正常，1 - 快用完，2 - 已用完，3 - 未装烫金盒， 4 - 未知
			dStatus[19]   烫银带状态：0 - 正常，1 - 快用完，2 - 已用完，3 - 未装烫银盒， 4 - 未知
			dStatus[20]   凹字带状态：0 - 正常，1 - 凹字带少，2 - 未知
			dStatus[21] ~ [22]  打印色带余量：dStatus[21]*256 + dStatus[22]
			dStatus[23] ~ [24]	覆膜带余量，转印机特有，直印可忽略：dStatus[23]*256 + dStatus[24]
			dStatus[25]   扫描仪状态 0 － 无卡，1 － 有卡，2 － 有错误
			dStatus[26]   310读卡器状态：0 － 无卡，1 － 有卡，2 － 有错误
	返回值：
		0 - 成功；>0 - 失败，见错误码
*/
CARDPROCESS_API int APIENTRY GetDeviceStatus(BYTE *dStatus);
 
/*
	查询打印模块状态
	参数：
		mStatus：[out] 模块状态，缓冲区大小至少8字节
		mStatus[0]   0 － 就绪，1 － 打印机出错
		mStatus[1] ~ mStatus[2]	色带余量：mStatus[1]*256 + mStatus[2]
		mStatus[3] ~ mStatus[4]	覆膜带余量：mStatus[3]*256 + mStatus[4]
	返回值：
		0 - 成功，>0 - 失败
  */
CARDPROCESS_API int APIENTRY GetPrinterStatus(BYTE *mStatus);

/*
	查询发卡模块、滑动器、回收箱、卡库、读卡器及通道状态
	参数：
		mStatus：[out] 设备状态，缓冲区大小至少16字节
			mStatus[0]    设备状态：0 － 正常，非0 - 模块异常，需要对模块进行复位清除错误，
							1 发卡箱异常，2 滑动器异常，3 卡库异常，4 读写读卡器异常，5 通道异常，6 小车读卡器异常,
							7 扫描仪异常，8 310读卡器异常
			mStatus[1]    发卡箱1状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			mStatus[2]    发卡箱2状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			mStatus[3]    发卡箱3状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			mStatus[4]    发卡箱4状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			mStatus[5]    发卡箱5状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			mStatus[6]    发卡箱6状态： 0 － 无卡，1 － 卡少，2 － 卡足，3 － 未放置，4 － N/A
			dStatus[7]    发卡箱滑动器状态：0 － 无卡，1 － 卡片在两端，2 － 卡片在滑动器内, 3 － 未放置，4 － N/A
			dStatus[8]    小车读卡器状态：0 － 无卡，1 － 有卡，2 － 有错误
			mStatus[9]    废卡箱状态： 0 － 无卡/未满，1 － 有卡，2 － 回收箱满，3 － 未放置
			mStatus[10]    清分箱状态： 0 － 无卡/未满，1 － 有卡，2 － 回收箱满，3 － 未放置
			mStatus[11]   卡库状态： 0 － 正常，1 － 有错误
			mStatus[12]   读写读卡器状态： 0 － 正常，1 － 有卡，2 - 有错误
			mStatus[13]   通道状态：0 - 正常，1 - 有卡，2 - 有错误
			mStatus[14]   出卡嘴是否有卡：0 - 无卡， 1 - 有卡，2 - 未知
			dStatus[15]   扫描仪状态 0 － 无卡，1 － 有卡，2 － 有错误
			dStatus[16]   310读卡器状态：0 － 无卡，1 － 有卡，2 － 有错误
	返回值：
		0 - 成功，>0 - 失败
  */
CARDPROCESS_API int APIENTRY GetCommonStatus(BYTE *mStatus);

/*
	从发卡箱至发卡模块滑动器
	参数：	
		boxNo：[in] 卡箱号：取值范围1~6
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardBoxToEncoderSlider(int boxNo);

/*
	从发卡滑动器发卡至小车读卡器
	参数：	
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardEncoderSliderToSlider();
///////////////////////////////////////////////////////////////////////////////////////

/*
	从发卡箱发卡至小车
	参数：	
		boxNo：[in] 卡箱号：取值范围1~6
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToSlider(int boxNo);


/*
	从滑动器移卡至310读卡器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardTo310Reader();

/*
	从310读卡器移卡至滑动器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCard310ReaderToSlider();

/*
	从滑动器移卡至打印机（走位到打印机位置，并送卡，调用接口前应该让打印机进卡口转动）
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToPrinter();

/*
	获取卡库某个盘的仓位存卡状态
	参数：
		diskNo: [in] 盘号，500张有两个盘，1和2 
		slots：[out] 输出所选盘仓位有卡/无卡状态
		len：[out] 仓位存卡数，默认250
	返回值:
		0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY GetStorageSlots(int diskNo, BYTE slots[], int *len);

/*
	读卡库某个盘仓位存储的卡号
	参数：
		diskNo: [in] 盘号，500张有两个盘，1和2 
		slotNo：[in] 仓位号，1 ~ 250
		cardInfo：[out] 仓位对应的卡号信息
	返回值:
		0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY ReadStorageSlotCardNo(int diskNo, int slotNo, char *cardInfo);

/*
	向卡库某个盘仓位写入卡号信息
	参数：
		diskNo: [in] 盘号，500张有两个盘，1和2 
		slotNo：[in] 仓位号，1 ~ 250
		cardInfo：[in] 卡号，长度最大32
	返回值:
		0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY WriteStorageSlotCardNo(int diskNo, int slotNo, char *cardInfo);

/*
	从滑动器移卡至卡库
	参数：	
		diskNo：[in] 盘号		
		slotNo：[in] 仓位号，取值范围1~250
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToStorage(int diskNo, int slotNo);

/*
	从卡库取卡至滑动器
	参数：	
		diskNo：[in] 盘号		
		slotNo：[in] 仓位号，取值范围1~250
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardStorage2Slider(int diskNo, int slotNo);

/*
	自动存卡(自动寻找空仓位)
	参数：
		diskNo: [out] 盘号  为0时表示卡库存满
		slotNo：[out] 随机输出空仓位
	返回值:
		0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY MtAutoSaveCardPos(int *diskNo, int *slotNo);

/*
	获取卡库传感器状态
	参数：
		sensors: [out] 各传感器的使能状态
		count：[out] 传感器个数
	返回值:
		0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY MtGetStorageSensors(BYTE sensors[], int *count);

/*
	从滑动器移卡至废卡箱
	参数：
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToWasteCardBox();

/*
	从滑动器移卡至清分箱
	参数：
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToClearingBox();

/*
	从滑动器移卡至读写读卡器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToRWReader();

/*
	从读写读卡器移卡至滑动器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardRWReader2Slider();

/*
	从读写读卡器移卡至出卡口
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardRWReader2Exit();

/*
	将出卡口的卡片重进到读写读卡器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardExit2RWReader();

/*
	自动检测小车上的 IC 卡类型
	参数：
	nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
	cardType：[out] 射频卡类型：	
	0,  卡不在允许操作的位置上
	1,  未知卡型
	2,  非接触式射频卡(S50 射频卡)
	3,  非接触式 S70 射频卡
	4,  非接触式 UL 射频卡
	5,  ISO1443 Type A CPU 卡
	6,  ISO1443 Type B CPU 卡
	7,  接触式 T=0 的 CPU  卡
	8,  接触式 T=1 的 CPU  卡
	9,	24C01 卡
	10, 24C02 卡
	11, 24C04 卡
	12, 24C08 卡
	13,	24C16 卡
	14,	24C32 卡
	15,	24C64 卡
	返回值：
		0 - 成功，>0 - 失败
*/
CARDPROCESS_API int APIENTRY CheckCardType310_004(int nReaderNo, int *cardType);
///////////////////////////////////////////////////////////////////////////////////////

/*
读卡器指定方式读磁卡
	参数：	
		nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
		pTrack1：[out]  缓冲区地址，接收磁道1数据，可以为NULL
		pTrack2：[out]  缓冲区地址，接收磁道2数据，可以为NULL
		pTrack3：[out]  缓冲区地址，接收磁道3数据，可以为NULL
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY ReadTrack310_004(int nReaderNo, char *pTrack1, char *pTrack2, char *pTrack3);
///////////////////////////////////////////////////////////////////////////////////////


/*
	寻射频卡上电复位
	参数：		
		nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
		nCardType[in] 卡类型通过CheckCardType310_004检测卡类型，    2,  非接触式射频卡(S50 射频卡)   3,  非接触式 S70 射频卡   4,  非接触式 UL 射频卡
																	5,  ISO1443 Type A CPU 卡		 6,  ISO1443 Type B CPU 卡
		pATRData：[out] 输出寻卡成功返回的ATR数据   0，1，2 类型卡没有ATR数据
		ATRLen：[out] 输出寻卡成功返回ATR数据的长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RFCardActive310_004(int nReaderNo, int nCardType, BYTE *pATRData, int *ATRLen);
///////////////////////////////////////////////////////////////////////////////////////


/*
	射频卡下电释放
	参数：无
	返回值：
		nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RFCardDeactive310_004(int nReaderNo);
///////////////////////////////////////////////////////////////////////////////////////

/*
	执行APDU命令
	参数：
		nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
		pSendData： [in] 需要发送的APDU命令 
		sendLen：[in] 需要发送的APDU命令的长度，最大长度263byte
		pRcvData：[out] 输出APDU通讯后返回的数据
		rcvLen：[out] 输出APDU通讯后返回数据的长度，最大长度263byte
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RFCardSendAPDU310_004(int nReaderNo, BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);
///////////////////////////////////////////////////////////////////////////////////////

/*
	接触式IC卡 上电、下电
	参数：	
		nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
		nPowerNo [in]  1,上电  0,下电
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY IcCardPowerOnOff310_004(int nReaderNo, int nPowerNo);
///////////////////////////////////////////////////////////////////////////////////////

/*
	IC卡复位
	参数：
		nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
		nTypeReset[in] 复位方式， 0，冷复位   1，热复位
		ATRData：[out] 复位返回的ATR数据
		len：[out] ATR数据长度
		nType:  [out] 返回卡类型  0，T=0 CPU 卡  1，T=1 CPU 卡  2，复位不成功  3，卡机无卡  4，卡不在允许操作的位置上
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY ICCardReset310_004(int nReaderNo, int nTypeReset, BYTE *ATRData, int *len, int *nType);
///////////////////////////////////////////////////////////////////////////////////////

/*
	IC卡执行APDU命令
	参数：	
		nReaderNo: [in] 读卡器类型，0，小车读卡器   1，310读卡器
		nCardType[in]   卡类型 0、1，通过调用ICCardReset310_004，返回卡类型 0，T=0 CPU 卡  1，T=1 CPU 卡
		pSendData： [in] 需要发送的APDU命令      最大值为 262byte
		sendLen：[in] 需要发送的APDU命令的长度
		pRcvData：[out] 输出APDU通讯后返回的数据  最大值 257byte
		rcvLen：[out] 输出APDU通讯后返回数据的长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY ICCardSendAPDU310_004(int nReaderNo, int nCardType, BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);
///////////////////////////////////////////////////////////////////////////////////////

/*
	读磁条卡（读写读卡器）
	参数：	
		pTrack1：[out] 缓冲区地址，接收磁道1数据，可以为NULL
		pTrack2：[out] 缓冲区地址，接收磁道2数据，可以为NULL
		pTrack3：[out] 缓冲区地址，接收磁道3数据，可以为NULL
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWReadTrack(char *pTrack1, char *pTrack2, char *pTrack3);

/*
	写磁条卡
	参数：	
		mode： [in] 0表示自动选择，1表示以低抗写磁，2表示以高抗写磁
		pTrack1：[in] 缓冲区地址，设置磁道1要写入的数据，没有写入数据设NULL
		pTrack2：[in] 缓冲区地址，设置磁道2要写入的数据，没有写入数据设NULL
		pTrack3：[in] 缓冲区地址，设置磁道3要写入的数据，没有写入数据设NULL
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWWriteTrack(int mode, char *pTrack1, char *pTrack2, char *pTrack3);

/*
	SAM激活（读写读卡器）
	参数：
		nSel:[in] 选择SAM卡，0，SAM1   1，SAM2    2，SAM3    3，SAM4
		ATRData：[out] 激活返回的ATR数据
		len：[out] ATR数据长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWSAMCardPowerOn(int nSel, BYTE *ATRData, int *len);

/*
	SAM卡热复位（读写读卡器）
	参数：
		ATRData：[out] 激活返回的ATR数据
		len：[out] ATR数据长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWSAMCardHotReset(BYTE *ATRData, int *len);

/*
	SAM卡下电释放（读写读卡器）
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWSAMCardPowerOff();

/*
	SAM卡执行APDU命令（读写读卡器）
	参数：	
		pSendData： [in] 需要发送的APDU命令
		sendLen：[in] 需要发送的APDU命令的长度
		pRcvData：[out] 输出APDU通讯后返回的数据
		rcvLen：[out] 输出APDU通讯后返回数据的长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWSAMSendAPDU(BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);

/*
	IC卡上电激活（读写读卡器）
	参数：
		ATRData：[out] 激活返回的ATR数据
		len：[out] ATR数据长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWICCardPowerOn(BYTE *ATRData, int *len);

/*
	IC卡热复位（读写读卡器）
	参数：
		ATRData：[out] 激活返回的ATR数据
		len：[out] ATR数据长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWICCardHotReset(BYTE *ATRData, int *len);


/*
	IC卡下电释放（读写读卡器）
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWICCardPowerOff();


/*
	IC卡执行APDU命令（读写读卡器）
	参数：	
		pSendData： [in] 需要发送的APDU命令
		sendLen：[in] 需要发送的APDU命令的长度
		pRcvData：[out] 输出APDU通讯后返回的数据
		rcvLen：[out] 输出APDU通讯后返回数据的长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWICSendAPDU(BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);

/*
	IC卡读57标签值（读写读卡器）
	参数：	
		tagData： [out] 返回的57标签值数据
		tagLen：[out] 57标签值数据的长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWICRead57Tag(char *tagData, int *tagLen);

/*
	射频卡复位激活（读写读卡器）
	参数：
		pRespData：[out] 输出激活成功返回的数据
		RespLen：[out] 输出激活成功返回数据的长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWRFCardActive(BYTE *pRespData, int *RespLen);

/*
	射频卡下电释放（读写读卡器）
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWRFCardDeactive();


/*
	RF卡执行APDU命令（读写读卡器）
	参数：	
		pSendData： [in] 需要发送的APDU命令
		sendLen：[in] 需要发送的APDU命令的长度
		pRcvData：[out] 输出APDU通讯后返回的数据
		rcvLen：[out] 输出APDU通讯后返回数据的长度
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY RWRFSendAPDU(BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);


/*
	获取设备错误详情
	参数：
		errNo：[out] 输出设备统一错误代码
		errMsg：[out] 错误信息
	返回值：
		0 - 成功；>0 - 失败
  */
CARDPROCESS_API int APIENTRY GetDeviceErrInfo(DWORD *errNo, TCHAR *errMsg);

/*
	获取读卡器通道卡的状态
	参数：
		cardState：[out] 通道卡片状态，0 - 无卡，1 - 卡在出卡嘴，2 - 卡在通道内，3 - 未知
	返回值：
		0 - 成功，>0 - 失败
*/
CARDPROCESS_API int APIENTRY GetPassageCardStatus(int *cardState);

/*
	将通道内的卡移到出卡嘴
	参数：无
	返回值：
		0 - 成功，>0 - 失败
*/
CARDPROCESS_API int APIENTRY MoveCardPass2Exit();

/*
	将通道内的卡移到读写读卡器	
	参数：无
	返回值：
		0 - 成功，>0 - 失败
*/
CARDPROCESS_API int APIENTRY MoveCardPass2RWReader();


/*
	设置读卡器前端进卡方式
	参数：
		way：[in] 前端进卡设置，
			0 - 禁止前端进卡
			1 - 前端进卡不检测磁信号
			2 - 前端进卡检测磁信号
			3 - 使能前端进IC卡
			4 - 使能前端进IC卡&磁卡（必须同时有磁道和IC芯片）
			5 - 使能前端进IC卡或磁卡（磁道和IC芯片有一个即可）	
	返回值：
		0 - 成功，>0 - 失败
*/
CARDPROCESS_API int APIENTRY SetFrontCardInMode(int way);


/*
	设置前端抖动进卡（仅对磁卡有效）
	参数：	
		enable：[in]控制是否抖动进卡，0 - 不抖动，1 - 抖动
	返回值：
		0 - 成功，>0 - 失败
*/
CARDPROCESS_API int APIENTRY SetJitterCardEnable(int enable);

///////////////////////////////////////////////////////////////////////////////////////非必要接口

/*
	打印机读卡器卡复位
	参数：
		cardType: [in] 卡类型   1, 非接射频卡(typeA, typeB)  2,接触IC卡(T=0,T=1)
		pRespData：[out] 输出复位成功返回的数据
		RespLen：[out] 输出复位成功返回数据的长度
	返回值:
		0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY SetPrinterReaderCardReset(int cardType, BYTE *pRespData, int *RespLen);

/*
	打印机读卡器执行APDU命令
	参数：
		cardType: [in] 卡类型   1, 非接射频卡(typeA, typeB)  2,接触IC卡(T=0,T=1)
		pSendData： [in] 需要发送的APDU命令
		sendLen：[in] 需要发送的APDU命令的长度
		pRcvData：[out] 输出APDU通讯后返回的数据
		rcvLen：[out] 输出APDU通讯后返回数据的长度
	返回值:
		0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY MtPrinterReaderCardApdu(int cardType, BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);

/*
	打印卡（执行完后卡回到小车）
	参数：	
		frontImgProp：[in] 正面图像
		backImgProp：[in] 背面图像，传入NULL不打印背面
		pInDocProp：[in] 打印配置，传入NULL会使用默认打印首选项
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY PrintCard(CardImageProp *frontImgProp, CardImageProp *backImgProp, DocProp *pInDocProp);


/*
	从打印机模块移卡至滑动器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardPrinter2Slider();

/*
	移动卡：滑动器 -> 读写读卡器
*/
CARDPROCESS_API int APIENTRY MtMoveCardSlider2RW();

/*
	移动卡：滑动器 <- 读写读卡器
*/
CARDPROCESS_API int APIENTRY MtMoveCardSlider4RW();

/*
	移动卡：读写读卡器 -> 通道
*/
CARDPROCESS_API int APIENTRY MtMoveCardRW2Pass();

/*
	移动卡：通道 <- 出卡嘴
*/
CARDPROCESS_API int APIENTRY MtMoveCardPass4Exit();


/*
	滑动器移位
	SliderMoveZero = 0,         // 原点（复位位置）
	SliderMoveEncoderBox1 = 1,  // 卡箱#1
	SliderMoveEncoderBox2 = 2,  // 卡箱#2
	SliderMoveEncoderBox3 = 3,  // 卡箱#3
	SliderMoveReader = 4,       // 读卡器
	SliderMoveStoreDisk1 = 5,   // 卡库1盘
	SliderMoveStoreDisk2 = 6,   // 卡库2盘
	SliderMoveHTPrinter = 7,      //法高打印机
	SliderMoveEmboss = 8,       // 凸烫
	SliderMoveHot = 9,          // 单烫
	SliderMoveBin1F1 = 10,    // 回收箱
	SliderMoveBin2F1 = 11,    // 清分箱
	SliderMovePrinter = 12,   // 赛尔瑞直印机
	SliderMoveScanner = 13,   // 扫描仪
	SliderMoveSpace = 14,       //空闲位置
	SliderMove310 = 15,       //310读卡器
*/
CARDPROCESS_API int APIENTRY MtMoveSliderTo(int pos);

/*
	调整滑动器走位偏差
	参数：
		xy：[in] 表示左右或者升降方向，1 - 左右方向，即X轴，2 - 升降方向，即Y轴
		pos：[in] 位置代码，X轴或者Y轴定义的位置
		forward：[in] 是否正向，X轴向右为正向（X轴复位后归零点，只可向右移动），Y轴向上为正向
		offset：[out] 偏移值，默认为0x40
*/
CARDPROCESS_API int APIENTRY MtModifySliderDeviation(int xy, int pos, bool forward, BYTE *offset); 

/*
	读滑动器走位偏差
*/
CARDPROCESS_API int APIENTRY MtReadSliderDeviation(int xy, int pos, BYTE *offset); 

/*
	卡库解锁
*/
CARDPROCESS_API int APIENTRY MtStorageUnlock();

/*
	读卡库偏移
	type    [in]  0，读卡库零位偏移    1，读卡库1盘偏移     2，读卡库2盘偏移
	offset：[out] 偏移值
	返回值:
			0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY MtReadStorageDeviation(int type, BYTE *offset);

/*
	调整卡库偏移
	参数：
	type    [in]  0，卡库零位偏移    1，卡库1盘偏移     2，卡库2盘偏移
	nDU     [in]  0，偏移+    1，偏移-
	offset：[out] 偏移值
	返回值:
			0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY MtModifyStorageDeviation(int type, int nDU, BYTE *offset); 

/*
	设置卡库盘点方式
	参数：
	type    [in]  0，盘点方式0    1，盘点方式1
	返回值:
			0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY MtSetStorageInitType(int type); 

/*
	查询卡库仓位模拟值
		diskNo: [in] 盘号，500张有两个盘，1和2 
		slots：[out] 输出所选盘仓位模拟信号
		len：[out] 仓位存卡数，默认250
	返回值:
			0 - 成功，> 0 失败
*/
CARDPROCESS_API int APIENTRY MtQueryStorageSignal(int diskNo, BYTE slots[], int *len); 

///////////////////////////////////////////////////////////////////////////////////////内部调试接口

// 调试小车移动
CARDPROCESS_API int APIENTRY DebugSliderMove(int mover, int direction, int steps);


// 滑动器复位 1 - 左右滑动器，2 - 上下滑动器，3 - 联动复位
CARDPROCESS_API int APIENTRY DebugSliderReset(int which);

/*
	查询滑动器传感器状态
	参数：
		mStatus：[out] 设备状态，缓冲区大小至少9字节
			mStatus[0]    滑动器左
			mStatus[1]    滑动器右
			mStatus[2]    滑动器上
			mStatus[3]    滑动器下
			mStatus[4]    卡箱1未放置
			mStatus[5]    卡箱1空
			mStatus[6]    卡箱1满
			dStatus[7]    卡箱2未放置
			dStatus[8]    卡箱2空
			dStatus[9]    卡箱2满
			dStatus[10]   0 － 正常，非0 - 异常
	返回值：
		0 - 成功，>0 - 失败
  */
CARDPROCESS_API int APIENTRY GetSliderStatus(BYTE *mStatus);


/*
	移动发卡箱滑动器
	参数：	
		boxNo：[in] 卡箱号：取值范围1~3
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardBoxSlider(int boxNo);
///////////////////////////////////////////////////////////////////////////////////////


/*
	设备模块发送自定义指令
	参数：
		module：[in] 模块
		buffSend：[in] 命令数据
		iSendLen：[in] 命令数据长度
		buffRcv：[out] 应答数据
		iRcvLen：[out] 应答数据长度
	返回值：
		0 - 成功， >0 - 失败
*/
CARDPROCESS_API int APIENTRY DebugExecuteCmd(int module, BYTE *buffSend, int iSendLen, BYTE *buffRcv, int *iRcvLen);


/*
	查找设备模块
*/
CARDPROCESS_API int APIENTRY DebugFindModules(int module);


/*
	扫描卡
	参数：	
		frontImg[in] 是否保存正面图像     false，不保存    true，保存
		backImg[in] 是否保存背面图像
		cutImg[in] 是否保存剪切图像
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY ScanCard(bool frontImg, bool backImg, bool cutImg);


/*
	从扫描模块移卡至滑动器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardScanner2Slider();

/*
	从滑动器移卡至扫描仪
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToScanner();


/*
	查询凸烫模块状态
	参数：
		mStatus：[out]   模块状态，缓冲区大小至少5字节
			mStatus[0]   凸烫模块状态： 0 － 正常，1 － 有卡，2 - 有错误
			mStatus[1]   烫银模块状态： 0 － 正常，1 － 有卡，2 - 有错误
			mStatus[2]   烫金带状态：0 - 正常，1 - 快用完，2 - 已用完，3 - 未装烫金盒， 4 - 未知
			mStatus[3]   烫银带状态：0 - 正常，1 - 快用完，2 - 已用完，3 - 未装烫银盒， 4 - 未知
			mStatus[4]   凹字带状态：0 - 正常，1 - 凹字带少，2 - 未知
	返回值：
		0 - 成功，>0 - 失败
  */
CARDPROCESS_API int APIENTRY GetEmbossStatus(BYTE *mStatus);

/*
	从滑动器移卡至凸烫模块
	参数：	
		无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardToEmboss();

/*
	从凸烫模块移卡至滑动器
	参数：无
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY MoveCardEmbossToSlider();

/*
	凸烫卡（执行完后卡回到小车）
	参数：
		type：[in] 凸烫类型：0 - 仅凸字，1 - 凸字烫金， 2 - 凸字烫银	
		pEmbossProp：[in] 凸烫内容(包括坐标、字体、内容)
		size：[in] 有几组凸烫内容
	返回值：
		0 - 成功；>0 - 失败，见错误码
  */
CARDPROCESS_API int APIENTRY PlayEmboss(int type, EmbossProp *pEmbossProp, int size);

/*
	凸烫模块进卡
*/
CARDPROCESS_API int APIENTRY MtEmbossHotCardIn();


/*
	凸烫模块出卡
*/
CARDPROCESS_API int APIENTRY MtEmbossHotCardOut();


/*
	单烫模块进卡
*/
CARDPROCESS_API int APIENTRY MtSingleHotCardIn();


/*
	单烫模块出卡
*/
CARDPROCESS_API int APIENTRY MtSingleHotCardOut();


/*
	单烫模块烫色
*/
CARDPROCESS_API int APIENTRY MtExecHotSilver();


#ifdef __cplusplus
}
#endif