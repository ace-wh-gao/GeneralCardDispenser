#include "stdafx.h"
#include "RetStatus.h"

const RetStatus RS_OK = { 0, EL_OK, "成功" };

const RetStatus RS_DISPENSER_NOT_OPEN = { -1, EL_SOFT, "领卡模块未打开" };

const RetStatus RS_UNKOWN = { -1, EL_SOFT, "未知错误" };

const RetStatus RS_MODULE_NOT_EXIST = { -1, EL_HARDWARE, "模块不存在" };

const RetStatus RS_PARAM_NOT_SUPPORT = { -1, EL_SOFT, "接口调用传参错误" };

