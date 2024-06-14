#include "Brick.h"

CBrick::CBrick(float x, float y, int type, int isCoin) : CGameObject(x, y)
{
	break_start = -1; 
	brickType = type;
	initY = y;
	this->isCoin = isCoin;
	down_start = -1;
	if (brickType == 1)
	{
		SetState(BRICK_STATE_WOOD);
	}
	else if (brickType == 2)
	{
		SetState(BRICK_STATE_BUTTON);
	}
	else SetState(BRICK_STATE_NORMAL);
}

void CBrick::Render()
{
	int ani_id = -1;
	if (GetState() == BRICK_STATE_BREAK)
	{
		ani_id = ID_ANI_BREAK;
	}
	else if (GetState() == BRICK_STATE_WOOD)
	{
		ani_id = ID_ANI_WOOD;
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
	if (y != initY)
	{
		if (GetTickCount64() - down_start > 50)
		{
			SetPosition(x, initY);
			if (brickType == 2)
			{
				SetState(BRICK_STATE_EMPTY);
			}
			else SetState(BRICK_STATE_NORMAL);
		}
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
	case BRICK_STATE_WOOD:
		return;
	case BRICK_STATE_BUTTON:
		return;
	case BRICK_STATE_ANI:
		down_start = GetTickCount64();
		return;
	case BRICK_STATE_EMPTY:
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
