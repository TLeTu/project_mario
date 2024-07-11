#pragma once

#include "GameObject.h"
#include "UI_Number.h"



class CUIScore: public CGameObject
{
protected:
	CUINum* num1;
	CUINum* num2;
	CUINum* num3;
	CUINum* num4;
	CUINum* num5;
	CUINum* num6;
	CUINum* num7;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);



	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetUIPosition(float x, float y);
	virtual void Render();
	virtual void SetScore(int score);
	CUIScore(float x, float y);
};