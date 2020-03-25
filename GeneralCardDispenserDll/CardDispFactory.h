#pragma once
#include "BaseCardDispenser.h"
#include <memory>

using namespace std;

//设备型号
#define CD_MODEL_CZ_QZ05CT3 1		// 创自QZ05-CT

class CardDispFactory
{
public:
	CardDispFactory();
	~CardDispFactory();

	int CreateDev(shared_ptr<BaseCardDispenser> &pbcd);
	
private:
	int GetDevModel();
};

