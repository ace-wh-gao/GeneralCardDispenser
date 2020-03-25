#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;

// Hex码编解码
class HexBin
{
public:
	HexBin();
	HexBin(BOOL isUpper);
	~HexBin();
	// Hex码编码
	int Encode(BYTE* bin, int len, CString &hex);
	// Hex码解码
	int Decode(CString hex, BYTE* bin, int &len);
	// Hex码解码后转成CString
	int DecodeToCString(CString hex, CString &binString);
private:
	BOOL mIsUpper;
};

