#pragma once

#include "GameObject.h"
#include "UI_CardSlot.h"


#define UI_TEXT_ID_SPRITE 1000700


class CUIClearText : public CGameObject
{
protected:
	CUICardSlot* cardSlot;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};



	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Render();
	void SetCard(int type) { 
		cardSlot->SetState(type); 
		cardSlot->SetEmpty(false);
	};
	void SetCardSlotPosition(float x, float y) { cardSlot->SetPosition(x, y); };
	CUIClearText(float x, float y);
};