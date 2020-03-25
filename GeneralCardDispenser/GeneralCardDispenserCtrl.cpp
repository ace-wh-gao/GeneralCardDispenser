// GeneralCardDispenserCtrl.cpp : CGeneralCardDispenserCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "GeneralCardDispenser.h"
#include "GeneralCardDispenserCtrl.h"
#include "GeneralCardDispenserPropPage.h"
#include "afxdialogex.h"
#include "../GeneralCardDispenserDll/GeneralCardDispenserDll.h"
#include "../JsonCpp/include/json/json.h"
#include "../SSCReaderFuncs/SSCReaderFuncs.h"
#include "StringUtil.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 自定义消息
#define WM_TASK (WM_USER + 1)

// 任务ID，用于任务线程的传递消息函数的wParam
#define TID_OPEN 1
#define TID_CLOSE 2
#define TID_INBOX_TO_CAR 3
#define TID_CAR_TO_REPO 4
#define TID_REPO_TO_CAR 5
#define TID_CAR_TO_OUTBOX 7
#define TID_READER_TO_CAR 8
#define TID_CAR_TO_READER 9
#define TID_CAR_TO_CARD_PRINTER 10
#define TID_CARD_PRINTER_TO_CAR 11
#define TID_CAR_TO_OCR 12
#define TID_OCR_TO_CAR 13
#define TID_CAR_TO_EMBOSS 14
#define TID_EMBOSS_TO_CAR 15
#define TID_CAR_STATUS 17
#define TID_INBOX_STATUS 18
#define TID_REPO_STATUS 19
#define TID_OUTBOX_STATUS 20
#define TID_CARD_PRINTER_STATUS 21
#define TID_READER_STATUS 22
#define TID_OCR_STATUS 23
#define TID_EMBOSS_STATUS 24
#define TID_RESET 25
#define TID_READER_READ_BK_NUM 26
#define TID_READER_READ_SS_INFO 27
#define TID_REPO_USED_NUM 28
#define TID_REPO_USED_SLOTS 29
#define TID_READER_CARD_IN 30
#define TID_READER_CARD_OUT 31

// 简化模型，一次只有一个接口可以执行
BOOL gIsWorking = FALSE;

CString gEventMsg;

// 函数入参
int gLocation = 0;
int gBoxnum = 0;
int gReaderId = 0;
int gCardPrinterPort = 0;
int gOutboxId = 0;
int gZone = 0;
int gTimeout = 0;

IMPLEMENT_DYNCREATE(CGeneralCardDispenserCtrl, COleControl)

//去掉安全警告 BEGIN
BEGIN_INTERFACE_MAP(CGeneralCardDispenserCtrl, COleControl)
	INTERFACE_PART(CGeneralCardDispenserCtrl, IID_IObjectSafety, ObjectSafety)
END_INTERFACE_MAP()

// Implementation of IObjectSafety
STDMETHODIMP CGeneralCardDispenserCtrl::XObjectSafety::GetInterfaceSafetyOptions(
	REFIID riid, DWORD __RPC_FAR *pdwSupportedOptions,
	DWORD __RPC_FAR *pdwEnabledOptions) {
	METHOD_PROLOGUE_EX(CGeneralCardDispenserCtrl, ObjectSafety)
		if (!pdwSupportedOptions || !pdwEnabledOptions) {
			return E_POINTER;
		}
	*pdwSupportedOptions =
		INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA;
	*pdwEnabledOptions = 0;
	if (NULL == pThis->GetInterface(&riid)) {
		TRACE("Requested interface is not supported.\n");
		return E_NOINTERFACE;
	}
	// What interface is being checked out anyhow?
	OLECHAR szGUID[39];
	int i = StringFromGUID2(riid, szGUID, 39);
	if (riid == IID_IDispatch) {
		// Client wants to know if object is safe for scripting
		*pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		return S_OK;
	}
	else if (riid == IID_IPersistPropertyBag ||
		riid == IID_IPersistStreamInit || riid == IID_IPersistStorage ||
		riid == IID_IPersistMemory) {
		// Those are the persistence interfaces COleControl derived controls support
		// as indicated in AFXCTL.H
		// Client wants to know if object is safe for initializing from persistent
		// data
		*pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA;
		return S_OK;
	}
	else {
		// Find out what interface this is, and decide what options to enable
		TRACE(
			"We didn't account for the safety of this interface, and it's one we "
			"support...\n");
		return E_NOINTERFACE;
	}
}

STDMETHODIMP CGeneralCardDispenserCtrl::XObjectSafety::SetInterfaceSafetyOptions(
	REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions) {
	METHOD_PROLOGUE_EX(CGeneralCardDispenserCtrl, ObjectSafety)
		OLECHAR szGUID[39];
	// What is this interface anyway?
	// We can do a quick lookup in the registry under HKEY_CLASSES_ROOT\Interface
	int i = StringFromGUID2(riid, szGUID, 39);
	if (0 == dwOptionSetMask && 0 == dwEnabledOptions) {
		// the control certainly supports NO requests through the specified
		// interface so it"s safe to return S_OK even if the interface isn"t
		// supported.
		return S_OK;
	}
	// Do we support the specified interface?
	if (NULL == pThis->GetInterface(&riid)) {
		TRACE1("%s is not support.\n", szGUID);
		return E_FAIL;
	}
	if (riid == IID_IDispatch) {
		TRACE("Client asking if it's safe to call through IDispatch.\n");
		TRACE("In other words, is the control safe for scripting?\n");
		if (INTERFACESAFE_FOR_UNTRUSTED_CALLER == dwOptionSetMask &&
			INTERFACESAFE_FOR_UNTRUSTED_CALLER == dwEnabledOptions) {
			return S_OK;
		}
		else {
			return E_FAIL;
		}
	}
	else if (riid == IID_IPersistPropertyBag ||
		riid == IID_IPersistStreamInit || riid == IID_IPersistStorage ||
		riid == IID_IPersistMemory) {
		TRACE("Client asking if it's safe to call through IPersist*.\n");
		TRACE(
			"In other words, is the control safe for initializing from persistent "
			"data?\n");
		if (INTERFACESAFE_FOR_UNTRUSTED_DATA == dwOptionSetMask &&
			INTERFACESAFE_FOR_UNTRUSTED_DATA == dwEnabledOptions) {
			return NOERROR;
		}
		else {
			return E_FAIL;
		}
	}
	else {
		TRACE1(
			"We didn't account for the safety of %s, and it's one we support...\n",
			szGUID);
		return E_FAIL;
	}
}
STDMETHODIMP_(ULONG) CGeneralCardDispenserCtrl::XObjectSafety::AddRef() {
	METHOD_PROLOGUE_EX_(CGeneralCardDispenserCtrl, ObjectSafety)
		return (ULONG)pThis->ExternalAddRef();
}
STDMETHODIMP_(ULONG) CGeneralCardDispenserCtrl::XObjectSafety::Release() {
	METHOD_PROLOGUE_EX_(CGeneralCardDispenserCtrl, ObjectSafety)
		return (ULONG)pThis->ExternalRelease();
}
STDMETHODIMP CGeneralCardDispenserCtrl::XObjectSafety::QueryInterface(
	REFIID iid, LPVOID *ppvObj) {
	METHOD_PROLOGUE_EX_(CGeneralCardDispenserCtrl, ObjectSafety)
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}
//去掉安全警告 END

// 消息映射

BEGIN_MESSAGE_MAP(CGeneralCardDispenserCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_TASK, OnMsgTask)
END_MESSAGE_MAP()

// 调度映射

BEGIN_DISPATCH_MAP(CGeneralCardDispenserCtrl, COleControl)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "Open", dispidOpen, Open, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "Close", dispidClose, Close, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "InboxToCar", dispidInboxToCar, InboxToCar, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CarToRepo", dispidCarToRepo, CarToRepo, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "RepoToCar", dispidRepoToCar, RepoToCar, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CarToOutbox", dispidCarToOutbox, CarToOutbox, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "ReaderToCar", dispidReaderToCar, ReaderToCar, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CarToReader", dispidCarToReader, CarToReader, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CarToCardPrinter", dispidCarToCardPrinter, CarToCardPrinter, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CardPrinterToCar", dispidCardPrinterToCar, CardPrinterToCar, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CarToOCR", dispidCarToOCR, CarToOCR, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "OCRToCar", dispidOCRToCar, OCRToCar, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CarToEmboss", dispidCarToEmboss, CarToEmboss, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "EmbossToCar", dispidEmbossToCar, EmbossToCar, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CarStatus", dispidCarStatus, CarStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "InboxStatus", dispidInboxStatus, InboxStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "RepoStatus", dispidRepoStatus, RepoStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "OutboxStatus", dispidOutboxStatus, OutboxStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "CardPrinterStatus", dispidCardPrinterStatus, CardPrinterStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "ReaderStatus", dispidReaderStatus, ReaderStatus, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "OCRStatus", dispidOCRStatus, OCRStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "EmbossStatus", dispidEmbossStatus, EmbossStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "Reset", dispidReset, Reset, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "ReaderReadSSInfo", dispidReaderReadSSInfo, ReaderReadSSInfo, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "ReaderReadBKNum", dispidReaderReadBKNum, ReaderReadBKNum, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "RepoUsedNum", dispidRepoUsedNum, RepoUsedNum, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "RepoUsedSlots", dispidRepoUsedSlots, RepoUsedSlots, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "ReaderCardIn", dispidReaderCardIn, ReaderCardIn, VT_BSTR, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CGeneralCardDispenserCtrl, "ReaderCardOut", dispidReaderCardOut, ReaderCardOut, VT_BSTR, VTS_I4 VTS_I4)
END_DISPATCH_MAP()

// 事件映射

BEGIN_EVENT_MAP(CGeneralCardDispenserCtrl, COleControl)
	EVENT_CUSTOM_ID("OnOpen", eventidOnOpen, FireOpenEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnClose", eventidOnClose, FireCloseEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnInboxToCar", eventidOnInboxToCar, FireInboxToCarEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCarToRepo", eventidOnCarToRepo, FireCarToRepoEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnRepoToCar", eventidOnRepoToCar, FireRepoToCarEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCarToOutbox", eventidOnCarToOutbox, FireCarToOutboxEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnReaderToCar", eventidOnReaderToCar, FireReaderToCarEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCarToReader", eventidOnCarToReader, FireCarToReaderEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCarToCardPrinter", eventidOnCarToCardPrinter, FireCarToCardPrinterEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCardPrinterToCar", eventidOnCardPrinterToCar, FireCardPrinterToCarEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCarToOCR", eventidOnCarToOCR, FireCarToOCREvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnOCRToCar", eventidOnOCRToCar, FireOCRToCarEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCarToEmboss", eventidOnCarToEmboss, FireCarToEmbossEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnEmbossToCar", eventidOnEmbossToCar, FireEmbossToCarEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCarStatus", eventidOnCarStatus, FireCarStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnInboxStatus", eventidOnInboxStatus, FireInboxStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnRepoStatus", eventidOnRepoStatus, FireRepoStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnOutboxStatus", eventidOnOutboxStatus, FireOutboxStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnCardPrinterStatus", eventidOnCardPrinterStatus, FireCardPrinterStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnOCRStatus", eventidOnOCRStatus, FireOCRStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnEmbossStatus", eventidOnEmbossStatus, FireEmbossStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnReaderStatus", eventidOnReaderStatus, FireReaderStatusEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnReset", eventidOnReset, FireResetEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnReaderReadSSInfo", eventidOnReaderReadSSInfo, FireReaderReadSSInfoEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnReaderReadBKNum", eventidOnReaderReadBKNum, FireReaderReadBKNumEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnRepoUsedNum", eventidOnRepoUsedNum, FireRepoUsedNumEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnRepoUsedSlots", eventidOnRepoUsedSlots, FireRepoUsedSlotsEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnReaderCardIn", eventidOnReaderCardIn, FireReaderCardInEvent, VTS_BSTR)
	EVENT_CUSTOM_ID("OnReaderCardOut", eventidOnReaderCardOut, FireReaderCardOutEvent, VTS_BSTR)
END_EVENT_MAP()

// 属性页

// TODO: 根据需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CGeneralCardDispenserCtrl, 1)
	PROPPAGEID(CGeneralCardDispenserPropPage::guid)
END_PROPPAGEIDS(CGeneralCardDispenserCtrl)

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CGeneralCardDispenserCtrl, "GENERALCARDDISPE.GeneralCardDispeCtrl.1",
	0xf2099ae7, 0xa01c, 0x4c16, 0x82, 0x42, 0xbd, 0x34, 0x3d, 0xfe, 0x38, 0x5b)

// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CGeneralCardDispenserCtrl, _tlid, _wVerMajor, _wVerMinor)

// 接口 ID

const IID IID_DGeneralCardDispenser = { 0x5785410C, 0xF4C1, 0x44AF, { 0x8B, 0x60, 0xE8, 0xD7, 0x62, 0xB4, 0x61, 0xCD } };
const IID IID_DGeneralCardDispenserEvents = { 0xDAC5958B, 0xCE78, 0x45DA, { 0x9D, 0xEF, 0x22, 0x87, 0xA7, 0x53, 0xD3, 0x82 } };

// 控件类型信息

static const DWORD _dwGeneralCardDispenserOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CGeneralCardDispenserCtrl, IDS_GENERALCARDDISPENSER, _dwGeneralCardDispenserOleMisc)

// CGeneralCardDispenserCtrl::CGeneralCardDispenserCtrlFactory::UpdateRegistry -
// 添加或移除 CGeneralCardDispenserCtrl 的系统注册表项

BOOL CGeneralCardDispenserCtrl::CGeneralCardDispenserCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO:  验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_GENERALCARDDISPENSER,
			IDB_GENERALCARDDISPENSER,
			afxRegApartmentThreading,
			_dwGeneralCardDispenserOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CGeneralCardDispenserCtrl::CGeneralCardDispenserCtrl - 构造函数

CGeneralCardDispenserCtrl::CGeneralCardDispenserCtrl()
{
	InitializeIIDs(&IID_DGeneralCardDispenser, &IID_DGeneralCardDispenserEvents);
	// TODO:  在此初始化控件的实例数据。
}

// CGeneralCardDispenserCtrl::~CGeneralCardDispenserCtrl - 析构函数

CGeneralCardDispenserCtrl::~CGeneralCardDispenserCtrl()
{
	// TODO:  在此清理控件的实例数据。
}

// CGeneralCardDispenserCtrl::OnDraw - 绘图函数

void CGeneralCardDispenserCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO:  用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CGeneralCardDispenserCtrl::DoPropExchange - 持久性支持

void CGeneralCardDispenserCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}

// CGeneralCardDispenserCtrl::OnResetState - 将控件重置为默认状态

void CGeneralCardDispenserCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO:  在此重置任意其他控件状态。
}

BOOL isWorking()
{
	return gIsWorking;
}

void SetIsWorking(BOOL b)
{
	gIsWorking = b;
}

// CGeneralCardDispenserCtrl 消息处理程序
// 处理异步自定义消息
LRESULT CGeneralCardDispenserCtrl::OnMsgTask(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TID_OPEN:
		FireOpenEvent(gEventMsg);
		break;
	case TID_CLOSE:
		FireCloseEvent(gEventMsg);
		break;
	case TID_INBOX_TO_CAR:
		FireInboxToCarEvent(gEventMsg);
		break;
	case TID_CAR_TO_REPO:
		FireCarToRepoEvent(gEventMsg);
		break;
	case TID_REPO_TO_CAR:
		FireRepoToCarEvent(gEventMsg);
		break;
	case TID_CAR_TO_OUTBOX:
		FireCarToOutboxEvent(gEventMsg);
		break;
	case TID_READER_TO_CAR:
		FireReaderToCarEvent(gEventMsg);
		break;
	case TID_CAR_TO_READER:
		FireCarToReaderEvent(gEventMsg); 
		break;
	case TID_CAR_TO_CARD_PRINTER:
		FireCarToCardPrinterEvent(gEventMsg);
		break;
	case TID_CARD_PRINTER_TO_CAR:
		FireCardPrinterToCarEvent(gEventMsg);
		break;
	case TID_CAR_TO_OCR:
		FireCarToOCREvent(gEventMsg);
		break;
	case TID_OCR_TO_CAR:
		FireOCRToCarEvent(gEventMsg);
		break;
	case TID_CAR_TO_EMBOSS:
		FireCarToEmbossEvent(gEventMsg);
		break;
	case TID_EMBOSS_TO_CAR:
		FireEmbossToCarEvent(gEventMsg);
		break;
	case TID_CAR_STATUS:
		FireCarStatusEvent(gEventMsg);
		break;
	case TID_INBOX_STATUS:
		FireInboxStatusEvent(gEventMsg);
		break;
	case TID_REPO_STATUS:
		FireRepoStatusEvent(gEventMsg);
		break;
	case TID_OUTBOX_STATUS:
		FireOutboxStatusEvent(gEventMsg);
		break;
	case TID_CARD_PRINTER_STATUS:
		FireCardPrinterStatusEvent(gEventMsg);
		break;
	case TID_OCR_STATUS:
		FireOCRStatusEvent(gEventMsg);
		break;
	case TID_EMBOSS_STATUS:
		FireEmbossStatusEvent(gEventMsg);
		break;
	case TID_READER_STATUS:
		FireReaderStatusEvent(gEventMsg);
		break;
	case TID_RESET:
		FireResetEvent(gEventMsg);
		break;
	case TID_READER_READ_SS_INFO:
		FireReaderReadSSInfoEvent(gEventMsg);
		break;
	case TID_READER_READ_BK_NUM:
		FireReaderReadBKNumEvent(gEventMsg);
		break;
	case TID_REPO_USED_NUM:
		FireRepoUsedNumEvent(gEventMsg);
		break;
	case TID_REPO_USED_SLOTS:
		FireRepoUsedSlotsEvent(gEventMsg);
		break;
	case TID_READER_CARD_IN:
		FireReaderCardInEvent(gEventMsg);
		break;
	case TID_READER_CARD_OUT:
		FireReaderCardOutEvent(gEventMsg);
		break;
	default:
		break;
	}
	SetIsWorking(FALSE);
	return 0;
}

int AddStatus(Json::Value &v, RetStatus &rs)
{
	if (rs.msg.length() != 0)
	{
		v["stat"] = rs.code;
		v["level"] = rs.level;
		v["msg"] = rs.msg;
	}
	else
	{
		v["stat"] = -1;
		v["level"] = EL_SOFT;
		v["msg"] = "控件未知错误";
	}
	return 0;
}

int Json2CString(Json::Value v, CString &cs)
{
	string s = v.toStyledString();
	USES_CONVERSION;
	cs = A2T(s.c_str());
	return 0;
}


int BuildEventMsg(Json::Value v, RetStatus rs)
{
	AddStatus(v, rs);
	Json2CString(v, gEventMsg);
	return 0;
}

UINT TaskOpen(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ctl = (CGeneralCardDispenserCtrl*)pParam;
	Json::Value v;
	RetStatus rs;
	int ret = GCDOpen(&rs);
	BuildEventMsg(v, rs);
	PostMessage(ctl->GetSafeHwnd(),WM_TASK, TID_OPEN, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::Open()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult=_T("0|0|成功");

	if (isWorking())
	{
		strResult= _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskOpen, this);
	}

	return strResult.AllocSysString();
}

UINT TaskClose(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDClose(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CLOSE, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskClose, this);
	}

	return strResult.AllocSysString();
}

UINT TaskInboxToCar(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDInboxToCar(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_INBOX_TO_CAR, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::InboxToCar()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskInboxToCar, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCarToRepo(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	int posit = 0;
	GCDCarToRepo(posit,&rs);
	Json::Value v;
	v["location"] = posit;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_TO_REPO, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CarToRepo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskCarToRepo, this);
	}

	return strResult.AllocSysString();
}

UINT TaskRepoToCar(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDRepoToCar(gLocation, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_REPO_TO_CAR, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::RepoToCar(LONG location)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gLocation=(int)location;
		AfxBeginThread(TaskRepoToCar, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCarToOutbox(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDCarToOutbox(gBoxnum, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_TO_OUTBOX, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CarToOutbox(LONG num)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gBoxnum = (int)num;
		AfxBeginThread(TaskCarToOutbox, this);
	}

	return strResult.AllocSysString();
}

UINT TaskReaderToCar(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDReaderToCar(gReaderId, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_TO_CAR, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::ReaderToCar(LONG readerId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gReaderId = (int)readerId;
		AfxBeginThread(TaskReaderToCar, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCarToReader(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDCarToReader(gReaderId, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_TO_READER, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CarToReader(LONG readerId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gReaderId = (int)readerId;
		AfxBeginThread(TaskCarToReader, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCarToCardPrinter(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDCarToCardPrinter(gCardPrinterPort, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_TO_CARD_PRINTER, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CarToCardPrinter(LONG port)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gCardPrinterPort = (int)port;
		AfxBeginThread(TaskCarToCardPrinter, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCardPrinterToCar(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDCardPrinterToCar(gCardPrinterPort, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_TO_CARD_PRINTER, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CardPrinterToCar(LONG port)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gCardPrinterPort = (int)port;
		AfxBeginThread(TaskCardPrinterToCar, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCarToOCR(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDCarToOCR(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_TO_OCR, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CarToOCR()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskCarToOCR, this);
	}
	return strResult.AllocSysString();
}

UINT TaskOCRToCar(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDOCRToCar(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_OCR_TO_CAR, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::OCRToCar()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskOCRToCar, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCarToEmboss(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDCarToEmboss(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_TO_EMBOSS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CarToEmboss()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskCarToEmboss, this);
	}

	return strResult.AllocSysString();
}

UINT TaskEmbossToCar(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDEmbossToCar(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_EMBOSS_TO_CAR, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::EmbossToCar()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskEmbossToCar, this);
	}
	return strResult.AllocSysString();
}

UINT TaskCarStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	stCardStatus cs;
	GCDCarStatus(cs, &rs);
	Json::Value v;
	if (rs.code == 0)
	{
		v["hasCard"] = cs.hasCard;
	}
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CAR_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CarStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskCarStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskInboxStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	stInboxStatus is = { 0, {0},{0} };
	GCDInboxStatus(is, &rs);
	Json::Value v;
	if (rs.code == 0)
	{
		for (int i = 0; i < is.num; i++)
		{
			v["boxstats"]["id"]=is.ids[i];
			v["boxstats"]["stat"] = is.stat[i];
		}
	}
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_INBOX_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::InboxStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskInboxStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskRepoStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	stRepoStatus reps;
	GCDRepoStatus(reps, &rs);
	Json::Value v;
	if (rs.code == 0)
	{
		v["isFull"] = reps.isFull;
	}
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_REPO_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::RepoStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskRepoStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskOutboxStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	stOutboxStatus os = { 0, {0}, {0} };
	GCDOutboxStatus(os, &rs);
	Json::Value v;
	if (rs.code == 0)
	{
		for (int i = 0; i < os.num; i++)
		{
			v["boxstats"]["id"] = os.ids[i];
			v["boxstats"]["stat"] = os.stat[i];
		}
	}
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_OUTBOX_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::OutboxStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskOutboxStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskCardPrinterStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDCardPrinterStatus(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_CARD_PRINTER_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::CardPrinterStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskCardPrinterStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskReaderStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	stReaderStatus reas;
	GCDReaderStatus(gReaderId, reas, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::ReaderStatus(LONG id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gReaderId = (int)id;
		AfxBeginThread(TaskReaderStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskOCRStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDOCRStatus(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_OCR_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::OCRStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskOCRStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskEmbossStatus(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDEmbossStatus(&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_EMBOSS_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::EmbossStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskEmbossStatus, this);
	}

	return strResult.AllocSysString();
}

UINT TaskReset(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	GCDReset(0, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_EMBOSS_STATUS, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::Reset(LONG type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskReset, this);
	}

	return strResult.AllocSysString();
}

UINT TaskReaderReadSSInfo(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	SSCBaseInfo info;
	GCDReaderReadSSInfo(gZone, gReaderId, info, &rs);
	Json::Value v;
	if (rs.code == 0)
	{
		v["ksbm"] = StringUtil::CString2String(info.ksbm);
		v["name"] = StringUtil::CString2String(info.name);
		v["sex"] = StringUtil::CString2String(info.sex);
		v["ssno"] = StringUtil::CString2String(info.ssno);
		v["idnum"] = StringUtil::CString2String(info.idnum);
		v["fkrq"] = StringUtil::CString2String(info.fkrq);
		v["yxrq"] = StringUtil::CString2String(info.yxrq);
		v["random"] = StringUtil::CString2String(info.random);
	}
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_READ_SS_INFO, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::ReaderReadSSInfo(LONG zone)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gZone = (int)zone;
		AfxBeginThread(TaskReaderReadSSInfo, this);
	}

	return strResult.AllocSysString();
}

UINT TaskReaderReadBKNum(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	CString bknum;
	GCDReaderReadBKNum(gReaderId, bknum, &rs);
	Json::Value v;
	if (rs.code == 0)
	{
		v["bknum"] = StringUtil::CString2String(bknum);
	}
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_READ_BK_NUM, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::ReaderReadBKNum()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskReaderReadBKNum, this);
	}

	return strResult.AllocSysString();
}

UINT TaskRepoUsedNum(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	int num = 0;
	GCDRepoUsedNum(num, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_READ_BK_NUM, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::RepoUsedNum()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskRepoUsedNum, this);
	}

	return strResult.AllocSysString();
}

UINT TaskRepoUsedSlots(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	vector<UINT16> slots;
	GCDRepoUsedSlots(slots,&rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_READ_BK_NUM, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::RepoUsedSlots()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		AfxBeginThread(TaskRepoUsedSlots, this);
	}

	return strResult.AllocSysString();
}

UINT TaskReaderCardIn(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	vector<UINT16> slots;
	GCDReaderCardIn(gReaderId, gTimeout, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_CARD_IN, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::ReaderCardIn(LONG readerId, LONG timeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gReaderId = (int)readerId;
		gTimeout = (int)timeout;
		AfxBeginThread(TaskReaderCardIn, this);
	}


	return strResult.AllocSysString();
}

UINT TaskReaderCardOut(LPVOID pParam)
{
	CGeneralCardDispenserCtrl* ocx = (CGeneralCardDispenserCtrl*)pParam;
	RetStatus rs;
	vector<UINT16> slots;
	GCDReaderCardOut(gReaderId, gTimeout, &rs);
	Json::Value v;
	BuildEventMsg(v, rs);
	PostMessage(ocx->GetSafeHwnd(), WM_TASK, TID_READER_CARD_OUT, 0);
	return 0;
}

BSTR CGeneralCardDispenserCtrl::ReaderCardOut(LONG readerId, LONG timeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = _T("0|0|成功");

	if (isWorking())
	{
		strResult = _T("3|-1|有任务正在执行");
	}
	else
	{
		SetIsWorking(TRUE);
		gReaderId = (int)readerId;
		gTimeout = (int)timeout;
		AfxBeginThread(TaskReaderCardOut, this);
	}


	return strResult.AllocSysString();
}

