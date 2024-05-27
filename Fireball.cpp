#include "Fireball.h"

CFireball::CFireball(float x, float y) :CGameObject(x, y)
{
	vx = -FIREBALL_FLYING_SPEED;
	vy = FIREBALL_FLYING_SPEED;
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
//	if (!e->obj->IsBlocking()) return;
//	if (e->obj->IsBlocking()) return;
//}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//vy += ay * dt;
	//vx += ax * dt;
	x += vx * dt;
	y += vy * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CFireball::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FIREBALL_FLYING)->Render(x, y);
	RenderBoundingBox();
}
