#pragma once
#include "../../BaseEngine/Engine/GLgame.h"
#include "../Scenes/MainScene.h"

class CRPGGame : public CGame
{
public:
	CRPGGame();

private:
	std::shared_ptr<CApi> m_Api;
};