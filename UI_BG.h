#pragma once

#include "GameObject.h"



#define UIBG_ID_SPRITE 1000600


class CUIBG : public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};



	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Render();
	CUIBG(float x, float y);
};