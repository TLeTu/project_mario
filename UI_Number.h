#pragma once

#include "GameObject.h"


#define UI_NUM_ID_SPRITE 1000300


class CUINum : public CGameObject
{
protected:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};


	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Render();
	CUINum(float x, float y);
};