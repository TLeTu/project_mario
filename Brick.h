#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_BREAK 10001
#define ID_ANI_WOOD	10002
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_BREAK 100
#define BRICK_STATE_NORMAL 200
#define BRICK_STATE_WOOD 300

class CBrick : public CGameObject {
protected:
	ULONGLONG break_start;
	int brickType;
	int isCoin;
public:
	CBrick(float x, float y, int type, int isCoin);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);
	virtual int IsCollidable();
};