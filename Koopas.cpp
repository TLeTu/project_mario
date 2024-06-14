#include "Koopas.h"
#include "EdgeDetector.h"
#include "Goomba.h"
#include "Box.h"
#include "debug.h"
#include "Brick.h"

CKoopas::CKoopas(float x, float y, int type, int color) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	this->detecting = 0;
	this->edgeDetector = new CEdgeDetector(x - 18, y);
	CGame::GetInstance()->GetCurrentScene()->AddGameObject(edgeDetector);
	this->spinDirection = -1;
	isOnPlatform = false;
	isCarried = false;
	restore_start = -1;
	walk_start = -1;
	this->koopaType = type;
	this->koopaColor = color;
	this->vx_temp = -KOOPAS_WALKING_SPEED;
	if (koopaType)
	{
		SetState(KOOPAS_STATE_JUMP);
	}
	else
	{
		SetState(KOOPAS_STATE_WALKING);
	}
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_JUMP)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking() && GetState() == KOOPAS_STATE_JUMP)
	{
		vy = 0;
		SetState(KOOPAS_STATE_TAKEOFF);
	}
	else if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CBox*>(e->obj))
		OnCollisionWithBox(e);
	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
}

void CKoopas::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (e->ny == 0 && GetState() == KOOPAS_STATE_SPIN)
	{
		if (brick->GetState() == BRICK_STATE_NORMAL)
		{
			brick->SetState(BRICK_STATE_BREAK);
		}
		else return;
	}
	else return;
}

void CKoopas::OnCollisionWithBox(LPCOLLISIONEVENT e)
{
	CBox* box = dynamic_cast<CBox*>(e->obj);
	if (e->ny == 0)
	{
		if (box->GetState() != BOX_STATE_EMPTY)
		{
			if (box->GetState() == BOX_STATE_MUSHROOM)
			{
				box->SpawnMushroom();
				box->SetState(BOX_STATE_EMPTY);
			}
			else
			{
				box->SetState(BOX_STATE_EMPTY);
				//coin++;
			}
		}
	}
	else return;
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetState() != GOOMBA_STATE_DIE && GetState() == KOOPAS_STATE_SPIN)
	{
			goomba->SetState(GOOMBA_STATE_DIE);
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;


	if (isCarried)
	{
		ay = 0;
	}
	//isOnPlatform = false;

	DebugOut(L"Koopa: %d\n", isOnPlatform);

	if (state == KOOPAS_STATE_WALKING && !detecting && isOnPlatform) 
	{
		if (vx > 0)
		{
			edgeDetector->SetPosition(x + 16, y);
			edgeDetector->SetSpeed(vx, 0);
			detecting = 1;
		}
		else
		{
			edgeDetector->SetPosition(x - 16, y);
			edgeDetector->SetSpeed(vx, 0);
			detecting = 1;
		}
	}

	if (state == KOOPAS_STATE_SHELL && detecting) 
	{
		detecting = 0;
		edgeDetector->SetSpeed(0, 0);
	}

	if ((state == KOOPAS_STATE_SHELL) && (GetTickCount64() - restore_start > KOOPAS_SHELL_TIMEOUT))
	{
		SetState(KOOPAS_STATE_RESTORE);
	}
	if ((state == KOOPAS_STATE_RESTORE) && (GetTickCount64() - restore_start > KOOPAS_RESTORE_TIMEOUT))
	{
		SetState(KOOPAS_STATE_WALKING);
	}

	if (state == KOOPAS_STATE_WALKING && detecting)
	{
		float edt_vx, edt_vy;
		edgeDetector->GetSpeed(edt_vx, edt_vy);
		if (edt_vy != 0) 
		{
			detecting = 0;
			vx = -vx;
			edgeDetector->SetSpeed(vx, 0);
		}
	}
	if (state == KOOPAS_STATE_TAKEOFF && (GetTickCount64() - walk_start > 100))
	{
		SetState(KOOPAS_STATE_JUMP);
	}
	if (state == KOOPAS_STATE_IDLE && (GetTickCount64() - walk_start > 200))
	{
		SetState(KOOPAS_STATE_WALKING);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CKoopas::Render()
{
	int aniId = -1;
	if (koopaColor)
	{
		if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_IDLE)
		{
			if (vx > 0)
			{
				aniId = ID_ANI_KOOPAS_FLIP;
			}
			if (vx < 0)
			{
				aniId = ID_ANI_KOOPAS_WALKING;
			}
		}
		if (state == KOOPAS_STATE_JUMP || state == KOOPAS_STATE_TAKEOFF)
		{
			if (vx > 0)
			{
				aniId = ID_ANI_KOOPAS_JUMP_FLIP;
			}
			if (vx < 0)
			{
				aniId = ID_ANI_KOOPAS_JUMP;
			}
		}
		if (state == KOOPAS_STATE_SHELL)
		{
			aniId = ID_ANI_KOOPAS_SHELL;
		}
		if (state == KOOPAS_STATE_SPIN)
		{
			aniId = ID_ANI_KOOPAS_SHELL;
		}
		if (state == KOOPAS_STATE_RESTORE)
		{
			aniId = ID_ANI_KOOPAS_RESTORING;
		}
	}
	else 
	{
		if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_IDLE)
		{
			if (vx > 0)
			{
				aniId = ID_ANI_KOOPAS_GREEN_FLIP;
			}
			if (vx < 0)
			{
				aniId = ID_ANI_KOOPAS_GREEN_WALKING;
			}
		}
		if (state == KOOPAS_STATE_JUMP || state == KOOPAS_STATE_TAKEOFF)
		{
			if (vx > 0)
			{
				aniId = ID_ANI_KOOPAS_JUMP_FLIP;
			}
			if (vx < 0)
			{
				aniId = ID_ANI_KOOPAS_JUMP;
			}
		}
		if (state == KOOPAS_STATE_SHELL)
		{
			aniId = ID_ANI_KOOPAS_GREEN_SHELL;
		}
		if (state == KOOPAS_STATE_SPIN)
		{
			aniId = ID_ANI_KOOPAS_GREEN_SHELL;
		}
		if (state == KOOPAS_STATE_RESTORE)
		{
			aniId = ID_ANI_KOOPAS_GREEN_RESTORING;
		}

	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	int oldState = this->state;
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
		restore_start = GetTickCount64();
		y -= (KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		vx_temp = vx;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL);
		if (vx_temp > 0) 
		{
			vx = KOOPAS_WALKING_SPEED;
		}
		else {
			vx = -KOOPAS_WALKING_SPEED;
		}
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_SPIN:
		y -= (KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		ay = KOOPAS_GRAVITY;
		if (spinDirection > 0)
			vx = KOOPAS_SPINNING_SPEED;
		else
			vx = -KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_RESTORE:
		restore_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_JUMP:
		vx = -KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_JUMP_SPEED;
		ay = KOOPAS_GRAVITY - 0.0015f;
		break;
	case KOOPAS_STATE_TAKEOFF:
		walk_start = GetTickCount64();
		break;
	case KOOPAS_STATE_IDLE:
		vy = 0;
		ay = KOOPAS_GRAVITY;
		isOnPlatform = false;
		walk_start = GetTickCount64();
		break;
	}
}
