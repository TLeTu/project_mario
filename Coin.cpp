#include "Coin.h"
#include "Brick.h"

CCoin::CCoin(float x, float y) : CGameObject(x, y)
{
	initY = y;
	SetState(COIN_STATE_NORMAL);
	to_brick_start = -1;
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
	case COIN_STATE_TOBRICK:
		to_brick_start = GetTickCount64();
		break;
	case COIN_STATE_FAKE_STAR:
		vy = -0.2f;
		break;
	case COIN_STATE_FAKE_MUSHROOM:
		vy = -0.2f;
		break;
	case COIN_STATE_FAKE_FLOWER:
		vy = -0.2f;
		break;
	}
}

void CCoin::Render()
{
	int aniID = -1;
	if (GetState() == COIN_STATE_FAKE_MUSHROOM)
	{
		aniID = ID_ANI_COIN_MUSHROOM;
	}
	else if (GetState() == COIN_STATE_FAKE_STAR)
	{
		aniID = ID_ANI_COIN_STAR;
	}
	else if (GetState() == COIN_STATE_FAKE_FLOWER)
	{
		aniID = ID_ANI_COIN_FLOWER;
	}
	else
	{
		aniID = ID_ANI_COIN;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniID)->Render(x, y);

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
	if (GetState() == COIN_STATE_FAKE_STAR && y<=0)
	{
		isDeleted = true;
		return;
	}
	if (GetState() == COIN_STATE_FAKE_MUSHROOM && y <= 0)
	{
		isDeleted = true;
		return;
	}
	if (GetState() == COIN_STATE_FAKE_FLOWER && y <= 0)
	{
		isDeleted = true;
		return;
	}
	if (GetState() == COIN_STATE_TOBRICK && GetTickCount64() - to_brick_start > 10000)
	{
		isDeleted = true;
		CBrick* brick = new CBrick(x, y, 0, 1);
		CGame::GetInstance()->GetCurrentScene()->CoinToBrick(brick);
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}