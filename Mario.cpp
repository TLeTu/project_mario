#include <algorithm>
#include "debug.h"
#include "PlayScene.h"
#include "PlayScene.h"


#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"

#include "Koopas.h"

#include "Box.h"
#include "Mushroom.h"
#include "Fireball.h"

#include "Piranha.h"
#include "SceneLoader.h"
#include "Flytrap.h"
#include "Button.h"
#include "Brick.h"
#include "LuckyBox.h"
#include "Leaf.h"

#include "Floor.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (GetState() == MARIO_STATE_END) x += 1;

	//debug out mario y
	/*DebugOut(L"mario y: %f\n", y);*/
	//debug out mario ax
	//DebugOut(L"mario ax: %f\n", ax);
	//if (inPortal)
	//{
	//	//DebugOut(L">>> IN PORTAL >>> \n");
	//}

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isCarryingKoopas && carriedKoopas)
	{
		if (isHoldingA)
		{
			if (nx > 0)
				carriedKoopas->SetPosition(x + 20, y - 5);
			else
				carriedKoopas->SetPosition(x - 20, y - 5);
			if (carriedKoopas->GetState() == KOOPAS_STATE_WALKING) {
				if (this->level == MARIO_LEVEL_SMALL)
				{
					carriedKoopas->SetPosition(x, y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2);
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
				else
				{
					carriedKoopas->SetPosition(x, y);
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				isCarryingKoopas = false;
				carriedKoopas->SetCarried(0);
				carriedKoopas = nullptr;
			}
		}
		else
		{
			float kx, ky;
			carriedKoopas->GetPosition(kx, ky);
			if (x < kx)
			{
				carriedKoopas->SetPosition(kx + 16, y);
				carriedKoopas->SetSpinDirection(1);
			}
			else
			{
				carriedKoopas->SetPosition(kx - 16, y);
				carriedKoopas->SetSpinDirection(0);
			}
			carriedKoopas->SetState(KOOPAS_STATE_SPIN);
			isCarryingKoopas = false;
			carriedKoopas->SetCarried(0);
			carriedKoopas = nullptr;
		}
	}

	if (isSitting && inPortal)
	{
		if (portalId != -1)
		{
			CGame::GetInstance()->InitiateSwitchScene(portalId, px, py);
		}
	}
	else if (inPortal && isHoldingUp && vy > 0)
	{
		if (portalId != -1)
		{
			CGame::GetInstance()->InitiateSwitchScene(portalId, px, py);
		}
	}

	if (vx == MARIO_RUNNING_SPEED || vx == -MARIO_RUNNING_SPEED)
	{
		chargedFlying = true;
	}
	if (chargedFlying && vy > 0 && GetTickCount64() - fall_start > 100)
	{
		chargedFlying = false;
	}

	isOnPlatform = false;
	inPortal = false;
	portalId = -1;

	if (GetState() == MARIO_STATE_ATTACK)
	{

		LPGAMEOBJECT obj;
		if (nx > 0)
			obj = CGame::GetInstance()->GetCurrentScene()->GetEnemiesInRange(x + 22, y);
		else
			obj = CGame::GetInstance()->GetCurrentScene()->GetEnemiesInRange(x - 22, y);
		if (obj != NULL)
		{
			if (dynamic_cast<CGoomba*>(obj))
			{
				obj->SetState(GOOMBA_STATE_DIE);
			}
			else if (dynamic_cast<CKoopas*>(obj))
			{
				obj->SetState(KOOPAS_STATE_SHELL);
			}
		}
	}

	if (GetState() == MARIO_STATE_DIE && GetTickCount64() - die_start > 1000)
	{
		CGame::GetInstance()->GameOver();
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CBox*>(e->obj))
		OnCollisionWithBox(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CFireball*>(e->obj))
		OnCollisionWithFireball(e);
	else if (dynamic_cast<CPiranha*>(e->obj))
		OnCollisionWithPiranha(e);
	else if (dynamic_cast<CSceneLoader*>(e->obj))
		OnCollisionWithSceneLoader(e);
	else if (dynamic_cast<CFlytrap*>(e->obj))
		OnCollisionWithFlytrap(e);
	else if (dynamic_cast<CButton*>(e->obj))
		OnCollisionWithButton(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCOllisionWithLeaf(e);
	else if (dynamic_cast<CLuckyBox*>(e->obj))
		OnCollisionWithLuckyBox(e);

}

void CMario::OnCollisionWithLuckyBox(LPCOLLISIONEVENT e)
{
	float lx, ly;
	CLuckyBox* luckyBox = dynamic_cast<CLuckyBox*>(e->obj);
	luckyBox->GetPosition(lx, ly);
	if (luckyBox->GetState() != LUCKYBOX_STATE_EMPTY)
	{
		if (luckyBox->GetState() == LUCKYBOX_STATE_MUSHROOM)
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddCard(200);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GameEnd(lx, ly, 200);
		}
		else if (luckyBox->GetState() == LUCKYBOX_STATE_FLOWER)
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddCard(300);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GameEnd(lx, ly, 300);
		}
		else if (luckyBox->GetState() == LUCKYBOX_STATE_STAR)
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddCard(400);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GameEnd(lx, ly, 400);
		}
		luckyBox->SetRolling(false);
		luckyBox->SpawnCoin();
		luckyBox->SetState(LUCKYBOX_STATE_EMPTY);
		SetState(MARIO_STATE_END);
	}
	else return;
}

void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{

	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (e->ny > 0)
	{
		if (brick->GetState() == BRICK_STATE_NORMAL || brick->GetState() == BRICK_STATE_BUTTON)
		{
			float _, by;
			brick->GetPosition(_, by);
			brick->SetPosition(_, by - 8);
			brick->SetState(BRICK_STATE_ANI);
		}
	}
}

void CMario::OnCollisionWithButton(LPCOLLISIONEVENT e)
{
	CButton* btn = dynamic_cast<CButton*>(e->obj);
	if (btn->GetState() == BUTTON_STATE_NORMAL && e->ny != 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		btn->SetState(BUTTON_STATE_PRESSED);
	}
	else return;
}

void CMario::OnCollisionWithSceneLoader(LPCOLLISIONEVENT e)
{
	DebugOut(L">>> LOAD >>> \n");
	CSceneLoader* sl = dynamic_cast<CSceneLoader*>(e->obj);
	int part = sl->GetPart();
	CGame::GetInstance()->GetCurrentScene()->SetScenePart(part);
}

void CMario::OnCollisionWithPiranha(LPCOLLISIONEVENT e)
{
	CPiranha* piranha = dynamic_cast<CPiranha*>(e->obj);
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_BIG)
		{
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_BIG;
			StartUntouchable();

		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::OnCollisionWithFlytrap(LPCOLLISIONEVENT e)
{
	CFlytrap* flytrap = dynamic_cast<CFlytrap*>(e->obj);
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::OnCollisionWithFireball(LPCOLLISIONEVENT e)
{
	CFireball* fireball = dynamic_cast<CFireball*>(e->obj);
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (mushroom->GetState() == MUSHROOM_STATE)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			SetLevel(MARIO_LEVEL_BIG);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(1000);
			StartUntouchable();
			mushroom->Delete();
		}
		else
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(1000);
			mushroom->Delete();
		}
	}
	else
	{
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(1000);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddLife(1);
		mushroom->Delete();
	}
}

void CMario::OnCOllisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (level == MARIO_LEVEL_BIG)
	{
		SetLevel(MARIO_LEVEL_RACOON);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(100);
		StartUntouchable();
		leaf->Delete();
	}
	else
	{
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(100);
		leaf->Delete();
	}
}

void CMario::OnCollisionWithBox(LPCOLLISIONEVENT e)
{
	CBox* box = dynamic_cast<CBox*>(e->obj);
	if (e->ny > 0)
	{
		if (box->GetState() != BOX_STATE_EMPTY)
		{
			float _, by;
			box->GetPosition(_, by);
			box->SetPosition(_, by - 8);
			if (box->GetState() == BOX_STATE_MUSHROOM || box->GetState() == BOX_STATE_GREEN_MUSHROOM)
			{
				box->SpawnMushroom();
				box->SetState(BOX_STATE_ANI);
			}
			else
			{
				box->SpawnCoin();
				box->SetState(BOX_STATE_ANI);
				((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddMoney(1);
				((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(100);
			}
		}
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() == GOOMBA_STATE_WALKING)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(100);
		}
		else if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(100);
			goomba->SetState(GOOMBA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	if (e->ny < 0)
	{
		if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState() == KOOPAS_STATE_SPIN)
		{
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(100);
			koopas->SetState(KOOPAS_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopas->GetState() == KOOPAS_STATE_JUMP)
		{
			koopas->SetState(KOOPAS_STATE_IDLE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else 
		{
			float kx, ky;
			koopas->GetPosition(kx, ky);
			if (x < kx)
				koopas->SetSpinDirection(1);
			else
				koopas->SetSpinDirection(0);
			koopas->SetState(KOOPAS_STATE_SPIN);
		}
	}
	else
	{
		if (untouchable == 0)
		{
			if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState() == KOOPAS_STATE_JUMP)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
			else if (koopas->GetState() == KOOPAS_STATE_SHELL) 
			{
				if (isHoldingA)
				{
					isCarryingKoopas = true;
					carriedKoopas = koopas;
					koopas->SetCarried(1);
				}
				else 
				{
					koopas->SetSpinDirection(nx);
					koopas->SetState(KOOPAS_STATE_SPIN);
				}
			}
			else if (koopas->GetState() == KOOPAS_STATE_SPIN) {
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	CCoin* coin = dynamic_cast<CCoin*>(e->obj);
	if (coin->GetState() == COIN_STATE_NORMAL || coin->GetState() == COIN_STATE_TOBRICK)
	{
		coin->Delete();
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddMoney(1);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetUI()->AddScore(100);
	}
	else return;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	this->inPortal = true;
	this->portalId = dynamic_cast<CPortal*>(e->obj)->GetSceneId();
	this->px = dynamic_cast<CPortal*>(e->obj)->GetX();
	this->py = dynamic_cast<CPortal*>(e->obj)->GetY();
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRacoon()
{

	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;
	if (GetState() == MARIO_STATE_FLY)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACOON_FLY_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACOON_FLY_LEFT;
	}
	if (GetState() == MARIO_STATE_FLAP)
		{
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACOON_FLAP_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACOON_FLAP_LEFT;
		}
	if (GetState() == MARIO_STATE_ATTACK)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACOON_SPIN_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACOON_SPIN_LEFT;
	}
	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if ( level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();

	if (GetState() == MARIO_STATE_END)
	{
		if (GetLevel() == MARIO_LEVEL_SMALL)
			aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
		else if (GetLevel() == MARIO_LEVEL_BIG)
			aniId = ID_ANI_MARIO_WALKING_RIGHT;
		else if (GetLevel() == MARIO_LEVEL_RACOON)
			aniId = ID_ANI_MARIO_RACOON_WALKING_RIGHT;
	}

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE || this->state == MARIO_STATE_END)
	{
		return;
	}

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
			if (isOnPlatform)
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
				{
					vy = -MARIO_JUMP_RUN_SPEED_Y;
				}
				else
					vy = -MARIO_JUMP_SPEED_Y;
			}

		break;
	case MARIO_STATE_FLY:
		if (chargedFlying)
		{
			vy = float(-MARIO_JUMP_RUN_SPEED_Y / 1.5);
			fall_start = -1;
		}

		break;
		
	case MARIO_STATE_RELEASE_JUMP:
		ay = MARIO_GRAVITY;
		break;
	case MARIO_STATE_RELEASE_FLY:
		fall_start = GetTickCount64();
		ay = MARIO_GRAVITY;
		break;
	case MARIO_STATE_FLAP:
		ay = 0.0003f;
		break;
	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	
	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		die_start = GetTickCount64();
		break;
	case MARIO_STATE_END:
		vy = 0;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_ATTACK:
		attack_start = GetTickCount64();
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level==MARIO_LEVEL_RACOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

