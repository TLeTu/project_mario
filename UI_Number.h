#pragma once

#include "GameObject.h"


#define NUM_ID_SPRITE_0 1000300
#define NUM_ID_SPRITE_1 1000301
#define NUM_ID_SPRITE_2 1000302
#define NUM_ID_SPRITE_3 1000303
#define NUM_ID_SPRITE_4 1000304
#define NUM_ID_SPRITE_5 1000305
#define NUM_ID_SPRITE_6 1000306
#define NUM_ID_SPRITE_7 1000307
#define NUM_ID_SPRITE_8 1000308
#define NUM_ID_SPRITE_9 1000309

#define NUM_STATE_0 0
#define NUM_STATE_1 1
#define NUM_STATE_2 2
#define NUM_STATE_3 3
#define NUM_STATE_4 4
#define NUM_STATE_5 5
#define NUM_STATE_6 6
#define NUM_STATE_7 7
#define NUM_STATE_8 8
#define NUM_STATE_9 9


class CUINum : public CGameObject
{
protected:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};


	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Render();
	virtual void SetState(int state) { this->state = state; };
	CUINum(float x, float y);
};