#pragma once
#include <windows.h>
#include "Engine.h"
#include "Function.h"

class Game
{
public:
	void Update();

	bool CheckAllPtr();

	Engine::CGame* pCGame = nullptr;

	Engine::GameDI* pGameDI = nullptr;

	Engine::SessionCooperativeDI* Session = nullptr;

	Engine::LevelDI* pLevelDI = nullptr;

	Engine::LocalClientDI* pLocalClientDI = nullptr;

	Engine::CameraManagerDI* pCameraManage = nullptr;

}*pGame;

void Game::Update()
{
	this->pCGame = Engine::CGame::GetInstance();

	if (this->pGameDI == nullptr)
	{
		this->pGameDI = this->pCGame->GameDI_;
	}
	
	if (this->pGameDI == nullptr)
		return;

	this->Session = pGameDI->Session_;

	if (this->Session == nullptr)
		return;

	this->pLevelDI = this->Session->LevelDI_;
	this->pLocalClientDI = this->Session->LocalClientDI_;
	this->pCameraManage = this->Session->CameraManager_;
}

bool Game::CheckAllPtr()
{
	if (this->pGameDI ==nullptr)
		return false;
	if (this->Session == nullptr)
		return false;

	if (this->pLocalClientDI == nullptr)
		return false;
	if (this->pLocalClientDI->Player == nullptr)
		return false;

	if (this->pCameraManage == nullptr)
		return false;
	if (this->pCameraManage->Camera == nullptr)
		return false;

	if (this->pLevelDI == nullptr)
		return false;
	if (this->pLevelDI->Level == nullptr)
		return false;

	return true;
}