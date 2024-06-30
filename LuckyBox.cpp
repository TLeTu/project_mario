#include "LuckyBox.h"
#include "Coin.h"

CLuckyBox::CLuckyBox(float x, float y) : CGameObject(x, y)
{
	isRolling = true;
	rolling_time = -1;
	SetState(LUCKYBOX_STATE_STAR);
}

void CLuckyBox::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LUCKYBOX_STATE_STAR:
		rolling_time = GetTickCount64();
		break;
	case LUCKYBOX_STATE_MUSHROOM:
		rolling_time = GetTickCount64();
		break;
	case LUCKYBOX_STATE_FLOWER:
	    rolling_time = GetTickCount64();
		break;
	}
}

void CLuckyBox::Render()
{
	int aniID = -1;
	switch (GetState())
	{
	case LUCKYBOX_STATE_STAR:
		aniID = ID_ANI_LUCKYBOX_STAR;
		break;
	case LUCKYBOX_STATE_MUSHROOM:
		aniID = ID_ANI_LUCKYBOX_MUSHROOM;
		break;
	case LUCKYBOX_STATE_FLOWER:
		aniID = ID_ANI_LUCKYBOX_FLOWER;
		break;
	case LUCKYBOX_STATE_EMPTY:
		aniID = ID_ANI_LUCKYBOX_EMPTY;
		break;
	}

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniID)->Render(x, y);

	//RenderBoundingBox();
}

void CLuckyBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LUCKYBOX_BBOX_WIDTH / 2;
	t = y - LUCKYBOX_BBOX_HEIGHT / 2;
	r = l + LUCKYBOX_BBOX_WIDTH;
	b = t + LUCKYBOX_BBOX_HEIGHT;
}

void CLuckyBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isRolling && GetTickCount64() - rolling_time > 250)
	{
		if (GetState() == LUCKYBOX_STATE_STAR)
			SetState(LUCKYBOX_STATE_MUSHROOM);
		else if (GetState() == LUCKYBOX_STATE_MUSHROOM)
			SetState(LUCKYBOX_STATE_FLOWER);
		else if (GetState() == LUCKYBOX_STATE_FLOWER)
			SetState(LUCKYBOX_STATE_STAR);
	}
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLuckyBox::SpawnCoin() {
	LPGAMEOBJECT coin = new CCoin(x, y - 21);
	coin->SetPosition(x, y - 21);
	if (GetState() == LUCKYBOX_STATE_STAR)
		coin->SetState(COIN_STATE_FAKE_STAR);
	else if (GetState() == LUCKYBOX_STATE_MUSHROOM)
		coin->SetState(COIN_STATE_FAKE_MUSHROOM);
	else if (GetState() == LUCKYBOX_STATE_FLOWER)
		coin->SetState(COIN_STATE_FAKE_FLOWER);
	CGame::GetInstance()->GetCurrentScene()->AddGameObject(coin);
}