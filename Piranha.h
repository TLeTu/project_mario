#pragma once

#include "GameObject.h"
#include "Fireball.h"

#define PIRANHA_GRAVITY 0.002f
#define PIRANHA_WALKING_SPEED 0.05f

#define PIRANHA_RELOAD_TIME 500
#define PIRANHA_MOVING_TIME 700
#define PIRANHA_GREEN_RELOAD_TIME 500
#define PIRANHA_GREEN_MOVING_TIME 700

#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_RED_HEIGHT 22
#define PIRANHA_BBOX_GREEN_HEIGHT 16

#define ID_ANI_PIRANHA_SHOOTING_LEFT 210000
#define ID_ANI_PIRANHA_SHOOTING_RIGHT 220000

#define ID_ANI_PIRANHA_GREEN_SHOOTING_LEFT 210001
#define ID_ANI_PIRANHA_GREEN_SHOOTING_RIGHT 210002

#define PIRANHA_STATE_SHOOTING_LEFT 100
#define PIRANHA_STATE_SHOOTING_RIGHT 200
#define PIRANHA_STATE_HIDING 300
#define PIRANHA_STATE_ASCENDING 400
#define PIRANHA_STATE_DESCENDING 500
#define PIRANHA_STATE_SHOOTING 600
#define PIRANHA_STATE_IDLE 700

class CPiranha : public CGameObject
{
protected:
	bool marioIsNear;
	bool marioIsAbove;
	bool isReloading;
	float mx, my;
	float shootingY, hidingY;
	int piranhaType;

	ULONGLONG reload_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPiranha(float x, float y, int type);
	CFireball* fireball;
	virtual void SetState(int state);
};