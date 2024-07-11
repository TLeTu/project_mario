#include "UI_Number.h"
#include "debug.h"
#include "Sprites.h"

void CUINum::Render()
{
	CSprites::GetInstance()->Get(UI_NUM_ID_SPRITE)->Draw(x, y);
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

}

