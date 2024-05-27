#pragma once
#include "GameObject.h"

#define FIREBALL_FLYING_SPEED 0.05f


#define FIREBALL_BBOX_WIDTH 16
#define FIREBALL_BBOX_HEIGHT 14
#define FIREBALL_BBOX_HEIGHT_DIE 7

#define ID_ANI_FIREBALL_FLYING 410000

class CFireball : public CGameObject
{
protected:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	//virtual void OnNoCollision(DWORD dt);

	//virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CFireball(float x, float y); 
};