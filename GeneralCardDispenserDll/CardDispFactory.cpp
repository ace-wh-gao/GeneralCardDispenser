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
		LOG(ERROR) << "配置错误。没有找到该型号的领卡机";
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
		LOG(ERROR) << "无法获取操作系统用户根目录。请检查操作环境变量HOMEPATH是否存在。";
		LOG(ERROR) << "找不到配置文件";
		return ERR_LOAD_CFG_FILE;
	}

	//判断配置文件是否存在
	if (GetFileAttributes(file_path) == -1)// 返回-1表示配置文件不存在,需要手动创建一个
	{
		LOG(ERROR) << "file path = " << file_path;
		LOG(ERROR) << "找不到配置文件";
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
		LOG(ERROR) << "段：" << T2A(section) << "; 配置项："<< T2A(key);
		LOG(ERROR) << "找不到配置项";
		return ERR_LOAD_CFG_FILE;
	}

	return ret;
}


CardDispFactory::~CardDispFactory()
{
}
