#include "UI.h"
#include "debug.h"

CUI::CUI(float x, float y) : CGameObject(x, y)
{
}

void CUI::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + UI_BBOX_WIDTH;
	bottom = y + UI_BBOX_HEIGHT;
}

void CUI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CGame::GetInstance()->GetCurrentSceneId() == 1)
	{
		return;
	}
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	SetPosition(cx + 108, cy + 220);
}

void CUI::Render()
{
	CAnimations::GetInstance()->Get(UI_ID_SPRITE)->Render(x, y);
	//RenderBoundingBox();
}