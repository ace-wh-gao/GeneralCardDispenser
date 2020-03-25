#pragma once
#include <string>

using namespace std;

typedef struct {
	int code;
	int level;	// 错误严重级别
	string msg;
} RetStatus;

// 错误严重级别
enum ErrLevel
{
	EL_OK,		// 无错误
	EL_DANGER, //危险错误，需要紧急处理
	EL_HARDWARE,	// 硬件错误
	EL_SOFT,	// 软件错误
	EL_WARNING,	// 警告
};

extern const RetStatus RS_OK;
extern const RetStatus RS_DISPENSER_NOT_OPEN;
extern const RetStatus RS_UNKOWN;
extern const RetStatus  RS_MODULE_NOT_EXIST;	// 模块不存在
extern const RetStatus RS_PARAM_NOT_SUPPORT;

//void RetStatusOK(RetStatus *stat);