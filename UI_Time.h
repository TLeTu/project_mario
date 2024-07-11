#pragma once

#include "GameObject.h"
#include "UI_Number.h"



class CUITime : public CGameObject
{
protected:
	CUINum* num1;
	CUINum* num2;
	CUINum* num3;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);



	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetUIPosition(float x, float y);
	virtual void Render();
	virtual void SetTime(int timeNum);
	CUITime(float x, float y);
};