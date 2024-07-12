#include "Mushroom.h"

CMushroom::CMushroom(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	SetState(MUSHROOM_STATE);
}

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - MUSHROOM_BBOX_WIDTH / 2;
		top = y - MUSHROOM_BBOX_HEIGHT / 2;
		right = left + MUSHROOM_BBOX_WIDTH;
		bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (GetState() == MUSHROOM_STATE)
	//{
		if (!e->obj->IsBlocking()) return;
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	//}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMushroom::Render()
{
	if (GetState() == MUSHROOM_STATE)
		CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM)->Render(x, y);
	else if (GetState() == GREEN_MUSHROOM_STATE)
		CAnimations::GetInstance()->Get(ID_ANI_GREEN_MUSHROOM)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE:
		vx = -MUSHROOM_WALKING_SPEED;
		break;
	case GREEN_MUSHROOM_STATE:
		vx = -MUSHROOM_WALKING_SPEED;
		break;
	}
}
