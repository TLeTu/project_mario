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
}

void CPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x - PIRANHA_BBOX_WIDTH / 2;
	top = y - PIRANHA_BBOX_HEIGHT / 2;
	right = left + PIRANHA_BBOX_WIDTH;
	bottom = top + PIRANHA_BBOX_HEIGHT;
}

void CPiranha::OnNoCollision(DWORD dt)
{
	//x += vx * dt;
	//y += vy * dt;
};

void CPiranha::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return;
	//if (dynamic_cast<CPiranha*>(e->obj)) return;

	//if (e->ny != 0)
	//{
	//	vy = 0;
	//}
	//else if (e->nx != 0)
	//{
	//	vx = -vx;
	//}
}

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame::GetInstance()->GetCurrentScene()->GetPlayerPosition(mx, my);

	if (abs(x - mx) <= 80) 
	{
		marioIsNear = true;
	}
	else
	{
		marioIsNear = false;
	}

	if (marioIsNear) {
		if (mx < x)
		{
			SetState(PIRANHA_STATE_SHOOTING_LEFT);
		}
		else
		{
			SetState(PIRANHA_STATE_SHOOTING_RIGHT);
		}
	}

	if (marioIsNear && fireball == NULL && !isReloading)
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
		else if (GetState() == PIRANHA_STATE_SHOOTING_RIGHT)
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
		isReloading = true;
		reload_start = GetTickCount64();
		fireball = NULL;
	}

	//if (marioIsNear && !isReloading)
	//{
	//	fireball->SetPosition(x, y);
	//	if (GetState() == PIRANHA_STATE_SHOOTING_LEFT)
	//	{
	//		if (my > y && my - y >= 16)
	//		{
	//			fireball->SetState(FIREBALL_STATE_FLYING_DOWNLEFT);
	//		}
	//		else if (my < y && y - my >= 16)
	//		{
	//			fireball->SetState(FIREBALL_STATE_FLYING_UPLEFT);
	//		}
	//		else
	//		{
	//			fireball->SetState(FIREBALL_STATE_FLYING_LEFT);
	//		}
	//	}
	//	else if (GetState() == PIRANHA_STATE_SHOOTING_RIGHT)
	//	{
	//		if (my > y && my - y >= 16)
	//		{
	//			fireball->SetState(FIREBALL_STATE_FLYING_DOWNRIGHT);
	//		}
	//		else if (my < y && y - my >= 16)
	//		{
	//			fireball->SetState(FIREBALL_STATE_FLYING_UPRIGHT);
	//		}
	//		else
	//		{
	//			fireball->SetState(FIREBALL_STATE_FLYING_RIGHT);
	//		}
	//	}
	//	isReloading = true;
	//	reload_start = GetTickCount64();
	//}

	if (isReloading)
	{
		if (GetTickCount64() - reload_start > PIRANHA_RELOAD_TIME)
		{
			isReloading = false;
			reload_start = 0;
		}
	}

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
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPiranha::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PIRANHA_STATE_IDLE:
		break;
	case PIRANHA_STATE_SHOOTING_LEFT:
		break;
	case PIRANHA_STATE_SHOOTING_RIGHT:
		break;
	}

}
