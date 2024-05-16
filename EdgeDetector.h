#pragma once
#include "GameObject.h"

#define EDGEDETECTOR_GRAVITY 0.002f
#define EDGEDETECTOR_WALKING_SPEED 0.05f


#define EDGEDETECTOR_BBOX_WIDTH 16
#define EDGEDETECTOR_BBOX_HEIGHT 24

class CEdgeDetector : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG restore_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CEdgeDetector(float x, float y);
	CEdgeDetector() : CGameObject() { this->ax = 0; this->ay = EDGEDETECTOR_GRAVITY; this->vx = -EDGEDETECTOR_WALKING_SPEED; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};