#include "Coin.h"

CCoin::CCoin(float x, float y) : CGameObject(x, y)
{
	initY = y;
	SetState(COIN_STATE_NORMAL);
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_NORMAL:
		vy = 0;
		break;
	case COIN_STATE_FAKE_UP:
		vy = -0.3f;
		break;
	case COIN_STATE_FAKE_DOWN:
		vy = 0.3f;
		break;
	}
}

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (GetState() == COIN_STATE_FAKE_UP && y < initY - 48)
	{
		SetState(COIN_STATE_FAKE_DOWN);
	}
	if (GetState() == COIN_STATE_FAKE_DOWN && y > initY)
	{
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}