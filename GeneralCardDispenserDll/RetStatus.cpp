#include "stdafx.h"
#include "RetStatus.h"

const RetStatus RS_OK = { 0, EL_OK, "�ɹ�" };

const RetStatus RS_DISPENSER_NOT_OPEN = { -1, EL_SOFT, "�쿨ģ��δ��" };

const RetStatus RS_UNKOWN = { -1, EL_SOFT, "δ֪����" };

const RetStatus RS_MODULE_NOT_EXIST = { -1, EL_HARDWARE, "ģ�鲻����" };

const RetStatus RS_PARAM_NOT_SUPPORT = { -1, EL_SOFT, "�ӿڵ��ô��δ���" };

