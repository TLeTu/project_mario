#pragma once

#include "GameObject.h"

#define PIRANHA_GRAVITY 0.002f
#define PIRANHA_WALKING_SPEED 0.05f


#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_HEIGHT 23

#define ID_ANI_PIRANHA_SHOOTING 210000
//#define ID_ANI_PIRANHA_DIE 5001

class CPiranha : public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPiranha(float x, float y);
	virtual void SetState(int state);
};