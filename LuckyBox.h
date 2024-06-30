#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define	LUCKYBOX_WIDTH 26
#define LUCKYBOX_BBOX_WIDTH 26
#define LUCKYBOX_BBOX_HEIGHT 26

#define ID_ANI_LUCKYBOX_STAR 910000
#define ID_ANI_LUCKYBOX_MUSHROOM 920000
#define ID_ANI_LUCKYBOX_FLOWER 930000
#define ID_ANI_LUCKYBOX_EMPTY 940000

#define LUCKYBOX_STATE_STAR 100
#define LUCKYBOX_STATE_MUSHROOM 200
#define LUCKYBOX_STATE_FLOWER 300
#define LUCKYBOX_STATE_EMPTY 400


class CLuckyBox : public CGameObject {
protected:
	bool isRolling;
	ULONGLONG rolling_time;
public:
	CLuckyBox(float x, float y);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetState(int state);
	void SetRolling(bool isRolling) { this->isRolling = isRolling; }
	void SpawnCoin();
};