#pragma once
#include "BaseCardDispenser.h"
#include <memory>

using namespace std;

//�豸�ͺ�
#define CD_MODEL_CZ_QZ05CT3 1		// ����QZ05-CT

class CardDispFactory
{
public:
	CardDispFactory();
	~CardDispFactory();

	int CreateDev(shared_ptr<BaseCardDispenser> &pbcd);
	
private:
	int GetDevModel();
};

