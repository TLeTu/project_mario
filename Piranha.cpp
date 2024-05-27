#include "Piranha.h"
#include "debug.h"

#include "Collision.h"


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
	float mx, my;
	CGame::GetInstance()->GetCurrentScene()->GetPlayerPosition(mx, my);

	if (x - mx <= 48) 
	{
		marioIsNear = true;
	}
	else
	{
		marioIsNear = false;
	}

	if (marioIsNear && fireball == NULL && !isReloading)
	{
		fireball = new CFireball(x, y);
		CGame::GetInstance()->GetCurrentScene()->AddGameObject(fireball);
		isReloading = true;
		reload_start = GetTickCount64();
	}

	if (marioIsNear && !isReloading)
	{
		fireball->SetPosition(x, y);
		isReloading = true;
		reload_start = GetTickCount64();
	}

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
	CAnimations::GetInstance()->Get(ID_ANI_PIRANHA_SHOOTING)->Render(x, y);
	RenderBoundingBox();
}

void CPiranha::SetState(int state)
{
	//CGameObject::SetState(state);
	//switch (state)
	//{
	//case PIRANHA_STATE_DIE:
	//	die_start = GetTickCount64();
	//	y += (PIRANHA_BBOX_HEIGHT - PIRANHA_BBOX_HEIGHT_DIE) / 2;
	//	vx = 0;
	//	vy = 0;
	//	ay = 0;
	//	break;
	//case PIRANHA_STATE_WALKING:
	//	vx = -PIRANHA_WALKING_SPEED;
	//	break;
	//}
}
