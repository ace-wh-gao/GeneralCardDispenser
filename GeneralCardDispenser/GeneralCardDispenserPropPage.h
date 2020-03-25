#pragma once

// GeneralCardDispenserPropPage.h : CGeneralCardDispenserPropPage 属性页类的声明。


// CGeneralCardDispenserPropPage : 有关实现的信息，请参阅 GeneralCardDispenserPropPage.cpp。

class CGeneralCardDispenserPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CGeneralCardDispenserPropPage)
	DECLARE_OLECREATE_EX(CGeneralCardDispenserPropPage)

// 构造函数
public:
	CGeneralCardDispenserPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_GENERALCARDDISPENSER };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

