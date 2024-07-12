#include "Portal.h"
#include "Game.h"
#include "Textures.h"
#include "Mario.h"
#include "debug.h"
CPortal::CPortal(float x, float y, int scene_id, float px, float py ) :CGameObject(x, y)
{
	this->scene_id = scene_id;
	this->px = px;
	this->py = py;
}

void CPortal::Render()
{
	//RenderBoundingBox();
}


void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - PORTAL_BBOX_WIDTH / 2;
	t = y - PORTAL_BBOX_HEIGHT / 2;
	r = l + PORTAL_BBOX_WIDTH;
	b = t + PORTAL_BBOX_HEIGHT;
}