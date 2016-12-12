#pragma once
#include "Model.h"
#include "TextureLoader.h"
class CEmptyLoader : public CModel
{
public:
	CEmptyLoader();
	virtual void InitModel(string file_name) override;
};
