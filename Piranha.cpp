#include "Piranha.h"
#include "debug.h"

#include "Collision.h"

#include <cmath>


CPiranha::CPiranha(float x, float y) :CGameObject(x, y)
{
	vx = 0;
	vy = 0;
	this->marioIsNear = false;
	this->isReloading = false;
	this->fireball = NULL;
	this->shootingY = y;
	this->hidingY = y + 16;
	SetState(PIRANHA_STATE_DESCENDING);
}

void CPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x - PIRANHA_BBOX_WIDTH / 2;
	top = y - PIRANHA_BBOX_HEIGHT / 2;
	right = left + PIRANHA_BBOX_WIDTH;
	bottom = top + PIRANHA_BBOX_HEIGHT;
}

//void CPiranha::OnNoCollision(DWORD dt)
//{
//	//x += vx * dt;
//	//y += vy * dt;
//};

//void CPiranha::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	//if (!e->obj->IsBlocking()) return;
//	//if (dynamic_cast<CPiranha*>(e->obj)) return;
//
//	//if (e->ny != 0)
//	//{
//	//	vy = 0;
//	//}
//	//else if (e->nx != 0)
//	//{
//	//	vx = -vx;
//	//}
//}

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	CGame::GetInstance()->GetCurrentScene()->GetPlayerPosition(mx, my);

	marioIsNear = abs(x - mx) <= 80;

	if (marioIsNear && GetState() == PIRANHA_STATE_HIDING)
	{
		SetState(PIRANHA_STATE_ASCENDING);
	}

	switch (GetState())
	{
	case PIRANHA_STATE_ASCENDING:
		if (!marioIsNear)
		{
			SetState(PIRANHA_STATE_DESCENDING);
		}
		else if (y > shootingY)
		{
			vy = -0.05f;
		}
		else
		{
			vy = 0;
			SetState(mx < x ? PIRANHA_STATE_SHOOTING_LEFT : PIRANHA_STATE_SHOOTING_RIGHT);
		}
		break;

	case PIRANHA_STATE_DESCENDING:
		if (y < hidingY)
		{
			vy = 0.05f;
		}
		else
		{
			vy = 0;
			SetState(PIRANHA_STATE



void CPiranha::Render()
{
	int aniId = ID_ANI_PIRANHA_SHOOTING_LEFT;
	if (GetState() == PIRANHA_STATE_SHOOTING_LEFT)
	{
		aniId = ID_ANI_PIRANHA_SHOOTING_LEFT;
	}
	else if (GetState() == PIRANHA_STATE_SHOOTING_RIGHT)
	{
		aniId = ID_ANI_PIRANHA_SHOOTING_RIGHT;
	}
	else if (GetState() == PIRANHA_STATE_HIDING)
	{
		aniId = ID_ANI_PIRANHA_SHOOTING_LEFT;
	}
	else if (GetState() == PIRANHA_STATE_ASCENDING)
	{
		aniId = ID_ANI_PIRANHA_SHOOTING_LEFT;
	}
	else if (GetState() == PIRANHA_STATE_DESCENDING)
	{
		aniId = ID_ANI_PIRANHA_SHOOTING_LEFT;
	}
	else if (GetState() == PIRANHA_STATE_SHOOTING)
	{
		aniId = ID_ANI_PIRANHA_SHOOTING_LEFT;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPiranha::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PIRANHA_STATE_HIDING:
		vy = 0;
		break;
	case PIRANHA_STATE_SHOOTING_LEFT:
		break;
	case PIRANHA_STATE_SHOOTING_RIGHT:
		break;
	case PIRANHA_STATE_ASCENDING:
		break;
	case PIRANHA_STATE_DESCENDING:
		break;
	}

}
