#include "UI.h"
#include "debug.h"

CUI::CUI(float x, float y) : CGameObject(x, y)
{
	if (CGame::GetInstance()->GetCurrentSceneId() != 1)
	{
		timeStart = GetTickCount64();
		timeNum = 200;
	}
	else 
	{
		timeStart = -1;
		timeNum = 0;
	}

	background = new CUIBG(x, y);
	time = new CUITime(x, y);
	time->SetTime(timeNum);
	score = new CUIScore(x, y);
	money = new CUIMoney(x, y);
	life = new CUILife(x, y);
	cardSlot1 = new CUICardSlot(x, y);
	cardSlot2 = new CUICardSlot(x, y);
	cardSlot3 = new CUICardSlot(x, y);
}

void CUI::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUI::Render()
{
	background->Render();
	CAnimations::GetInstance()->Get(UI_ID_SPRITE)->Render(x, y);
	time->Render();
	score->Render();
	money->Render();
	life->Render();
	cardSlot1->Render();
	cardSlot2->Render();
	cardSlot3->Render();
}

void CUI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - timeStart > 500 && timeNum >0)
	{
		timeNum--;
		time->SetTime(timeNum);
		timeStart = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
}

void CUI::SetUIPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	background->SetPosition(x + 50, y);
	time->SetUIPosition(x + 44, y + 4);
	score->SetUIPosition(x - 28, y + 4);
	money->SetUIPosition(x + 52, y - 4);
	life->SetUIPosition(x - 50, y + 4);
	cardSlot1->SetPosition(x + 98, y);
	cardSlot2->SetPosition(x + 98 + 23, y);
	cardSlot3->SetPosition(x + 98 + 23 + 23, y);
}