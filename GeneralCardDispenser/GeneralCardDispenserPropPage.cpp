// GeneralCardDispenserPropPage.cpp : CGeneralCardDispenserPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "GeneralCardDispenser.h"
#include "GeneralCardDispenserPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGeneralCardDispenserPropPage, COlePropertyPage)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CGeneralCardDispenserPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CGeneralCardDispenserPropPage, "GENERALCARDDIS.GeneralCardDisPropPage.1",
	0x1676ca11, 0x66b6, 0x4cde, 0x86, 0x2a, 0x51, 0x99, 0x44, 0x4d, 0x75, 0xa2)

// CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CGeneralCardDispenserPropPage ��ϵͳע�����

BOOL CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_GENERALCARDDISPENSER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPage - ���캯��

CGeneralCardDispenserPropPage::CGeneralCardDispenserPropPage() :
	COlePropertyPage(IDD, IDS_GENERALCARDDISPENSER_PPG_CAPTION)
{
}

// CGeneralCardDispenserPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CGeneralCardDispenserPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CGeneralCardDispenserPropPage ��Ϣ�������
