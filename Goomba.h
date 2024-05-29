#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_JUMP_SPEED 0.4f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_WALK_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_JUMPING 300
#define GOOMBA_STATE_TAKEOFF 400

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_TAKEOFF 5002
#define ID_ANI_GOOMBA_JUMP 5003
#define ID_ANI_GOOMBA_WALKING_RED 5004
#define ID_ANI_GOOMBA_DIE_RED 5005

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	int goombaType;

	ULONGLONG die_start;
	ULONGLONG walk_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y, int type);
	virtual void SetState(int state);
	virtual int GetGoombaType() { return this->goombaType; };
};