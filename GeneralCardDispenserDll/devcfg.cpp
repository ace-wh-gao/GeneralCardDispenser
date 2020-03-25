#include "stdafx.h"
#include "devcfg.h"


devcfg::devcfg()
{
}


devcfg::~devcfg()
{
}

// ��ȡ���ݴ洢Ŀ¼
CString devcfg::WorkDir()
{
	TCHAR path[MAX_PATH] = { 0 };
	DWORD wr=GetEnvironmentVariable(_T("HOMEPATH"), path, MAX_PATH);
	return CString(path);
}

CString devcfg::CfgDir()
{
	return WorkDir();
}

// ��ȡ����Ŀ¼
CString devcfg::CfgPath()
{
	CString ini_name = _T("CardDispenser.ini");
	CString wd = WorkDir();
	if (wd.GetLength() == 0)
	{
		return CString();
	}
	return WorkDir() + _T("\\") + ini_name;
}

CString devcfg::LogCfgPath()
{
	CString name = _T("log.conf");
	CString wd = WorkDir();
	if (wd.GetLength() == 0)
	{
		return CString();
	}
	return WorkDir() + _T("\\") + name;
	return CString();
}
