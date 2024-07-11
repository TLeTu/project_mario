#include "UI_Number.h"
#include "debug.h"
#include "Sprites.h"

void CUINum::Render()
{
	int aniId = NUM_ID_SPRITE_0;
	int state = GetState();
	if (state == NUM_STATE_0)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_0)->Draw(x, y);
	else if (state == NUM_STATE_1)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_1)->Draw(x, y);
	else if (state == NUM_STATE_2)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_2)->Draw(x, y);
	else if (state == NUM_STATE_3)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_3)->Draw(x, y);
	else if (state == NUM_STATE_4)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_4)->Draw(x, y);
	else if (state == NUM_STATE_5)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_5)->Draw(x, y);
	else if (state == NUM_STATE_6)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_6)->Draw(x, y);
	else if (state == NUM_STATE_7)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_7)->Draw(x, y);
	else if (state == NUM_STATE_8)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_8)->Draw(x, y);
	else if (state == NUM_STATE_9)
		CSprites::GetInstance()->Get(NUM_ID_SPRITE_9)->Draw(x, y);
}

void CUINum::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

CUINum::CUINum(float x, float y)
{
	SetState(NUM_STATE_0);
}

