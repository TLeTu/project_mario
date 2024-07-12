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
	scoreNum = 0;
	lifeNum = 3;
	moneyNum = 0;

	background = new CUIBG(x, y);
	time = new CUITime(x, y);
	time->SetTime(timeNum);
	score = new CUIScore(x, y);
	score->SetScore(scoreNum);
	money = new CUIMoney(x, y);
	money->SetMoney(moneyNum);
	life = new CUILife(x, y);
	life->SetLife(lifeNum);
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
	if (GetTickCount64() - timeStart > 1000 && timeNum >0)
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

void CUI::AddScore(int scoreNumber)
{
	//check if score + scoreNumber > 9999999 ,if true, set score = 9999999
	if (scoreNum + scoreNumber > 9999999)
	{
		scoreNum = 9999999;
	}
	else
		scoreNum += scoreNumber;
	score->SetScore(scoreNum);
}

void CUI::SetScore(int scoreNumber)
{
	scoreNum = scoreNumber;
	score->SetScore(scoreNum);
}

void CUI::AddLife(int lifeNumber)
{
	//check if life + lifeNumber > 99 ,if true, set life = 99
	if (lifeNum + lifeNumber > 99)
	{
		lifeNum = 99;
	}
	else
	{
		lifeNum += lifeNumber;
	}
	life->SetLife(lifeNum);
}

void CUI::DecreaseLife(int lifeNumber)
{
	//check if life - lifeNumber < 0 ,if true, set life = 0
	if (lifeNum - lifeNumber < 0)
	{
		lifeNum = 0;
	}
	else
	{
		lifeNum -= lifeNumber;
	}
	life->SetLife(lifeNum);
}

void CUI::SetLife(int lifeNumber)
{
	lifeNum = lifeNumber;
	life->SetLife(lifeNum);
}

void CUI::AddMoney(int moneyNumber)
{
	//check if money + moneyNumber > 99 ,if true, set money = 99
	if (moneyNum + moneyNumber > 99)
	{
		moneyNum = 99;
	}
	else
		moneyNum += moneyNumber;
	money->SetMoney(moneyNum);
}

void CUI::SetMoney(int moneyNumber)
{
	moneyNum = moneyNumber;
	money->SetMoney(moneyNum);
}

void CUI::AddCard(int cardType)
{
	if (cardSlot1->GetEmpty())
	{
		cardSlot1->SetState(cardType);
		cardSlot1->SetEmpty(false);
	}
	else if (cardSlot2->GetEmpty())
	{
		cardSlot2->SetState(cardType);
		cardSlot2->SetEmpty(false);
	}
	else if (cardSlot3->GetEmpty())
	{
		cardSlot3->SetState(cardType);
		cardSlot3->SetEmpty(false);
	}
	else return;
}

void CUI::SetCardSlot(int cardSlot, int cardType)
{
	switch (cardSlot)
	{
	case 1:
		cardSlot1->SetState(cardType);
		cardSlot1->SetEmpty(false);
		break;
	case 2:
		cardSlot2->SetState(cardType);
		cardSlot2->SetEmpty(false);
		break;
	case 3:
		cardSlot3->SetState(cardType);
		cardSlot3->SetEmpty(false);
		break;
	}
}

int CUI::GetCardSlot(int slot)
{
	switch (slot)
	{
	case 1:
		return cardSlot1->GetState();
	case 2:
		return cardSlot2->GetState();
	case 3:
		return cardSlot3->GetState();
	}
	return 100;
}

void CUI::Save(int& score, int& life, int& money)
{
	score = scoreNum;
	life = lifeNum;
	money = moneyNum;
}