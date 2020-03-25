#pragma once
#include <string>

using namespace std;

typedef struct {
	int code;
	int level;	// �������ؼ���
	string msg;
} RetStatus;

// �������ؼ���
enum ErrLevel
{
	EL_OK,		// �޴���
	EL_DANGER, //Σ�մ�����Ҫ��������
	EL_HARDWARE,	// Ӳ������
	EL_SOFT,	// �������
	EL_WARNING,	// ����
};

extern const RetStatus RS_OK;
extern const RetStatus RS_DISPENSER_NOT_OPEN;
extern const RetStatus RS_UNKOWN;
extern const RetStatus  RS_MODULE_NOT_EXIST;	// ģ�鲻����
extern const RetStatus RS_PARAM_NOT_SUPPORT;

//void RetStatusOK(RetStatus *stat);