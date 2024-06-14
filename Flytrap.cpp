#include "Flytrap.h"
#include "debug.h"

#include "Collision.h"

#include <cmath>

#include "Mario.h"


CFlytrap::CFlytrap(float x, float y) :CGameObject(x, y)
{
	this->marioIsNear = false;
	this->marioIsAbove = false;
	this->isReloading = true;
	this->shootingY = y;
	this->hidingY = y + 32;
	reload_start = -1;
	this->type = "piranha";
	SetState(FLYTRAP_STATE_DESCENDING);
}

void CFlytrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x - FLYTRAP_BBOX_WIDTH / 2;
	top = y - FLYTRAP_BBOX_HEIGHT / 2;
	right = left + FLYTRAP_BBOX_WIDTH;
	bottom = top + FLYTRAP_BBOX_HEIGHT;
}

void CFlytrap::OnNoCollision(DWORD dt)
{
	//x += vx * dt;
	//y += vy * dt;
};

void CFlytrap::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CFlytrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;

	CGame::GetInstance()->GetCurrentScene()->GetPlayerPosition(mx, my);

	//debug out mario mx my
	DebugOut(L"mx: %f, my: %f\n", mx, my);

	marioIsNear = abs(x - mx) <= 80;

	marioIsAbove = y > my && abs(x - mx) <= 16;

	if (GetState() == FLYTRAP_STATE_HIDING && marioIsNear && !marioIsAbove)
	{
			if (GetTickCount64() - reload_start > FLYTRAP_MOVING_TIME)
			{
				SetState(FLYTRAP_STATE_ASCENDING);
			}

	}
	if (GetState() == FLYTRAP_STATE_DESCENDING)
	{
		if (y < hidingY)
		{
			vy = 0.03f;
		}
		else
		{
			SetState(FLYTRAP_STATE_HIDING);
		}
	}
	if (GetState() == FLYTRAP_STATE_ASCENDING)
	{
		if (y > shootingY)
		{
			vy = -0.03f;
		}
		else
		{
			SetState(FLYTRAP_STATE_IDLE);
		}
	}
	if (GetState() == FLYTRAP_STATE_IDLE)
	{
			if (GetTickCount64() - reload_start > 3000)
			{
				SetState(FLYTRAP_STATE_DESCENDING);
			}
	}

	// Update other properties and process collisions
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}



void CFlytrap::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FLYTRAP)->Render(x, y);
	//RenderBoundingBox();
}

void CFlytrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLYTRAP_STATE_HIDING:
		vy = 0;
		reload_start = GetTickCount64();
		break;
	case FLYTRAP_STATE_ASCENDING:
		break;
	case FLYTRAP_STATE_DESCENDING:
		break;
	case FLYTRAP_STATE_IDLE:
		vy = 0;
		reload_start = GetTickCount64();
		break;
	}

}
