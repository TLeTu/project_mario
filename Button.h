#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BUTTON 810000
#define ID_ANI_PRESSED 820000

#define BUTTON_WIDTH 16
#define BUTTON_BBOX_WIDTH 16
#define BUTTON_BBOX_HEIGHT 16
#define BUTTON_BBOX_HEIGHT_PRESSED 7

#define BUTTON_STATE_NORMAL 100
#define BUTTON_STATE_PRESSED 200

class CButton : public CGameObject {
public:
	CButton(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);
};