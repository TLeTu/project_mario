#include "Leaf.h"

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	if (GetState() == LEAF_STATE_LEFT && GetTickCount64() - state_timer > 500)
	{
		SetState(LEAF_STATE_RIGHT);
	}
	else if (GetState() == LEAF_STATE_RIGHT && GetTickCount64() - state_timer > 500)
	{
		SetState(LEAF_STATE_LEFT);
	}
	else if (GetState() == LEAF_STATE_UP && GetTickCount64() - state_timer > 200)
	{
		SetState(LEAF_STATE_LEFT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_LEAF)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_LEFT:
		vx = -LEAF_GRAVITY;
		vy = LEAF_GRAVITY;
		state_timer = GetTickCount64();
		break;
	case LEAF_STATE_RIGHT:
		vx = LEAF_GRAVITY * 1.3f;
		vy = LEAF_GRAVITY;
		state_timer = GetTickCount64();
		break;
	case LEAF_STATE_UP:
		vx = 0;
		vy = -0.1f;
		state_timer = GetTickCount64();
		break;
	}
}