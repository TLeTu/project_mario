#pragma once

#include "GameObject.h"

#define CARD_STATE_EMPTY 100
#define CARD_STATE_MUSHROOM 200
#define CARD_STATE_FLOWER 300
#define CARD_STATE_STAR 400

#define CARD_SPRITE_EMPTY 1000200
#define CARD_SPRITE_MUSHROOM 1000201
#define CARD_SPRITE_FLOWER 1000202
#define CARD_SPRITE_STAR 1000203


class CUICardSlot : public CGameObject
{
protected:
	bool isEmpty;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);



	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	CUICardSlot(float x, float y) { isEmpty = true; SetState(CARD_STATE_EMPTY); };
	void SetEmpty(bool isEmpty) { this->isEmpty = isEmpty; };
	bool GetEmpty() { return isEmpty; };
};