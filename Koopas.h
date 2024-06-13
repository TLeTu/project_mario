#pragma once
#include "GameObject.h"
#include "EdgeDetector.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SPINNING_SPEED 0.2f
#define KOOPAS_JUMP_SPEED 0.17f


#define KOOPAS_BBOX_WIDTH 15
#define KOOPAS_BBOX_HEIGHT 24
#define KOOPAS_BBOX_HEIGHT_SHELL 15

#define KOOPAS_SHELL_TIMEOUT 2000
#define KOOPAS_RESTORE_TIMEOUT 3000

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200
#define KOOPAS_STATE_SPIN 300
#define KOOPAS_STATE_RESTORE 400
#define KOOPAS_STATE_JUMP 500
#define KOOPAS_STATE_TAKEOFF 600
#define KOOPAS_STATE_IDLE 700

#define ID_ANI_KOOPAS_WALKING 7000
#define ID_ANI_KOOPAS_FLIP 7002
#define ID_ANI_KOOPAS_SHELL 7001
#define ID_ANI_KOOPAS_RESTORING 7003

#define ID_ANI_KOOPAS_GREEN_WALKING 7100
#define ID_ANI_KOOPAS_GREEN_FLIP 7102
#define ID_ANI_KOOPAS_GREEN_SHELL 7101
#define ID_ANI_KOOPAS_GREEN_RESTORING 7103

#define ID_ANI_KOOPAS_JUMP 7200
#define ID_ANI_KOOPAS_JUMP_FLIP 7201

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;
	float vx_temp;
	int detecting;
	int spinDirection;
	int isCarried;
	int koopaType;
	int koopaColor;
	bool isOnPlatform;

	ULONGLONG restore_start;
	ULONGLONG walk_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithBox(LPCOLLISIONEVENT e);

public:
	CEdgeDetector* edgeDetector;

	CKoopas(float x, float y, int type, int color);
	virtual void SetState(int state);
	virtual void SetSpinDirection(int direction) { spinDirection = direction; }	
	void SetCarried(int is) { this->isCarried = is; };
};