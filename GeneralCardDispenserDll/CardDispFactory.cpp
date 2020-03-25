#include "stdafx.h"
#include "CardDispFactory.h"
#include "CZ_QZ05CT.h"
#include "devcfg.h"
#include "easylogging++.h"
#include "errcode.h"

using namespace std;

CardDispFactory::CardDispFactory()
{
}

int CardDispFactory::CreateDev(shared_ptr<BaseCardDispenser> &pbcd)
{
	int ret=GetDevModel();
	if (ret == CD_MODEL_CZ_QZ05CT3)
	{
		pbcd = make_shared<CCZ_QZ05CT>();
		RetStatus rs;
		pbcd->LoadCfg(&rs);
		if (rs.code != 0)
		{
			ret = ERR_LOAD_CFG_FILE;
		}
	}
	else
	{
		LOG(ERROR) << "���ô���û���ҵ����ͺŵ��쿨��";
		pbcd = nullptr;
		ret = ERR_LOAD_CFG_FILE;
	}
	return ret;
}

int CardDispFactory::GetDevModel()
{
	devcfg cfg;
	CString file_path = cfg.CfgPath();
	
	if (file_path.GetLength() == 0)
	{
		LOG(ERROR) << "�޷���ȡ����ϵͳ�û���Ŀ¼�����������������HOMEPATH�Ƿ���ڡ�";
		LOG(ERROR) << "�Ҳ��������ļ�";
		return ERR_LOAD_CFG_FILE;
	}

	//�ж������ļ��Ƿ����
	if (GetFileAttributes(file_path) == -1)// ����-1��ʾ�����ļ�������,��Ҫ�ֶ�����һ��
	{
		LOG(ERROR) << "file path = " << file_path;
		LOG(ERROR) << "�Ҳ��������ļ�";
		return ERR_LOAD_CFG_FILE;
	}

	CString section;
	CString key;
	int val;
	const int defa_int = -1;
	section = _T("manager");
	key = _T("model");
	USES_CONVERSION;
	int ret = GetPrivateProfileInt(section, key, defa_int, file_path);
	if (ret < 0)
	{
		LOG(ERROR) << "�Σ�" << T2A(section) << "; �����"<< T2A(key);
		LOG(ERROR) << "�Ҳ���������";
		return ERR_LOAD_CFG_FILE;
	}

	return ret;
}


CardDispFactory::~CardDispFactory()
{
}
