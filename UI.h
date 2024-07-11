#pragma once

#include "GameObject.h"
#include "UI_BG.h"
#include "UI_Number.h"
#include "UI_Time.h"
#include "UI_Score.h"
#include "UI_Money.h"
#include "UI_Life.h"
#include "UI_CardSlot.h"

#define UI_ID_SPRITE 1000502


class CUI : public CGameObject
{
protected:
	int timeNum;
	int scoreNum;
	int lifeNum;
	int moneyNum;

	CUIBG* background;
	CUITime* time;
	CUIScore* score;
	CUIMoney* money;
	CUILife* life;
	CUICardSlot* cardSlot1;
	CUICardSlot* cardSlot2;
	CUICardSlot* cardSlot3;

	ULONGLONG timeStart;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);



	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void AddScore(int scoreNumber);
	virtual void AddLife(int lifeNumber);
	virtual void DecreaseLife(int lifeNumber);
	virtual void AddMoney(int moneyNumber);
	virtual void AddCard(int cardType);
	virtual void SetCardSlot(int cardSlot, int cardType);
	virtual void Save(int& score, int& life, int& money);
	void SetUIPosition(float x, float y);
	CUI(float x, float y);
};