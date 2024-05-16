#include "Koopas.h"
#include "EdgeDetector.h"

CKoopas::CKoopas(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	this->detecting = 0;
	this->edgeDetector = new CEdgeDetector();
	restore_start = -1;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_SHELL)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
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

	if (state == KOOPAS_STATE_WALKING && !detecting) {
		if (vx > 0)
		{
			edgeDetector->SetPosition(x + 16, y);
			edgeDetector->SetSpeed(vx, vy);
			detecting = 1;
		}
		else 
		{
			edgeDetector->SetPosition(x - 16, y);
			edgeDetector->SetSpeed(vx, vy);
			detecting = 1;
		}
	}

	if ((state == KOOPAS_STATE_SHELL) && (GetTickCount64() - restore_start > KOOPAS_RESTORE_TIMEOUT) && detecting)
	{

		SetState(KOOPAS_STATE_WALKING);
		detecting = 0;
	}

	if (state == KOOPAS_STATE_WALKING && detecting)
	{
		edgeDetector->Update(dt, coObjects);
		float edt_vx, edt_vy;
		edgeDetector->GetSpeed(edt_vx, edt_vy);
		if (edt_vy > 0) 
		{
			detecting = 0;
			vx = -vx;
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
	case KOOPAS_STATE_SHELL:
		restore_start = GetTickCount64();
		y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		vx_temp = vx;
		vx = 0;
		vy = 0;
		ay = 0;
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
	}
}
