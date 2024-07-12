#include "UI_ClearText.h"
#include "debug.h"
#include "Sprites.h"

CUIClearText::CUIClearText(float x, float y) : CGameObject(x, y)
{
	cardSlot = new CUICardSlot(x, y);
}

void CUIClearText::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUIClearText::Render()
{
	CSprites::GetInstance()->Get(UI_TEXT_ID_SPRITE)->Draw(x, y);
	cardSlot->Render();
	//RenderBoundingBox();
}