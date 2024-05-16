#include "Koopas.h"
#include "EdgeDetector.h"

CKoopas::CKoopas(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	this->edgeDetector = NULL;
	restore_start = -1;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_HIDE)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_HIDE / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_HIDE;
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == KOOPAS_STATE_WALKING && edgeDetector == NULL) {
		if (vx > 0)
		{
			edgeDetector = new CEdgeDetector(x + 16, y);
		}
		else 
		{
			edgeDetector = new CEdgeDetector(x - 16, y);
		}
	}

	if ((state == KOOPAS_STATE_HIDE) && (GetTickCount64() - restore_start > KOOPAS_RESTORE_TIMEOUT) && (edgeDetector != NULL))
	{
		edgeDetector->Delete();
		edgeDetector = NULL;
		SetState(KOOPAS_STATE_WALKING);
	}

	if (state == KOOPAS_STATE_WALKING && edgeDetector != NULL)
	{
		edgeDetector->Update(dt, coObjects);
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
	if (state == KOOPAS_STATE_HIDE)
	{
		aniId = ID_ANI_KOOPAS_HIDE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
	if (edgeDetector != NULL)
	{
		edgeDetector->Render();
	}
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_HIDE:
		restore_start = GetTickCount64();
		y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_HIDE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPAS_STATE_WALKING:
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_HIDE) / 2;
		vx = -KOOPAS_WALKING_SPEED;
		break;
	}
}
