#include "Fireball.h"

CFireball::CFireball(float x, float y):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	die_start = GetTickCount64();
	SetState(FIREBALL_STATE_IDLE);
}

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIREBALL_BBOX_WIDTH / 2;
	top = y - FIREBALL_BBOX_HEIGHT / 2;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

//void CFireball::OnNoCollision(DWORD dt)
//{
//	x += vx * dt;
//	y += vy * dt;
//};
//
//void CFireball::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	if (e->obj->IsBlocking()) return;
//}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	x += vx * dt;
	y += vy * dt;

	if (GetTickCount64() - die_start > FIREBALL_DIE_TIMEOUT)
	{
		isDeleted = true;
		return;
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CFireball::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FIREBALL_FLYING)->Render(x, y);
	//RenderBoundingBox();
}

void CFireball::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREBALL_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case FIREBALL_STATE_FLYING_LEFT:
		vx = -FIREBALL_FLYING_SPEED;
		vy = 0;
		break;
	case FIREBALL_STATE_FLYING_RIGHT:
		vx = FIREBALL_FLYING_SPEED;
		vy = 0;
		break;
	case FIREBALL_STATE_FLYING_UPLEFT:
		vx = -FIREBALL_FLYING_SPEED;
		vy = -FIREBALL_FLYING_SPEED;
		magnitude = sqrt(vx * vx + vy * vy);
		vx = vx / magnitude * FIREBALL_FLYING_SPEED;
		vy = vy / magnitude * FIREBALL_FLYING_SPEED;
		break;
	case FIREBALL_STATE_FLYING_UPRIGHT:
		vx = FIREBALL_FLYING_SPEED;
		vy = -FIREBALL_FLYING_SPEED;
		magnitude = sqrt(vx * vx + vy * vy);
		vx = vx / magnitude * FIREBALL_FLYING_SPEED;
		vy = vy / magnitude * FIREBALL_FLYING_SPEED;
		break;
	case FIREBALL_STATE_FLYING_DOWNLEFT:
		vx = -FIREBALL_FLYING_SPEED;
		vy = FIREBALL_FLYING_SPEED;
		magnitude = sqrt(vx * vx + vy * vy);
		vx = vx / magnitude * FIREBALL_FLYING_SPEED;
		vy = vy / magnitude * FIREBALL_FLYING_SPEED;
		break;
	case FIREBALL_STATE_FLYING_DOWNRIGHT:
		vx = FIREBALL_FLYING_SPEED;
		vy = FIREBALL_FLYING_SPEED;
		magnitude = sqrt(vx * vx + vy * vy);
		vx = vx / magnitude * FIREBALL_FLYING_SPEED;
		vy = vy / magnitude * FIREBALL_FLYING_SPEED;
		break;
	}
}
