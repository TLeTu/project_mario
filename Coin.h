#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_NORMAL 100
#define COIN_STATE_FAKE_UP 200
#define COIN_STATE_FAKE_DOWN 300
#define COIN_STATE_TOBRICK 400

class CCoin : public CGameObject {
protected:
	float initY;
	ULONGLONG to_brick_start;
public:
	CCoin(float x, float y);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetState(int state);
};