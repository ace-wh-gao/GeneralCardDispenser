#pragma once

// GeneralCardDispenserPropPage.h : CGeneralCardDispenserPropPage ����ҳ���������


// CGeneralCardDispenserPropPage : �й�ʵ�ֵ���Ϣ������� GeneralCardDispenserPropPage.cpp��

class CGeneralCardDispenserPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CGeneralCardDispenserPropPage)
	DECLARE_OLECREATE_EX(CGeneralCardDispenserPropPage)

// ���캯��
public:
	CGeneralCardDispenserPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_GENERALCARDDISPENSER };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

