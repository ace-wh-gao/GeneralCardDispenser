#include "stdafx.h"
#include "StringUtil.h"


StringUtil::StringUtil()
{
}


StringUtil::~StringUtil()
{
}

std::string StringUtil::CString2String(CString cs)
{
	USES_CONVERSION;
	return std::string(T2A(cs));
}

