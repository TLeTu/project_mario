#include "Box.h"
#include "GameObject.h"
#include "Collision.h"

void CBox::Render()
{
	int aniId = ID_ANI_BOX;
	if (state == BOX_STATE_EMPTY)
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
	}
}

void CBox::SpawnMushroom() {
	LPGAMEOBJECT mushroom = new CMushroom(x, y - 17);
	mushroom->SetPosition(x, y - 17);
	CGame::GetInstance()->GetCurrentScene()->AddGameObject(mushroom);
}