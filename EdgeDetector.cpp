#include "EdgeDetector.h"

CEdgeDetector::CEdgeDetector(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = EDGEDETECTOR_GRAVITY;
	vx = -EDGEDETECTOR_WALKING_SPEED;
}

void CEdgeDetector::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - EDGEDETECTOR_BBOX_WIDTH / 2;
		top = y - EDGEDETECTOR_BBOX_HEIGHT / 2;
		right = left + EDGEDETECTOR_BBOX_WIDTH;
		bottom = top + EDGEDETECTOR_BBOX_HEIGHT;
}

void CEdgeDetector::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CEdgeDetector::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CEdgeDetector*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CEdgeDetector::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CEdgeDetector::Render()
{
	//RenderBoundingBox();
}
