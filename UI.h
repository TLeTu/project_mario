#pragma once

#include "GameObject.h"
#include "UI_BG.h"
#include "UI_Number.h"
#include "UI_Time.h"
#include "UI_Score.h"
#include "UI_Money.h"
#include "UI_Life.h"

#define UI_ID_SPRITE 1000502


class CUI : public CGameObject
{
protected:
	CUIBG* background;
	CUITime* time;
	CUIScore* score;
	CUIMoney* money;
	CUILife* life;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);


	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Render();
	void SetUIPosition(float x, float y);
	CUI(float x, float y);
};