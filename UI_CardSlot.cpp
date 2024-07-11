#include "UI_CardSlot.h"
#include "debug.h"
#include "Sprites.h"

void CUICardSlot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUICardSlot::Render()
{
	int state = GetState();
	int aniID = CARD_SPRITE_EMPTY;
	if (state == CARD_STATE_MUSHROOM)
		aniID = CARD_SPRITE_MUSHROOM;
	else if (state == CARD_STATE_FLOWER)
		aniID = CARD_SPRITE_FLOWER;
	else if (state == CARD_STATE_STAR)
		aniID = CARD_SPRITE_STAR;
	CSprites::GetInstance()->Get(aniID)->Draw(x, y);
}

void CUICardSlot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void CUICardSlot::SetState(int state)
{
	this->state = state;
}

