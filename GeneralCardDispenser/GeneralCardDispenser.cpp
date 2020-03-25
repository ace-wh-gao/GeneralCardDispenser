// GeneralCardDispenser.cpp : CGeneralCardDispenserApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "GeneralCardDispenser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGeneralCardDispenserApp theApp;

const GUID CDECL _tlid = { 0x6E41FA2A, 0x4E46, 0x4A02, { 0xB1, 0x8A, 0xCD, 0x6, 0xD2, 0x23, 0xEE, 0x52 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CGeneralCardDispenserApp::InitInstance - DLL 初始化

BOOL CGeneralCardDispenserApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO:  在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CGeneralCardDispenserApp::ExitInstance - DLL 终止

int CGeneralCardDispenserApp::ExitInstance()
{
	// TODO:  在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
