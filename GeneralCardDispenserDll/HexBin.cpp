#include "stdafx.h"
#include "HexBin.h"
#include <memory>

using namespace std;

HexBin::HexBin()
{
	mIsUpper = TRUE;
}

HexBin::HexBin(BOOL isUpper)
{
	mIsUpper = isUpper;
}


HexBin::~HexBin()
{
}

// 编码Hex
int HexBin::Encode(BYTE * bin, int len, CString & hex)
{
	hex = _T("");
	CString sf;
	if (mIsUpper)
	{
		sf = _T("%02X");
	}
	else
	{
		sf = _T("%02x");
	}
	for (int i = 0; i < len; ++i)
	{
		hex.Format(sf, bin[i]);
	}
	return 0;
}

// 解码Hex
// 返回值:
// -1 hex码不为偶数
// -2 hex码存在非法字符
int HexBin::Decode(CString hex, BYTE * bin, int & len)
{
	int hlen = hex.GetLength();
	if (hlen % 2 != 0)
	{
		return -1;
	}
	int nHexLen = hlen / 2;
	for (int i = 0; i < nHexLen; i++) {
		unsigned char Nibble[2];
		Nibble[0] = hex.GetAt(i*2);
		Nibble[1] = hex.GetAt(i * 2 + 1);
		for (int j = 0; j < 2; j++) {
			if (Nibble[j] <= 'F' && Nibble[j] >= 'A')
				Nibble[j] = Nibble[j] - 'A' + 10;
			else if (Nibble[j] <= 'f' && Nibble[j] >= 'a')
				Nibble[j] = Nibble[j] - 'a' + 10;
			else if (Nibble[j] >= '0' && Nibble[j] <= '9')
				Nibble[j] = Nibble[j] - '0';
			else
				return -2;
		}                          // for (int j = ...)
		bin[i] = Nibble[0] << 4;  // Set the high nibble
		bin[i] |= Nibble[1];      // Set the low nibble
	}                            // for (int i = ...)
	len = nHexLen;
	return 0;
}

// 解码Hex后转成CString
// 返回值:
// -1 hex码不为偶数
// -2 hex码存在非法字符
int HexBin::DecodeToCString(CString hex, CString & binString)
{
	int blen = hex.GetLength();
	if (blen == 0)
	{
		binString = _T("");
		return 0;
	}
	vector<BYTE> vb(hex.GetLength() + 1);
	int ret = Decode(hex, vb.data(), blen);
	if (ret != 0)
	{
		return ret;
	}
	USES_CONVERSION;
	vb[blen] = '0';
	binString = A2T((char *)vb.data());
	return 0;
}

