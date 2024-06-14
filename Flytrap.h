#pragma once
#include "GameObject.h"
#include "Fireball.h"

#define FLYTRAP_MOVING_TIME 700

#define FLYTRAP_BBOX_WIDTH 16
#define FLYTRAP_BBOX_HEIGHT 16

#define ID_ANI_FLYTRAP 210003

#define FLYTRAP_STATE_HIDING 300
#define FLYTRAP_STATE_ASCENDING 400
#define FLYTRAP_STATE_DESCENDING 500
#define FLYTRAP_STATE_SHOOTING 600
#define FLYTRAP_STATE_IDLE 700

class CFlytrap : public CGameObject
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
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CFlytrap(float x, float y);
	virtual void SetState(int state);
};