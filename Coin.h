#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000
#define ID_ANI_COIN_STAR 11002
#define ID_ANI_COIN_MUSHROOM 11003
#define ID_ANI_COIN_FLOWER 11004

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_NORMAL 100
#define COIN_STATE_FAKE_UP 200
#define COIN_STATE_FAKE_DOWN 300
#define COIN_STATE_TOBRICK 400
#define COIN_STATE_FAKE_STAR 500
#define COIN_STATE_FAKE_MUSHROOM 600
#define COIN_STATE_FAKE_FLOWER 700

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