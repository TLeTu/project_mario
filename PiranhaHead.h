#pragma once

#include "GameObject.h"


#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_HEIGHT 23

class CPiranha : public CGameObject
{
protected:
	bool marioIsNear;
	bool marioIsAbove;
	bool isReloading;
	float mx, my;
	float shootingY, hidingY;

	ULONGLONG reload_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	//virtual void OnNoCollision(DWORD dt);

	//virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPiranha(float x, float y);
	CFireball* fireball;
	virtual void SetState(int state);
}; #pragma once
