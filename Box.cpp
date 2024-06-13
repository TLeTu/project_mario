#include "Box.h"
#include "GameObject.h"
#include "Collision.h"

void CBox::Render()
{
	int aniId = ID_ANI_BOX;
	if (state == BOX_STATE_EMPTY || state == BOX_STATE_ANI)
		aniId = ID_ANI_BOX_EMPTY;
	else
		aniId = ID_ANI_BOX;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BOX_BBOX_WIDTH / 2;
	t = y - BOX_BBOX_HEIGHT / 2;
	r = l + BOX_BBOX_WIDTH;
	b = t + BOX_BBOX_HEIGHT;
}

void CBox::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOX_STATE_NORMAL:
		break;
	case BOX_STATE_EMPTY:
		break;
	case BOX_STATE_ANI:
		down_start = GetTickCount64();
		break;
	}
}

void CBox::SpawnCoin() {
	LPGAMEOBJECT coin = new CCoin(x, y - 17);
	coin->SetPosition(x, y - 17);
	coin->SetState(COIN_STATE_FAKE_UP);
	CGame::GetInstance()->GetCurrentScene()->AddGameObject(coin);
}

void CBox::SpawnMushroom() {
	LPGAMEOBJECT mushroom = new CMushroom(x, y - 17);
	mushroom->SetPosition(x, y - 17);
	CGame::GetInstance()->GetCurrentScene()->AddGameObject(mushroom);
}

void CBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y != initY)
	{
		if (GetTickCount64() - down_start > 50)
		{
			SetPosition(x, initY);
			SetState(BOX_STATE_EMPTY);
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}