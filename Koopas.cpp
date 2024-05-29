#include "Koopas.h"
#include "EdgeDetector.h"
#include "Goomba.h"
#include "Box.h"
#include "debug.h"

CKoopas::CKoopas(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	this->detecting = 0;
	this->edgeDetector = new CEdgeDetector(x - 18, y);
	CGame::GetInstance()->GetCurrentScene()->AddGameObject(edgeDetector);
	this->spinDirection = -1;
	restore_start = -1;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_WALKING)
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
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CBox*>(e->obj))
		OnCollisionWithBox(e);
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

	//debug out y
	//DebugOut(L"y: %f\n", y);

	if (state == KOOPAS_STATE_WALKING && !detecting) {
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


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CKoopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING;
	if (vx > 0) 
	{
		aniId = ID_ANI_KOOPAS_FLIP;
	}
	if (vx < 0)
	{
		aniId = ID_ANI_KOOPAS_WALKING;
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
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		if (vx_temp > 0) 
		{
			vx = KOOPAS_WALKING_SPEED;
		}
		else {
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
	case KOOPAS_STATE_SPIN:
		y -= (KOOPAS_BBOX_HEIGHT_SHELL) / 2;
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
	}
}
