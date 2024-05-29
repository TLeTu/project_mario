#pragma once
#include "GameObject.h"

#define FIREBALL_FLYING_SPEED 0.05f


#define FIREBALL_BBOX_WIDTH 7
#define FIREBALL_BBOX_HEIGHT 11

#define ID_ANI_FIREBALL_FLYING 410000

#define FIREBALL_STATE_FLYING_UPLEFT 100
#define FIREBALL_STATE_FLYING_UPRIGHT 200
#define FIREBALL_STATE_FLYING_DOWNLEFT 300
#define FIREBALL_STATE_FLYING_DOWNRIGHT 400
#define FIREBALL_STATE_FLYING_LEFT 500
#define FIREBALL_STATE_FLYING_RIGHT 600
#define FIREBALL_STATE_IDLE 700

#define FIREBALL_DIE_TIMEOUT 5000

class CFireball : public CGameObject
{
protected:
	float ax;
	float ay;
	float magnitude;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	//virtual void OnNoCollision(DWORD dt);

	//virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CFireball(float x, float y); 
	virtual void SetState(int state);
};