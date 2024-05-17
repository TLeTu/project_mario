#pragma once
#include "GameObject.h"
#include "EdgeDetector.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SPINNING_SPEED 0.2f


#define KOOPAS_BBOX_WIDTH 15
#define KOOPAS_BBOX_HEIGHT 24
#define KOOPAS_BBOX_HEIGHT_SHELL 15
#define KOOPAS_RESTORE_TIMEOUT 5000

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200
#define KOOPAS_STATE_SPIN 300

#define ID_ANI_KOOPAS_WALKING 7000
#define ID_ANI_KOOPAS_FLIP 7002
#define ID_ANI_KOOPAS_SHELL 7001

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;
	float vx_temp;
	int detecting;
	int spinDirection;

	ULONGLONG restore_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

public:
	CEdgeDetector* edgeDetector;

	CKoopas(float x, float y);
	virtual void SetState(int state);
	virtual void SetSpinDirection(int direction) { spinDirection = direction; }	
};