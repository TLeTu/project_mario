#include "Piranha.h"
#include "debug.h"

#include "Collision.h"

#include <cmath>

#include "Mario.h"


CPiranha::CPiranha(float x, float y, int type) :CGameObject(x, y)
{
	this->marioIsNear = false;
	this->marioIsAbove = false;
	this->fireball = NULL;
	this->isReloading = true;
	this->shootingY = y;
	this->hidingY = y + 32;
	this->piranhaType = type;
	reload_start = -1;
	this->type = "piranha";
	SetState(PIRANHA_STATE_DESCENDING);
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
	
}

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;

    CGame::GetInstance()->GetCurrentScene()->GetPlayerPosition(mx, my);


    marioIsNear = abs(x - mx) <= 80;

	marioIsAbove = y > my && abs(x-mx) <= 16;

	if (GetState() == PIRANHA_STATE_HIDING && marioIsNear && !marioIsAbove)
	{
		if (piranhaType)
		{
			if (GetTickCount64() - reload_start > PIRANHA_GREEN_MOVING_TIME)
			{
				SetState(PIRANHA_STATE_ASCENDING);
			}
		}
		else
		{
			if (GetTickCount64() - reload_start > PIRANHA_MOVING_TIME)
			{
				SetState(PIRANHA_STATE_ASCENDING);
			}
		}

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
		if (piranhaType)
		{
			if (GetTickCount64() - reload_start > PIRANHA_GREEN_RELOAD_TIME)
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
		else
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
		if (piranhaType)
		{
			if (GetTickCount64() - reload_start > PIRANHA_GREEN_MOVING_TIME)
			{
				SetState(PIRANHA_STATE_DESCENDING);
			}
		}
		else
		{
			if (GetTickCount64() - reload_start > PIRANHA_MOVING_TIME)
			{
				SetState(PIRANHA_STATE_DESCENDING);
			}
		}
	}

    // Update other properties and process collisions
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}



void CPiranha::Render()
{
	int aniId = -1;
	if (piranhaType)
	{
		if (x > mx)
		{
			aniId = ID_ANI_PIRANHA_GREEN_SHOOTING_LEFT;
		}
		else
		{
			aniId = ID_ANI_PIRANHA_GREEN_SHOOTING_RIGHT;
		}
	}
	else
	{
		if (x > mx)
		{
			aniId = ID_ANI_PIRANHA_SHOOTING_LEFT;
		}
		else
		{
			aniId = ID_ANI_PIRANHA_SHOOTING_RIGHT;
		}
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
		reload_start = GetTickCount64();
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
