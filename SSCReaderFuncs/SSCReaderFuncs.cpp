// SSCReaderFuncs.cpp : ���� DLL �ĳ�ʼ�����̡�
//
#define SCC_READER_FUNCS_EXPORT

#include "stdafx.h"
#include "SSCReaderFuncs.h"
#include "HexBin.h"
#include "easylogging++.h"
#include "devcfg.h"

INITIALIZE_EASYLOGGINGPP;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CSSCReaderFuncsApp

BEGIN_MESSAGE_MAP(CSSCReaderFuncsApp, CWinApp)
END_MESSAGE_MAP()


// CSSCReaderFuncsApp ����

CSSCReaderFuncsApp::CSSCReaderFuncsApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSSCReaderFuncsApp ����

CSSCReaderFuncsApp theApp;


// CSSCReaderFuncsApp ��ʼ��

BOOL CSSCReaderFuncsApp::InitInstance()
{
	CWinApp::InitInstance();
	initLog();
	return TRUE;
}

SSCReaderFuncs::SSCReaderFuncs(std::shared_ptr<SSCApduSender> sender)
{
	mSender = sender;
}

SSCReaderFuncs::~SSCReaderFuncs()
{
}

// ��ʼ��log
int CSSCReaderFuncsApp::initLog()
{
#define	LOG_TAG "SSCRFDLL"	// ģ���ʶ
	USES_CONVERSION;
	devcfg cfg;
	CString pathCtr = cfg.LogCfgPath();
	if (pathCtr.GetLength() == 0)
	{
		return -1;
	}
	string s = string(T2A(pathCtr));
	el::Configurations conf(s);
	el::Configuration* cf = conf.get(el::Level::Global, el::ConfigurationType::Format);
	// ���ģ��tag
	string newVal = cf->value();
	int inde = newVal.find_first_of(' ');
	string add = LOG_TAG;
	add = " " + add + " ";
	newVal.replace(inde, 1, add);
	conf.set(el::Level::Global, el::ConfigurationType::Format, newVal);
	el::Loggers::reconfigureAllLoggers(conf);
	return 0;
}

// ���籣������Ϣ
int SSCReaderFuncs::ReadCardBaseInfo(SSCBaseInfo & info)
{
	CString sResp;
	CString gfbb;  //�淶�汾�������Ǽ����籣��
	DWORD sw;
	int ret = SendApduSw(_T("0084000008"), sResp, sw);
	if (ret) {
		return ret;
	}
	info.random = sResp;  //�����
	if (SendApduSw(_T("00A404000F7378312E73682EC9E7BBE1B1A3D5CF"), sResp,
		sw)) {
		return ERR_AS_TRANS;
	}
	if (0x9000 != sw) {
		return ERR_APDU_SW;
	}

	sResp.Empty();
	if (SendApduSw(_T("00b2032c06"), sResp, sw)) {
		return ERR_AS_TRANS;
	}
	if (0x9000 != sw) {
		return ERR_APDU_SW;
	}
	HexBin hb;
	gfbb = sResp.Mid(4, 2);
	if (_T("32") == gfbb || _T("00") == gfbb) {  //������
		sResp.Empty();
		if (SendApduSw(_T("00b2012c12"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp.Empty();
		if (SendApduSw(_T("00b2042c0E"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}

		sResp.Empty();
		if (SendApduSw(_T("00b2052c06"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		info.fkrq = sResp.Right(sResp.GetLength() - 4);  //��������

		sResp.Empty();
		if (SendApduSw(_T("00b2062c06"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		info.yxrq = sResp.Right(sResp.GetLength() - 4);  //����Ч��

		sResp.Empty();
		if (SendApduSw(_T("00b2072c0B"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.ssno);  //�籣����

		sResp.Empty();
		if (SendApduSw(_T("00b2013414"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.idnum);  //���֤��

		sResp.Empty();
		if (SendApduSw(_T("00b2023420"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.name);  //����

		sResp.Empty();
		if (SendApduSw(_T("00b2043403"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.sex);  //�Ա�
	}
	else if (_T("31") == gfbb) {             //һ����
		sResp.Empty();
		if (SendApduSw(_T("00b2012c12"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp.Empty();
		if (SendApduSw(_T("00b2042c0E"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}

		sResp.Empty();
		if (SendApduSw(_T("00b2052c06"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		info.fkrq = sResp.Right(sResp.GetLength() - 4);  //��������

		sResp.Empty();
		if (SendApduSw(_T("00b2062c06"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		info.yxrq = sResp.Right(sResp.GetLength() - 4);  //����Ч��

		sResp.Empty();
		if (SendApduSw(_T("00b2072c0B"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.ssno);  //�籣����

		sResp.Empty();
		if (SendApduSw(_T("00b2013414"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.idnum);  //���֤��

		sResp.Empty();
		if (SendApduSw(_T("00b2023420"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.name);  //����

		sResp.Empty();
		if (SendApduSw(_T("00b2033403"), sResp, sw)) {
			return ERR_AS_TRANS;
		}
		if (0x9000 != sw) {
			return ERR_APDU_SW;
		}
		sResp = sResp.Right(sResp.GetLength() - 4);
		hb.DecodeToCString(sResp, info.sex);  //�Ա�
	}
	else {
		return -1;
	}
	return 0;
}

// �����п���
int SSCReaderFuncs::ReadBanknum(CString & bknum)
{
	string val;
	// ����
	string resp;
	int ret = 0;
	ret = SendApduMustOk("00A4040008A000000333010101", resp);
	if (ret < 0) {
		LOG(ERROR) << "ѡӦ��ʧ��";
		return ret;
	}
	string tmp;
	int pos = 0;
	do {
		// 00B2018421
		ret = SendApdu("00B2018421", resp);
		if (ret < 0) {
			return ret;
		}
		string tmp;
		if (resp.length() == 4) {
			tmp = resp.substr(0, 2);
			if (0 != tmp.compare("6C")) {
				break;
			}
			tmp = resp.substr(2, 2);
			string send = "00B2018421";
			send.append(tmp);
			ret = SendApdu(send, resp);
			if (ret < 0) {
				return ret;
			}
		}
		// ����5A0A��5A09
		int pos = resp.find("5A0A", 0);
		if (pos >= 0) {
			tmp = resp.substr(pos + 4, 0x0a * 2);
			pos = tmp.find('F', 0);
			if (pos >= 0) {
				val = tmp.substr(0, pos);
				bknum = CString(val.c_str());
			}
			else {
				val = tmp; 
				bknum = CString(val.c_str());
			}
			return 0;
		}
		pos = resp.find("5A09", 0);
		if (pos >= 0) {
			tmp = resp.substr(pos + 4, 0x09 * 2);
			pos = tmp.find('F', 0);
			if (pos >= 0) {
				val = tmp.substr(0, pos);
				bknum = CString(val.c_str());
			}
			else {
				val = tmp;
				bknum = CString(val.c_str());
			}
			return 0;
		}
	} while (FALSE);
	do {
		//00B2011400
		ret = SendApdu("00B2011400", resp);
		if (ret < 0) {
			return ret;
		}
		if (resp.length() == 4) {
			tmp = resp.substr(0, 2);
			if (0 != tmp.compare("6C")) {
				break;
			}
			tmp = resp.substr(2, 2);
			string send = "00B20114";
			send.append(tmp);
			ret = SendApdu(send, resp);
			if (ret < 0) {
				return ret;
			}
		}
		// ����5A0A��5A08��5A09
		pos = resp.find("5A0A", 0);
		if (pos >= 0) {
			tmp = resp.substr(pos + 4, 0x0a * 2);
			pos = tmp.find('F', 0);
			if (pos >= 0) {
				val = tmp.substr(0, pos);
				bknum = CString(val.c_str());
			}
			else {
				val = tmp;
				bknum = CString(val.c_str());
			}
			return 0;
		}
		pos = resp.find("5A08", 0);
		if (pos >= 0) {
			tmp = resp.substr(pos + 4, 0x08 * 2);
			pos = tmp.find('F', 0);
			if (pos >= 0) {
				val = tmp.substr(0, pos);
				bknum = CString(val.c_str());
			}
			else {
				val = tmp;
				bknum = CString(val.c_str());
			}
			return 0;
		}
		pos = resp.find("5A09", 0);
		if (pos >= 0) {
			tmp = resp.substr(pos + 4, 0x09 * 2);
			pos = tmp.find('F', 0);
			if (pos >= 0) {
				val = tmp.substr(0, pos);
				bknum = CString(val.c_str());
			}
			else {
				val = tmp;
				bknum = CString(val.c_str());
			}
			return 0;
		}
	} while (FALSE);

	do {
		// 00B2011C00
		ret = SendApdu("00B2011C00", resp);
		if (ret < 0) {
			return ret;
		}
		if (resp.length() == 4) {
			tmp = resp.substr(0, 2);
			if (0 != tmp.compare("6C")) {
				break;
			}
			tmp = resp.substr(2, 2);
			string send = "00B2011C";
			send.append(tmp);
			ret = SendApdu(send, resp);
			if (ret < 0) {
				return ret;
			}
		}
		// ����5A0A
		pos = resp.find("5A0A", 0);
		if (pos >= 0) {
			tmp = resp.substr(pos + 4, 0x0a * 2);
			pos = tmp.find('F', 0);
			if (pos >= 0) {
				val = tmp.substr(0, pos);
				bknum = CString(val.c_str());
			}
			else {
				val = tmp;
				bknum = CString(val.c_str());
			}
			return 0;
		}
	} while (FALSE);
	LOG(ERROR) << "��֧�ֶ������п�����";
	return -6;
}

int SSCReaderFuncs::SendApduMustOk(std::string send, std::string resp)
{
	CString sResp;
	DWORD sw;
	LOG(DEBUG) << "send: " << send;
	USES_CONVERSION;
	int ret = mSender->apdu(CString(send.c_str()), sResp, sw);	
	if (ret != 0) {
		LOG(ERROR) << "err: " << ret;
		return ERR_AS_TRANS;
	}
	if (sw == 0x9000)
	{
		resp = string(T2A(sResp));
		LOG(DEBUG) << "resp: " << resp;
		return 0;
	}
	else
	{
		CString s;
		s.Format(_T("%04X"), sw);
		LOG(DEBUG) << "resp: " << T2A(s);
		return ERR_APDU_SW;
	}
}

int SSCReaderFuncs::SendApdu(std::string send, std::string &resp)
{
	CString s;
	CString r;
	DWORD sw;
	LOG(DEBUG) << "send: " << send;
	USES_CONVERSION;
	s = A2T(send.c_str());
	int ret = mSender->apdu(s, r, sw);
	if (ret != 0)
	{
		LOG(ERROR) << "err: " << ret;
		return ret;
	}
	r.Append(_T("%04X"), sw);
	resp = string(T2A(r));
	LOG(DEBUG) << "resp: " << resp;
	return 0;
}

int SSCReaderFuncs::SendApduSw(CString send, CString & resp, DWORD & SW1SW2)
{
	USES_CONVERSION;
	LOG(DEBUG) << "send: " << T2A(send);
	int ret = mSender->apdu(send, resp, SW1SW2);
	if (ret < 0)
	{
		LOG(ERROR) << "err: " << ret;
		return ret;
	}
	CString s;
	s.Format(_T("%04X"), SW1SW2);
	LOG(DEBUG) << "resp: " << resp << T2A(s);
	return ret;
}

