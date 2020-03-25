// GeneralCardDispenserPropPage.cpp : CGeneralCardDispenserPropPage 属性页类的实现。

#include "stdafx.h"
#include "GeneralCardDispenser.h"
#include "GeneralCardDispenserPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGeneralCardDispenserPropPage, COlePropertyPage)

// 消息映射

BEGIN_MESSAGE_MAP(CGeneralCardDispenserPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CGeneralCardDispenserPropPage, "GENERALCARDDIS.GeneralCardDisPropPage.1",
	0x1676ca11, 0x66b6, 0x4cde, 0x86, 0x2a, 0x51, 0x99, 0x44, 0x4d, 0x75, 0xa2)

// CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPageFactory::UpdateRegistry -
// 添加或移除 CGeneralCardDispenserPropPage 的系统注册表项

BOOL CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_GENERALCARDDISPENSER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPage - 构造函数

CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPage() :
	COlePropertyPage(IDD, IDS_GENERALCARDDISPENSER_PPG_CAPTION)
{
}

// CGeneralCardDispenserPropPage::DoDataExchange - 在页和属性间移动数据

void CGeneralCardDispenserPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CGeneralCardDispenserPropPage 消息处理程序
