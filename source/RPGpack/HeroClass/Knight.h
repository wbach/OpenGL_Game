#pragma once
#include "../Player.h"

class CKnight : virtual public CHero
{
public:
	//CKnight();
	virtual void Create(CScene* scene) override;
};