#include "Brick.h"

void CBrick::Render()
{
	int ani_id = -1;
	if (GetState() == BRICK_STATE_BREAK)
	{
		ani_id = ID_ANI_BREAK;
	}
	else ani_id = ID_ANI_BRICK;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ani_id)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state == BRICK_STATE_BREAK) && (GetTickCount64() - break_start > 99))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_BREAK:
		break_start = GetTickCount64();
		return;
	case BRICK_STATE_NORMAL:
		return;
	}
}

int CBrick::IsCollidable()
{
	if (this->GetState() != BRICK_STATE_BREAK)
	{
		return 1;
	}
	else return 0;
}
