#pragma once

#include "GameObject.h"

#define UI_BBOX_WIDTH 152
#define UI_BBOX_HEIGHT 28

#define UI_ID_SPRITE 1000502


class CUI : public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);


	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Render();
	CUI(float x, float y);
};