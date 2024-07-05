#pragma once
#include "GameObject.h"

#define LEAF_GRAVITY 0.05f

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14
#define LEAF_STATE_LEFT 100
#define LEAF_STATE_RIGHT 200
#define LEAF_STATE_UP 300

#define ID_ANI_LEAF 1211110

class CLeaf : public CGameObject
{
protected:
	ULONGLONG state_timer;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void SetState(int state);

public:
	CLeaf(float x, float y) : CGameObject(x, y)
	{
		state_timer = -1;
		SetState(LEAF_STATE_UP);
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};
