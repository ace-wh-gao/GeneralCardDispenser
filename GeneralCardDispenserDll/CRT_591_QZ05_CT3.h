/**
  * @file       CRT_591_QZ05_CT3.h
  
  * @author     �����д��Լ������޹�˾
  
  * @date       2020/03/09
  
  * @version    2, 0, 2, 309
  
  * @remarks    CRT_591_QZ05_CT3�ƿ�һ����ӿ��ļ�
  
  */
#include "DeviceDefine.h"

#ifdef CRT_591_QZ05_CT3_EXPORTS
#define CARDPROCESS_API __declspec(dllexport)
#else
#define CARDPROCESS_API __declspec(dllimport)
#endif


// ���ش����� 11 ��12ʱ������GetDeviceErrInfo()�ɻ�ȡ����ģ���ͳһ�����뼰��������

#ifdef __cplusplus
extern "C" {
#endif

/*
	���豸
	������
		config��[in]  �������ͣ�0 �� �Զ�����ģ����豸�� 1 �� �������ļ��м��ز������豸
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
*/
CARDPROCESS_API int APIENTRY OpenDevices(int config);


/*
	�ر��豸
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY CloseDevices();

/*
	�ͷŶ�̬�⣨����λ������˳�ʱ����ã�
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY ReleaseDll();

/*
	��־����
	����:
		level��0 - ��������־��1 - ������־
		path����־�洢·����Ϊ����Ĭ��·��
	����ֵ��
			��
  */
CARDPROCESS_API void APIENTRY SetTraceLog(int level, char *path);

/*
	�����豸�ڵĿ�
	������
	mode��[in] ����ģʽ��
			0 �� ���豸�ڵĿ�������Ͽ��䣬1 �� ���豸�ڵĿ�����������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY ClearDeviceCard(int mode);
///////////////////////////////////////////////////////////////////////////////////////

/*
	��ȡ�̼��汾��
	������
		index: [in] ģ��������	
				0 - ����ģ�飬��"#"�ָ���1 �� ����ģ�飬2 �� ���⣬3 �� С����4 �� С����������5 ����д��������
				6 �� ͨ���������� 7 �� ��ӡ����8 �� ͹��ģ�飬9 ������ģ��, 10 - ɨ���ǣ�11 - 310������
		firmware��[out] �����Ӧ�Ĺ̼��汾��Ϣ����������С����250�ֽ�
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY GetDeviceFWVersion(int index, char *firmware);

/*
	��ȡ��̬��汾��
	������
		dllVer��[out] �����Ӧ�İ汾����Ϣ����������С����25�ֽ�
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY GetDllVersion(char *dllVer);

/*
	��ȡ�豸���к�
	������
		SN��[out] ����豸���кţ���������С����30�ֽ�
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY GetDeviceSN(char *SN);

/*
	�豸��λ
	������
		mode��[in] ��λģʽ��
			0 �� ������λ��1 �� ����ģ�飬2 �� ���⣬3 �� ��������4 �� С����������5 ����д��������
			6 �� ͨ���������� 7 �� ��ӡģ�飬8 �� ͹��ģ�飬9 ������ģ�飬10 - ɨ���ǣ�11 - 310������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY ResetDevices(int mode);

/*
��ȡ��ǰ���ӵĴ�ӡ������
	������
		nType:[out] �����ӡ������   0���޴�ӡ������   1��������ֱӡ��   2������˿תӡ��   3����Ͽ�תӡ��   4����ʵתӡ��  5������תӡ��
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY GetDevicePrinterType(int *nType);

/*
	��ѯ�豸״̬
	������
		dStatus��[out] �豸״̬����������С����32�ֽ�
			dStatus[0]    �豸״̬��0 �� ������1 - æµ��2 �� ����ģ���쳣��3 - �����쳣��4 - �������쳣��5 - С���������쳣��6 - ��д�������쳣��
			7 - ͨ���������쳣��8 - ƽӡģ���쳣��9 - ͹��ģ���쳣��10 - ����ģ���쳣�� 11 - ɨ�����쳣�� 12 - 310�������쳣
			dStatus[1]    ������1״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			dStatus[2]    ������2״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			dStatus[3]    ������3״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			dStatus[4]    ������4״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			dStatus[5]    ������5״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			dStatus[6]    ������6״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			dStatus[7]    �����们����״̬��0 �� �޿���1 �� ��Ƭ�����ˣ�2 �� ��Ƭ�ڻ�������, 3 �� δ���ã�4 �� N/A
			dStatus[8]    С��������״̬��0 �� �޿���1 �� �п���2 �� �д���
			dStatus[9]    �Ͽ���״̬�� 0 �� �޿�/δ����1 �� �п���2 �� ����������3 �� δ����
			dStatus[10]    �����״̬�� 0 �� �޿�/δ����1 �� �п���2 �� ����������3 �� δ����
			dStatus[11]   ����״̬�� 0 �� ������1 �� �д���
			dStatus[12]   ͹��ģ��״̬�� 0 �� ������1 �� �п���2 - �д���
			dStatus[13]   ����ģ��״̬�� 0 �� ������1 �� �п���2 - �д���
			dStatus[14]   ��д������״̬�� 0 �� ������1 �� �п���2 - �д���
			dStatus[15]   ��ӡ��״̬��0 �� ������1 �� ��ӡ������
			dStatus[16]   �������Ƿ��п���0 - �޿��� 1 - �п���2 - δ֪
			dStatus[17]   ͨ��״̬��0 - ������1 - �п���2 - �д���
			dStatus[18]   �̽��״̬��0 - ������1 - �����꣬2 - �����꣬3 - δװ�̽�У� 4 - δ֪
			dStatus[19]   ������״̬��0 - ������1 - �����꣬2 - �����꣬3 - δװ�����У� 4 - δ֪
			dStatus[20]   ���ִ�״̬��0 - ������1 - ���ִ��٣�2 - δ֪
			dStatus[21] ~ [22]  ��ӡɫ��������dStatus[21]*256 + dStatus[22]
			dStatus[23] ~ [24]	��Ĥ��������תӡ�����У�ֱӡ�ɺ��ԣ�dStatus[23]*256 + dStatus[24]
			dStatus[25]   ɨ����״̬ 0 �� �޿���1 �� �п���2 �� �д���
			dStatus[26]   310������״̬��0 �� �޿���1 �� �п���2 �� �д���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
*/
CARDPROCESS_API int APIENTRY GetDeviceStatus(BYTE *dStatus);
 
/*
	��ѯ��ӡģ��״̬
	������
		mStatus��[out] ģ��״̬����������С����8�ֽ�
		mStatus[0]   0 �� ������1 �� ��ӡ������
		mStatus[1] ~ mStatus[2]	ɫ��������mStatus[1]*256 + mStatus[2]
		mStatus[3] ~ mStatus[4]	��Ĥ��������mStatus[3]*256 + mStatus[4]
	����ֵ��
		0 - �ɹ���>0 - ʧ��
  */
CARDPROCESS_API int APIENTRY GetPrinterStatus(BYTE *mStatus);

/*
	��ѯ����ģ�顢�������������䡢���⡢��������ͨ��״̬
	������
		mStatus��[out] �豸״̬����������С����16�ֽ�
			mStatus[0]    �豸״̬��0 �� ��������0 - ģ���쳣����Ҫ��ģ����и�λ�������
							1 �������쳣��2 �������쳣��3 �����쳣��4 ��д�������쳣��5 ͨ���쳣��6 С���������쳣,
							7 ɨ�����쳣��8 310�������쳣
			mStatus[1]    ������1״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			mStatus[2]    ������2״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			mStatus[3]    ������3״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			mStatus[4]    ������4״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			mStatus[5]    ������5״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			mStatus[6]    ������6״̬�� 0 �� �޿���1 �� ���٣�2 �� ���㣬3 �� δ���ã�4 �� N/A
			dStatus[7]    �����们����״̬��0 �� �޿���1 �� ��Ƭ�����ˣ�2 �� ��Ƭ�ڻ�������, 3 �� δ���ã�4 �� N/A
			dStatus[8]    С��������״̬��0 �� �޿���1 �� �п���2 �� �д���
			mStatus[9]    �Ͽ���״̬�� 0 �� �޿�/δ����1 �� �п���2 �� ����������3 �� δ����
			mStatus[10]    �����״̬�� 0 �� �޿�/δ����1 �� �п���2 �� ����������3 �� δ����
			mStatus[11]   ����״̬�� 0 �� ������1 �� �д���
			mStatus[12]   ��д������״̬�� 0 �� ������1 �� �п���2 - �д���
			mStatus[13]   ͨ��״̬��0 - ������1 - �п���2 - �д���
			mStatus[14]   �������Ƿ��п���0 - �޿��� 1 - �п���2 - δ֪
			dStatus[15]   ɨ����״̬ 0 �� �޿���1 �� �п���2 �� �д���
			dStatus[16]   310������״̬��0 �� �޿���1 �� �п���2 �� �д���
	����ֵ��
		0 - �ɹ���>0 - ʧ��
  */
CARDPROCESS_API int APIENTRY GetCommonStatus(BYTE *mStatus);

/*
	�ӷ�����������ģ�黬����
	������	
		boxNo��[in] ����ţ�ȡֵ��Χ1~6
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardBoxToEncoderSlider(int boxNo);

/*
	�ӷ���������������С��������
	������	
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardEncoderSliderToSlider();
///////////////////////////////////////////////////////////////////////////////////////

/*
	�ӷ����䷢����С��
	������	
		boxNo��[in] ����ţ�ȡֵ��Χ1~6
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToSlider(int boxNo);


/*
	�ӻ������ƿ���310������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardTo310Reader();

/*
	��310�������ƿ���������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCard310ReaderToSlider();

/*
	�ӻ������ƿ�����ӡ������λ����ӡ��λ�ã����Ϳ������ýӿ�ǰӦ���ô�ӡ��������ת����
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToPrinter();

/*
	��ȡ����ĳ���̵Ĳ�λ�濨״̬
	������
		diskNo: [in] �̺ţ�500���������̣�1��2 
		slots��[out] �����ѡ�̲�λ�п�/�޿�״̬
		len��[out] ��λ�濨����Ĭ��250
	����ֵ:
		0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY GetStorageSlots(int diskNo, BYTE slots[], int *len);

/*
	������ĳ���̲�λ�洢�Ŀ���
	������
		diskNo: [in] �̺ţ�500���������̣�1��2 
		slotNo��[in] ��λ�ţ�1 ~ 250
		cardInfo��[out] ��λ��Ӧ�Ŀ�����Ϣ
	����ֵ:
		0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY ReadStorageSlotCardNo(int diskNo, int slotNo, char *cardInfo);

/*
	�򿨿�ĳ���̲�λд�뿨����Ϣ
	������
		diskNo: [in] �̺ţ�500���������̣�1��2 
		slotNo��[in] ��λ�ţ�1 ~ 250
		cardInfo��[in] ���ţ��������32
	����ֵ:
		0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY WriteStorageSlotCardNo(int diskNo, int slotNo, char *cardInfo);

/*
	�ӻ������ƿ�������
	������	
		diskNo��[in] �̺�		
		slotNo��[in] ��λ�ţ�ȡֵ��Χ1~250
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToStorage(int diskNo, int slotNo);

/*
	�ӿ���ȡ����������
	������	
		diskNo��[in] �̺�		
		slotNo��[in] ��λ�ţ�ȡֵ��Χ1~250
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardStorage2Slider(int diskNo, int slotNo);

/*
	�Զ��濨(�Զ�Ѱ�ҿղ�λ)
	������
		diskNo: [out] �̺�  Ϊ0ʱ��ʾ�������
		slotNo��[out] �������ղ�λ
	����ֵ:
		0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY MtAutoSaveCardPos(int *diskNo, int *slotNo);

/*
	��ȡ���⴫����״̬
	������
		sensors: [out] ����������ʹ��״̬
		count��[out] ����������
	����ֵ:
		0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY MtGetStorageSensors(BYTE sensors[], int *count);

/*
	�ӻ������ƿ����Ͽ���
	������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToWasteCardBox();

/*
	�ӻ������ƿ��������
	������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToClearingBox();

/*
	�ӻ������ƿ�����д������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToRWReader();

/*
	�Ӷ�д�������ƿ���������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardRWReader2Slider();

/*
	�Ӷ�д�������ƿ���������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardRWReader2Exit();

/*
	�������ڵĿ�Ƭ�ؽ�����д������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardExit2RWReader();

/*
	�Զ����С���ϵ� IC ������
	������
	nReaderNo: [in] ���������ͣ�0��С��������   1��310������
	cardType��[out] ��Ƶ�����ͣ�	
	0,  ���������������λ����
	1,  δ֪����
	2,  �ǽӴ�ʽ��Ƶ��(S50 ��Ƶ��)
	3,  �ǽӴ�ʽ S70 ��Ƶ��
	4,  �ǽӴ�ʽ UL ��Ƶ��
	5,  ISO1443 Type A CPU ��
	6,  ISO1443 Type B CPU ��
	7,  �Ӵ�ʽ T=0 �� CPU  ��
	8,  �Ӵ�ʽ T=1 �� CPU  ��
	9,	24C01 ��
	10, 24C02 ��
	11, 24C04 ��
	12, 24C08 ��
	13,	24C16 ��
	14,	24C32 ��
	15,	24C64 ��
	����ֵ��
		0 - �ɹ���>0 - ʧ��
*/
CARDPROCESS_API int APIENTRY CheckCardType310_004(int nReaderNo, int *cardType);
///////////////////////////////////////////////////////////////////////////////////////

/*
������ָ����ʽ���ſ�
	������	
		nReaderNo: [in] ���������ͣ�0��С��������   1��310������
		pTrack1��[out]  ��������ַ�����մŵ�1���ݣ�����ΪNULL
		pTrack2��[out]  ��������ַ�����մŵ�2���ݣ�����ΪNULL
		pTrack3��[out]  ��������ַ�����մŵ�3���ݣ�����ΪNULL
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY ReadTrack310_004(int nReaderNo, char *pTrack1, char *pTrack2, char *pTrack3);
///////////////////////////////////////////////////////////////////////////////////////


/*
	Ѱ��Ƶ���ϵ縴λ
	������		
		nReaderNo: [in] ���������ͣ�0��С��������   1��310������
		nCardType[in] ������ͨ��CheckCardType310_004��⿨���ͣ�    2,  �ǽӴ�ʽ��Ƶ��(S50 ��Ƶ��)   3,  �ǽӴ�ʽ S70 ��Ƶ��   4,  �ǽӴ�ʽ UL ��Ƶ��
																	5,  ISO1443 Type A CPU ��		 6,  ISO1443 Type B CPU ��
		pATRData��[out] ���Ѱ���ɹ����ص�ATR����   0��1��2 ���Ϳ�û��ATR����
		ATRLen��[out] ���Ѱ���ɹ�����ATR���ݵĳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RFCardActive310_004(int nReaderNo, int nCardType, BYTE *pATRData, int *ATRLen);
///////////////////////////////////////////////////////////////////////////////////////


/*
	��Ƶ���µ��ͷ�
	��������
	����ֵ��
		nReaderNo: [in] ���������ͣ�0��С��������   1��310������
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RFCardDeactive310_004(int nReaderNo);
///////////////////////////////////////////////////////////////////////////////////////

/*
	ִ��APDU����
	������
		nReaderNo: [in] ���������ͣ�0��С��������   1��310������
		pSendData�� [in] ��Ҫ���͵�APDU���� 
		sendLen��[in] ��Ҫ���͵�APDU����ĳ��ȣ���󳤶�263byte
		pRcvData��[out] ���APDUͨѶ�󷵻ص�����
		rcvLen��[out] ���APDUͨѶ�󷵻����ݵĳ��ȣ���󳤶�263byte
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RFCardSendAPDU310_004(int nReaderNo, BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);
///////////////////////////////////////////////////////////////////////////////////////

/*
	�Ӵ�ʽIC�� �ϵ硢�µ�
	������	
		nReaderNo: [in] ���������ͣ�0��С��������   1��310������
		nPowerNo [in]  1,�ϵ�  0,�µ�
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY IcCardPowerOnOff310_004(int nReaderNo, int nPowerNo);
///////////////////////////////////////////////////////////////////////////////////////

/*
	IC����λ
	������
		nReaderNo: [in] ���������ͣ�0��С��������   1��310������
		nTypeReset[in] ��λ��ʽ�� 0���临λ   1���ȸ�λ
		ATRData��[out] ��λ���ص�ATR����
		len��[out] ATR���ݳ���
		nType:  [out] ���ؿ�����  0��T=0 CPU ��  1��T=1 CPU ��  2����λ���ɹ�  3�������޿�  4�����������������λ����
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY ICCardReset310_004(int nReaderNo, int nTypeReset, BYTE *ATRData, int *len, int *nType);
///////////////////////////////////////////////////////////////////////////////////////

/*
	IC��ִ��APDU����
	������	
		nReaderNo: [in] ���������ͣ�0��С��������   1��310������
		nCardType[in]   ������ 0��1��ͨ������ICCardReset310_004�����ؿ����� 0��T=0 CPU ��  1��T=1 CPU ��
		pSendData�� [in] ��Ҫ���͵�APDU����      ���ֵΪ 262byte
		sendLen��[in] ��Ҫ���͵�APDU����ĳ���
		pRcvData��[out] ���APDUͨѶ�󷵻ص�����  ���ֵ 257byte
		rcvLen��[out] ���APDUͨѶ�󷵻����ݵĳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY ICCardSendAPDU310_004(int nReaderNo, int nCardType, BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);
///////////////////////////////////////////////////////////////////////////////////////

/*
	������������д��������
	������	
		pTrack1��[out] ��������ַ�����մŵ�1���ݣ�����ΪNULL
		pTrack2��[out] ��������ַ�����մŵ�2���ݣ�����ΪNULL
		pTrack3��[out] ��������ַ�����մŵ�3���ݣ�����ΪNULL
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWReadTrack(char *pTrack1, char *pTrack2, char *pTrack3);

/*
	д������
	������	
		mode�� [in] 0��ʾ�Զ�ѡ��1��ʾ�ԵͿ�д�ţ�2��ʾ�Ը߿�д��
		pTrack1��[in] ��������ַ�����ôŵ�1Ҫд������ݣ�û��д��������NULL
		pTrack2��[in] ��������ַ�����ôŵ�2Ҫд������ݣ�û��д��������NULL
		pTrack3��[in] ��������ַ�����ôŵ�3Ҫд������ݣ�û��д��������NULL
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWWriteTrack(int mode, char *pTrack1, char *pTrack2, char *pTrack3);

/*
	SAM�����д��������
	������
		nSel:[in] ѡ��SAM����0��SAM1   1��SAM2    2��SAM3    3��SAM4
		ATRData��[out] ����ص�ATR����
		len��[out] ATR���ݳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWSAMCardPowerOn(int nSel, BYTE *ATRData, int *len);

/*
	SAM���ȸ�λ����д��������
	������
		ATRData��[out] ����ص�ATR����
		len��[out] ATR���ݳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWSAMCardHotReset(BYTE *ATRData, int *len);

/*
	SAM���µ��ͷţ���д��������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWSAMCardPowerOff();

/*
	SAM��ִ��APDU�����д��������
	������	
		pSendData�� [in] ��Ҫ���͵�APDU����
		sendLen��[in] ��Ҫ���͵�APDU����ĳ���
		pRcvData��[out] ���APDUͨѶ�󷵻ص�����
		rcvLen��[out] ���APDUͨѶ�󷵻����ݵĳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWSAMSendAPDU(BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);

/*
	IC���ϵ缤���д��������
	������
		ATRData��[out] ����ص�ATR����
		len��[out] ATR���ݳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWICCardPowerOn(BYTE *ATRData, int *len);

/*
	IC���ȸ�λ����д��������
	������
		ATRData��[out] ����ص�ATR����
		len��[out] ATR���ݳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWICCardHotReset(BYTE *ATRData, int *len);


/*
	IC���µ��ͷţ���д��������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWICCardPowerOff();


/*
	IC��ִ��APDU�����д��������
	������	
		pSendData�� [in] ��Ҫ���͵�APDU����
		sendLen��[in] ��Ҫ���͵�APDU����ĳ���
		pRcvData��[out] ���APDUͨѶ�󷵻ص�����
		rcvLen��[out] ���APDUͨѶ�󷵻����ݵĳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWICSendAPDU(BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);

/*
	IC����57��ǩֵ����д��������
	������	
		tagData�� [out] ���ص�57��ǩֵ����
		tagLen��[out] 57��ǩֵ���ݵĳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWICRead57Tag(char *tagData, int *tagLen);

/*
	��Ƶ����λ�����д��������
	������
		pRespData��[out] �������ɹ����ص�����
		RespLen��[out] �������ɹ��������ݵĳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWRFCardActive(BYTE *pRespData, int *RespLen);

/*
	��Ƶ���µ��ͷţ���д��������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWRFCardDeactive();


/*
	RF��ִ��APDU�����д��������
	������	
		pSendData�� [in] ��Ҫ���͵�APDU����
		sendLen��[in] ��Ҫ���͵�APDU����ĳ���
		pRcvData��[out] ���APDUͨѶ�󷵻ص�����
		rcvLen��[out] ���APDUͨѶ�󷵻����ݵĳ���
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY RWRFSendAPDU(BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);


/*
	��ȡ�豸��������
	������
		errNo��[out] ����豸ͳһ�������
		errMsg��[out] ������Ϣ
	����ֵ��
		0 - �ɹ���>0 - ʧ��
  */
CARDPROCESS_API int APIENTRY GetDeviceErrInfo(DWORD *errNo, TCHAR *errMsg);

/*
	��ȡ������ͨ������״̬
	������
		cardState��[out] ͨ����Ƭ״̬��0 - �޿���1 - ���ڳ����죬2 - ����ͨ���ڣ�3 - δ֪
	����ֵ��
		0 - �ɹ���>0 - ʧ��
*/
CARDPROCESS_API int APIENTRY GetPassageCardStatus(int *cardState);

/*
	��ͨ���ڵĿ��Ƶ�������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ��
*/
CARDPROCESS_API int APIENTRY MoveCardPass2Exit();

/*
	��ͨ���ڵĿ��Ƶ���д������	
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ��
*/
CARDPROCESS_API int APIENTRY MoveCardPass2RWReader();


/*
	���ö�����ǰ�˽�����ʽ
	������
		way��[in] ǰ�˽������ã�
			0 - ��ֹǰ�˽���
			1 - ǰ�˽����������ź�
			2 - ǰ�˽��������ź�
			3 - ʹ��ǰ�˽�IC��
			4 - ʹ��ǰ�˽�IC��&�ſ�������ͬʱ�дŵ���ICоƬ��
			5 - ʹ��ǰ�˽�IC����ſ����ŵ���ICоƬ��һ�����ɣ�	
	����ֵ��
		0 - �ɹ���>0 - ʧ��
*/
CARDPROCESS_API int APIENTRY SetFrontCardInMode(int way);


/*
	����ǰ�˶������������Դſ���Ч��
	������	
		enable��[in]�����Ƿ񶶶�������0 - ��������1 - ����
	����ֵ��
		0 - �ɹ���>0 - ʧ��
*/
CARDPROCESS_API int APIENTRY SetJitterCardEnable(int enable);

///////////////////////////////////////////////////////////////////////////////////////�Ǳ�Ҫ�ӿ�

/*
	��ӡ������������λ
	������
		cardType: [in] ������   1, �ǽ���Ƶ��(typeA, typeB)  2,�Ӵ�IC��(T=0,T=1)
		pRespData��[out] �����λ�ɹ����ص�����
		RespLen��[out] �����λ�ɹ��������ݵĳ���
	����ֵ:
		0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY SetPrinterReaderCardReset(int cardType, BYTE *pRespData, int *RespLen);

/*
	��ӡ��������ִ��APDU����
	������
		cardType: [in] ������   1, �ǽ���Ƶ��(typeA, typeB)  2,�Ӵ�IC��(T=0,T=1)
		pSendData�� [in] ��Ҫ���͵�APDU����
		sendLen��[in] ��Ҫ���͵�APDU����ĳ���
		pRcvData��[out] ���APDUͨѶ�󷵻ص�����
		rcvLen��[out] ���APDUͨѶ�󷵻����ݵĳ���
	����ֵ:
		0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY MtPrinterReaderCardApdu(int cardType, BYTE *pSendData, int sendLen, BYTE *pRcvData, int *rcvLen);

/*
	��ӡ����ִ����󿨻ص�С����
	������	
		frontImgProp��[in] ����ͼ��
		backImgProp��[in] ����ͼ�񣬴���NULL����ӡ����
		pInDocProp��[in] ��ӡ���ã�����NULL��ʹ��Ĭ�ϴ�ӡ��ѡ��
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY PrintCard(CardImageProp *frontImgProp, CardImageProp *backImgProp, DocProp *pInDocProp);


/*
	�Ӵ�ӡ��ģ���ƿ���������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardPrinter2Slider();

/*
	�ƶ����������� -> ��д������
*/
CARDPROCESS_API int APIENTRY MtMoveCardSlider2RW();

/*
	�ƶ����������� <- ��д������
*/
CARDPROCESS_API int APIENTRY MtMoveCardSlider4RW();

/*
	�ƶ�������д������ -> ͨ��
*/
CARDPROCESS_API int APIENTRY MtMoveCardRW2Pass();

/*
	�ƶ�����ͨ�� <- ������
*/
CARDPROCESS_API int APIENTRY MtMoveCardPass4Exit();


/*
	��������λ
	SliderMoveZero = 0,         // ԭ�㣨��λλ�ã�
	SliderMoveEncoderBox1 = 1,  // ����#1
	SliderMoveEncoderBox2 = 2,  // ����#2
	SliderMoveEncoderBox3 = 3,  // ����#3
	SliderMoveReader = 4,       // ������
	SliderMoveStoreDisk1 = 5,   // ����1��
	SliderMoveStoreDisk2 = 6,   // ����2��
	SliderMoveHTPrinter = 7,      //���ߴ�ӡ��
	SliderMoveEmboss = 8,       // ͹��
	SliderMoveHot = 9,          // ����
	SliderMoveBin1F1 = 10,    // ������
	SliderMoveBin2F1 = 11,    // �����
	SliderMovePrinter = 12,   // ������ֱӡ��
	SliderMoveScanner = 13,   // ɨ����
	SliderMoveSpace = 14,       //����λ��
	SliderMove310 = 15,       //310������
*/
CARDPROCESS_API int APIENTRY MtMoveSliderTo(int pos);

/*
	������������λƫ��
	������
		xy��[in] ��ʾ���һ�����������1 - ���ҷ��򣬼�X�ᣬ2 - �������򣬼�Y��
		pos��[in] λ�ô��룬X�����Y�ᶨ���λ��
		forward��[in] �Ƿ�����X������Ϊ����X�Ḵλ�����㣬ֻ�������ƶ�����Y������Ϊ����
		offset��[out] ƫ��ֵ��Ĭ��Ϊ0x40
*/
CARDPROCESS_API int APIENTRY MtModifySliderDeviation(int xy, int pos, bool forward, BYTE *offset); 

/*
	����������λƫ��
*/
CARDPROCESS_API int APIENTRY MtReadSliderDeviation(int xy, int pos, BYTE *offset); 

/*
	�������
*/
CARDPROCESS_API int APIENTRY MtStorageUnlock();

/*
	������ƫ��
	type    [in]  0����������λƫ��    1��������1��ƫ��     2��������2��ƫ��
	offset��[out] ƫ��ֵ
	����ֵ:
			0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY MtReadStorageDeviation(int type, BYTE *offset);

/*
	��������ƫ��
	������
	type    [in]  0��������λƫ��    1������1��ƫ��     2������2��ƫ��
	nDU     [in]  0��ƫ��+    1��ƫ��-
	offset��[out] ƫ��ֵ
	����ֵ:
			0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY MtModifyStorageDeviation(int type, int nDU, BYTE *offset); 

/*
	���ÿ����̵㷽ʽ
	������
	type    [in]  0���̵㷽ʽ0    1���̵㷽ʽ1
	����ֵ:
			0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY MtSetStorageInitType(int type); 

/*
	��ѯ�����λģ��ֵ
		diskNo: [in] �̺ţ�500���������̣�1��2 
		slots��[out] �����ѡ�̲�λģ���ź�
		len��[out] ��λ�濨����Ĭ��250
	����ֵ:
			0 - �ɹ���> 0 ʧ��
*/
CARDPROCESS_API int APIENTRY MtQueryStorageSignal(int diskNo, BYTE slots[], int *len); 

///////////////////////////////////////////////////////////////////////////////////////�ڲ����Խӿ�

// ����С���ƶ�
CARDPROCESS_API int APIENTRY DebugSliderMove(int mover, int direction, int steps);


// ��������λ 1 - ���һ�������2 - ���»�������3 - ������λ
CARDPROCESS_API int APIENTRY DebugSliderReset(int which);

/*
	��ѯ������������״̬
	������
		mStatus��[out] �豸״̬����������С����9�ֽ�
			mStatus[0]    ��������
			mStatus[1]    ��������
			mStatus[2]    ��������
			mStatus[3]    ��������
			mStatus[4]    ����1δ����
			mStatus[5]    ����1��
			mStatus[6]    ����1��
			dStatus[7]    ����2δ����
			dStatus[8]    ����2��
			dStatus[9]    ����2��
			dStatus[10]   0 �� ��������0 - �쳣
	����ֵ��
		0 - �ɹ���>0 - ʧ��
  */
CARDPROCESS_API int APIENTRY GetSliderStatus(BYTE *mStatus);


/*
	�ƶ������们����
	������	
		boxNo��[in] ����ţ�ȡֵ��Χ1~3
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardBoxSlider(int boxNo);
///////////////////////////////////////////////////////////////////////////////////////


/*
	�豸ģ�鷢���Զ���ָ��
	������
		module��[in] ģ��
		buffSend��[in] ��������
		iSendLen��[in] �������ݳ���
		buffRcv��[out] Ӧ������
		iRcvLen��[out] Ӧ�����ݳ���
	����ֵ��
		0 - �ɹ��� >0 - ʧ��
*/
CARDPROCESS_API int APIENTRY DebugExecuteCmd(int module, BYTE *buffSend, int iSendLen, BYTE *buffRcv, int *iRcvLen);


/*
	�����豸ģ��
*/
CARDPROCESS_API int APIENTRY DebugFindModules(int module);


/*
	ɨ�迨
	������	
		frontImg[in] �Ƿ񱣴�����ͼ��     false��������    true������
		backImg[in] �Ƿ񱣴汳��ͼ��
		cutImg[in] �Ƿ񱣴����ͼ��
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY ScanCard(bool frontImg, bool backImg, bool cutImg);


/*
	��ɨ��ģ���ƿ���������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardScanner2Slider();

/*
	�ӻ������ƿ���ɨ����
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToScanner();


/*
	��ѯ͹��ģ��״̬
	������
		mStatus��[out]   ģ��״̬����������С����5�ֽ�
			mStatus[0]   ͹��ģ��״̬�� 0 �� ������1 �� �п���2 - �д���
			mStatus[1]   ����ģ��״̬�� 0 �� ������1 �� �п���2 - �д���
			mStatus[2]   �̽��״̬��0 - ������1 - �����꣬2 - �����꣬3 - δװ�̽�У� 4 - δ֪
			mStatus[3]   ������״̬��0 - ������1 - �����꣬2 - �����꣬3 - δװ�����У� 4 - δ֪
			mStatus[4]   ���ִ�״̬��0 - ������1 - ���ִ��٣�2 - δ֪
	����ֵ��
		0 - �ɹ���>0 - ʧ��
  */
CARDPROCESS_API int APIENTRY GetEmbossStatus(BYTE *mStatus);

/*
	�ӻ������ƿ���͹��ģ��
	������	
		��
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardToEmboss();

/*
	��͹��ģ���ƿ���������
	��������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY MoveCardEmbossToSlider();

/*
	͹�̿���ִ����󿨻ص�С����
	������
		type��[in] ͹�����ͣ�0 - ��͹�֣�1 - ͹���̽� 2 - ͹������	
		pEmbossProp��[in] ͹������(�������ꡢ���塢����)
		size��[in] �м���͹������
	����ֵ��
		0 - �ɹ���>0 - ʧ�ܣ���������
  */
CARDPROCESS_API int APIENTRY PlayEmboss(int type, EmbossProp *pEmbossProp, int size);

/*
	͹��ģ�����
*/
CARDPROCESS_API int APIENTRY MtEmbossHotCardIn();


/*
	͹��ģ�����
*/
CARDPROCESS_API int APIENTRY MtEmbossHotCardOut();


/*
	����ģ�����
*/
CARDPROCESS_API int APIENTRY MtSingleHotCardIn();


/*
	����ģ�����
*/
CARDPROCESS_API int APIENTRY MtSingleHotCardOut();


/*
	����ģ����ɫ
*/
CARDPROCESS_API int APIENTRY MtExecHotSilver();


#ifdef __cplusplus
}
#endif