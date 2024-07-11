#include "UI_BG.h"
#include "debug.h"
#include "Sprites.h"

CUIBG::CUIBG(float x, float y) : CGameObject(x, y)
{
}

void CUIBG::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUIBG::Render()
{
	CSprites::GetInstance()->Get(UIBG_ID_SPRITE)->Draw(x, y);
	//RenderBoundingBox();
}