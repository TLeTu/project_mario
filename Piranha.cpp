#include "Piranha.h"
#include "debug.h"

#include "Collision.h"

#include <cmath>


CPiranha::CPiranha(float x, float y) :CGameObject(x, y)
{
	this->marioIsNear = false;
	this->fireball = NULL;
	this->isReloading = true;
	this->shootingY = y;
	this->hidingY = y + 50;
	reload_start = -1;
	type = "piranha";
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
	y += vy * dt;

    CGame::GetInstance()->GetCurrentScene()->GetPlayerPosition(mx, my);

    marioIsNear = abs(x - mx) <= 80;

	if (GetState() == PIRANHA_STATE_HIDING && marioIsNear)
	{
		SetState(PIRANHA_STATE_ASCENDING);
	}
	if (GetState() == PIRANHA_STATE_DESCENDING)
	{
		if (y < hidingY)
		{
			vy = 0.03f;
		}
		else
		{
			SetState(PIRANHA_STATE_HIDING);
		}
	}
	if (GetState() == PIRANHA_STATE_ASCENDING)
	{
		if (y > shootingY)
		{
			vy = -0.03f;
		}
		else
		{
			SetState(PIRANHA_STATE_SHOOTING);
		}
	}
	if (GetState() == PIRANHA_STATE_SHOOTING)
	{
		if (GetTickCount64() - reload_start > PIRANHA_RELOAD_TIME)
		{
				if (x > mx)
				{
					SetState(PIRANHA_STATE_SHOOTING_LEFT);
				}
				else
				{
					SetState(PIRANHA_STATE_SHOOTING_RIGHT);
				}
		}
	}
	if (GetState() == PIRANHA_STATE_SHOOTING_RIGHT || GetState() == PIRANHA_STATE_SHOOTING_LEFT)
	{
		fireball = new CFireball(x, y);
		CGame::GetInstance()->GetCurrentScene()->AddGameObject(fireball);
		if (GetState() == PIRANHA_STATE_SHOOTING_LEFT)
		{
			if (my > y && my - y >= 16)
			{
				fireball->SetState(FIREBALL_STATE_FLYING_DOWNLEFT);
			}
			else if (my < y && y - my >= 16)
			{
				fireball->SetState(FIREBALL_STATE_FLYING_UPLEFT);
			}
			else
			{
				fireball->SetState(FIREBALL_STATE_FLYING_LEFT);
			}
		}
		else
		{
			if (my > y && my - y >= 16)
			{
				fireball->SetState(FIREBALL_STATE_FLYING_DOWNRIGHT);
			}
			else if (my < y && y - my >= 16)
			{
				fireball->SetState(FIREBALL_STATE_FLYING_UPRIGHT);
			}
			else
			{
				fireball->SetState(FIREBALL_STATE_FLYING_RIGHT);
			}
		}
		SetState(PIRANHA_STATE_IDLE);
	}
	if (GetState() == PIRANHA_STATE_IDLE)
	{
		if (GetTickCount64() - reload_start > PIRANHA_RELOAD_TIME)
		{
			SetState(PIRANHA_STATE_DESCENDING);
		}
	}

    // Update other properties and process collisions
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}



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
	case PIRANHA_STATE_SHOOTING:
		vy = 0;
		reload_start = GetTickCount64();
		break;
	case PIRANHA_STATE_IDLE:
		reload_start = GetTickCount64();
		break;
	}

}
