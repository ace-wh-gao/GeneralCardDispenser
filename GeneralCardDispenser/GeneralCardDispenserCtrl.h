#pragma once

// GeneralCardDispenserCtrl.h : CGeneralCardDispenserCtrl ActiveX 控件类的声明。

//去掉安全警告 BEGIN
#include <objsafe.h>
//去掉安全警告 END


// CGeneralCardDispenserCtrl : 有关实现的信息，请参阅 GeneralCardDispenserCtrl.cpp。

class CGeneralCardDispenserCtrl : public COleControl
{
	DECLARE_DYNCREATE(CGeneralCardDispenserCtrl)

// 构造函数
public:
	CGeneralCardDispenserCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CGeneralCardDispenserCtrl();

	DECLARE_OLECREATE_EX(CGeneralCardDispenserCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CGeneralCardDispenserCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CGeneralCardDispenserCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CGeneralCardDispenserCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID


//去掉安全警告 BEGIN
	DECLARE_INTERFACE_MAP()
	BEGIN_INTERFACE_PART(ObjectSafety, IObjectSafety)
		STDMETHOD(GetInterfaceSafetyOptions)
		(REFIID riid, DWORD __RPC_FAR* pdwSupportedOptions,
			DWORD __RPC_FAR* pdwEnabledOptions);
	STDMETHOD(SetInterfaceSafetyOptions)
		(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions);
	END_INTERFACE_PART(ObjectSafety)
//去掉安全警告 END

public:
	enum {
		eventidOnReaderCardOut = 29L,
		dispidReaderCardOut = 29L,
		eventidOnReaderCardIn = 28L,
		dispidReaderCardIn = 28L,
		eventidOnRepoUsedSlots = 27L,
		dispidRepoUsedSlots = 27L,
		eventidOnRepoUsedNum = 26L,
		dispidRepoUsedNum = 26L,
		eventidOnReaderReadBKNum = 25L,
		dispidReaderReadBKNum = 25L,
		eventidOnReaderReadSSInfo = 24L,
		dispidReaderReadSSInfo = 24L,
		eventidOnReset = 23L,
		dispidReset = 23L,
		dispidEmbossStatus = 22L,
		dispidOCRStatus = 21L,
		dispidReaderStatus = 20L,
		dispidCardPrinterStatus = 19L,
		dispidOutboxStatus = 18L,
		dispidRepoStatus = 17L,
		dispidInboxStatus = 16L,
		eventidOnReaderStatus = 22L,
		eventidOnEmbossStatus = 21L,
		eventidOnOCRStatus = 20L,
		eventidOnCardPrinterStatus = 19L,
		eventidOnOutboxStatus = 18L,
		eventidOnRepoStatus = 17L,
		eventidOnInboxStatus = 16L,
		eventidOnCarStatus = 15L,
		dispidCarStatus = 15L,
		eventidOnEmbossToCar = 14L,
		dispidEmbossToCar = 14L,
		eventidOnCarToEmboss = 13L,
		dispidCarToEmboss = 13L,
		eventidOnOCRToCar = 12L,
		dispidOCRToCar = 12L,
		eventidOnCarToOCR = 11L,
		dispidCarToOCR = 11L,
		eventidOnCardPrinterToCar = 10L,
		dispidCardPrinterToCar = 10L,
		eventidOnCarToCardPrinter = 9L,
		dispidCarToCardPrinter = 9L,
		eventidOnCarToReader = 8L,
		dispidCarToReader = 8L,
		eventidOnReaderToCar = 7L,
		dispidReaderToCar = 7L,
		eventidOnCarToOutbox = 6L,
		dispidCarToOutbox = 6L,
		eventidOnRepoToCar = 5L,
		dispidRepoToCar = 5L,
		eventidOnCarToRepo = 4L,
		dispidCarToRepo = 4L,
		eventidOnInboxToCar = 3L,
		dispidInboxToCar = 3L,
		eventidOnClose = 2L,
		dispidClose = 2L,
		eventidOnOpen = 1L,
		dispidOpen = 1L
	};
protected:
	BSTR Open();

	LRESULT OnMsgTask(WPARAM wParam, LPARAM lParam);

	void FireOpenEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnOpen, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR Close();

	void FireCloseEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnClose, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR InboxToCar();

	void FireInboxToCarEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnInboxToCar, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CarToRepo();

	void FireCarToRepoEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCarToRepo, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR RepoToCar(LONG location);

	void FireRepoToCarEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnRepoToCar, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CarToOutbox(LONG num);

	void FireCarToOutboxEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCarToOutbox, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR ReaderToCar(LONG readerId);

	void FireReaderToCarEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnReaderToCar, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CarToReader(LONG readerId);

	void FireCarToReaderEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCarToReader, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CarToCardPrinter(LONG port);

	void FireCarToCardPrinterEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCarToCardPrinter, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CardPrinterToCar(LONG port);

	void FireCardPrinterToCarEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCardPrinterToCar, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CarToOCR();

	void FireCarToOCREvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCarToOCR, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR OCRToCar();

	void FireOCRToCarEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnOCRToCar, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CarToEmboss();

	void FireCarToEmbossEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCarToEmboss, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR EmbossToCar();

	void FireEmbossToCarEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnEmbossToCar, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR CarStatus();

	void FireCarStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCarStatus, EVENT_PARAM(VTS_BSTR), msg);
	}

	void FireInboxStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnInboxStatus, EVENT_PARAM(VTS_BSTR), msg);
	}

	void FireRepoStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnRepoStatus, EVENT_PARAM(VTS_BSTR), msg);
	}

	void FireOutboxStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnOutboxStatus, EVENT_PARAM(VTS_BSTR), msg);
	}

	void FireCardPrinterStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnCardPrinterStatus, EVENT_PARAM(VTS_BSTR), msg);
	}

	void FireOCRStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnOCRStatus, EVENT_PARAM(VTS_BSTR), msg);
	}

	void FireEmbossStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnEmbossStatus, EVENT_PARAM(VTS_BSTR), msg);
	}

	void FireReaderStatusEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnReaderStatus, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR InboxStatus();
	BSTR RepoStatus();
	BSTR OutboxStatus();
	BSTR CardPrinterStatus();
	BSTR ReaderStatus(LONG id);
	BSTR OCRStatus();
	BSTR EmbossStatus();
	BSTR Reset(LONG type);

	void FireResetEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnReset, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR ReaderReadSSInfo(LONG zone);

	void FireReaderReadSSInfoEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnReaderReadSSInfo, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR ReaderReadBKNum();

	void FireReaderReadBKNumEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnReaderReadBKNum, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR RepoUsedNum();

	void FireRepoUsedNumEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnRepoUsedNum, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR RepoUsedSlots();

	void FireRepoUsedSlotsEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnRepoUsedSlots, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR ReaderCardIn(LONG readerId, LONG timeout);

	void FireReaderCardInEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnReaderCardIn, EVENT_PARAM(VTS_BSTR), msg);
	}
	BSTR ReaderCardOut(LONG readerId, LONG timeout);

	void FireReaderCardOutEvent(LPCTSTR msg)
	{
		FireEvent(eventidOnReaderCardOut, EVENT_PARAM(VTS_BSTR), msg);
	}
};

