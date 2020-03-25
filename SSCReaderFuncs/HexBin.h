#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;

// Hex������
class HexBin
{
public:
	HexBin();
	HexBin(BOOL isUpper);
	~HexBin();
	// Hex�����
	int Encode(BYTE* bin, int len, CString &hex);
	// Hex�����
	int Decode(CString hex, BYTE* bin, int &len);
	// Hex������ת��CString
	int DecodeToCString(CString hex, CString &binString);
private:
	BOOL mIsUpper;
};

