#include "Goomba.h"

CGoomba::CGoomba(float x, float y, int type):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	this->goombaType = type;
	die_start = -1;
	walk_start = -1;
	if (goombaType) SetState(GOOMBA_STATE_JUMPING);
	else SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 && GetState() == GOOMBA_STATE_JUMPING)
	{
		vy = 0;
		SetState(GOOMBA_STATE_TAKEOFF);
	}
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}

	if (state == GOOMBA_STATE_TAKEOFF && (GetTickCount64() - walk_start > GOOMBA_WALK_TIMEOUT))
	{
		SetState(GOOMBA_STATE_JUMPING);
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = -1;
	if (goombaType) 
	{
		if (state == GOOMBA_STATE_WALKING) aniId = ID_ANI_GOOMBA_WALKING_RED;
		else if (state == GOOMBA_STATE_DIE) aniId = ID_ANI_GOOMBA_DIE_RED;
		else if (state == GOOMBA_STATE_TAKEOFF) aniId = ID_ANI_GOOMBA_TAKEOFF;
		else if (state == GOOMBA_STATE_JUMPING) aniId = ID_ANI_GOOMBA_JUMP;
	}
	else
	{
		if (state == GOOMBA_STATE_WALKING) aniId = ID_ANI_GOOMBA_WALKING;
		else if (state == GOOMBA_STATE_DIE) aniId = ID_ANI_GOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	/*RenderBoundingBox();*/
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			ay = GOOMBA_GRAVITY;
			vy = 0;
			break;
		case GOOMBA_STATE_TAKEOFF:
			vx = -GOOMBA_WALKING_SPEED;
			walk_start = GetTickCount64();
			break;
		case GOOMBA_STATE_JUMPING:
			vx = -GOOMBA_WALKING_SPEED;
			vy = -GOOMBA_JUMP_SPEED;
			ay = GOOMBA_GRAVITY - 0.0015f;
			break;
	}
}
