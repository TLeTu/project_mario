#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "WorldMario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	int sceneId = CGame::GetInstance()->GetCurrentSceneId();
	if (sceneId == 1)
	{
		CWorldMario* mario = (CWorldMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		switch (KeyCode)
		{
			case DIK_RIGHT:
				if (mario->CheckMoveDirection(32,0))
					mario->Move(32, 0);
				break;
			case DIK_LEFT:
				if (mario->CheckMoveDirection(-32, 0))
					mario->Move(-32, 0);
				break;
			case DIK_UP:
				if (mario->CheckMoveDirection(0, -32))
				mario->Move(0, -32);
				break;
			case DIK_DOWN:
				if (mario->CheckMoveDirection(0, 32))
					mario->Move(0, 32);
				break;
			case DIK_A:
				if (mario->GetSceneId() != -1)
					CGame::GetInstance()->InitiateSwitchScene(mario->GetSceneId(), -1, -1);
				break;
		}

	}
	else
	{
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		switch (KeyCode)
		{
		case DIK_DOWN:
			mario->SetState(MARIO_STATE_SIT);
			break;
		case DIK_S:
			mario->SetState(MARIO_STATE_JUMP);
			break;
		case DIK_D:
			if (mario->GetLevel() == MARIO_LEVEL_RACOON)
			{
				mario->SetState(MARIO_STATE_FLY);
			}
			break;
		case DIK_1:
			mario->SetLevel(MARIO_LEVEL_SMALL);
			break;
		case DIK_2:
			mario->SetLevel(MARIO_LEVEL_BIG);
			break;
		case DIK_0:
			mario->SetState(MARIO_STATE_DIE);
			break;
		case DIK_R: // reset
			CGame::GetInstance()->ReloadScene();
			break;
		case DIK_Q:
			if (mario->GetLevel() == MARIO_LEVEL_RACOON)
			{
				mario->SetState(MARIO_STATE_ATTACK);
				break;
			}
			else break;
		case DIK_H:
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(1000);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddLife(4);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddMoney(4);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddCard(200);
			break;
		case DIK_J:
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->DecreaseLife(1);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddCard(300);
			break;
		case DIK_K:
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddCard(400);
			break;

		}
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	int sceneId = CGame::GetInstance()->GetCurrentSceneId();
	if (sceneId == 1)
	{
		CWorldMario* mario = (CWorldMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	}
	else
	{
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		switch (KeyCode)
		{
		case DIK_S:
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
			break;
		case DIK_D:
			mario->SetState(MARIO_STATE_RELEASE_FLY);
			break;
		case DIK_DOWN:
			mario->SetState(MARIO_STATE_SIT_RELEASE);
			break;
		}
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	int sceneId = CGame::GetInstance()->GetCurrentSceneId();
	if (sceneId == 1)
	{
		CWorldMario* mario = (CWorldMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	}
	else
	{
		LPGAME game = CGame::GetInstance();
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (game->IsKeyDown(DIK_A))
			{
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			}
			else
			{
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}

		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			if (game->IsKeyDown(DIK_A))
			{
				mario->SetState(MARIO_STATE_RUNNING_LEFT);
			}
			else
			{
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
		}
		else
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
		float _, vy;
		mario->GetSpeed(_, vy);
		if (mario->GetLevel() == MARIO_LEVEL_RACOON && vy > 0)
		{
			if (game->IsKeyDown(DIK_F))
			{
				mario->SetState(MARIO_STATE_FLAP);
			}
			else
			{
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
			}
		}
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetHoldingA(true);
		}
		else
		{
			mario->SetHoldingA(false);
		}
		if (game->IsKeyDown(DIK_UP))
		{
			mario->SetHoldingUp(true);
		}
		else
		{
			mario->SetHoldingUp(false);
		}
	}
}