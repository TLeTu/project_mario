#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Koopas.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.25f

#define MARIO_ACCEL_WALK_X	0.0004f
#define MARIO_ACCEL_RUN_X	0.0005f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_CARRYING_SHELL	700

#define MARIO_STATE_FLY 800
#define MARIO_STATE_RELEASE_FLY 801

#define MARIO_STATE_FLAP 900

#define MARIO_STATE_ATTACK 1000


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 110000
#define ID_ANI_MARIO_IDLE_LEFT 110010

#define ID_ANI_MARIO_WALKING_RIGHT 110100
#define ID_ANI_MARIO_WALKING_LEFT 110110

#define ID_ANI_MARIO_RUNNING_RIGHT 110300
#define ID_ANI_MARIO_RUNNING_LEFT 110310

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 110500
#define ID_ANI_MARIO_JUMP_WALK_LEFT 110510

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 110600
#define ID_ANI_MARIO_JUMP_RUN_LEFT 110610

#define ID_ANI_MARIO_SIT_RIGHT 110700
#define ID_ANI_MARIO_SIT_LEFT 110710

#define ID_ANI_MARIO_BRACE_RIGHT 110400
#define ID_ANI_MARIO_BRACE_LEFT 110410

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 100000
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 100010

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 100100
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 100110

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 100300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 100310

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 100400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 100410

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 100500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 100510

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 100600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 100610

// RACOON MARIO
#define ID_ANI_MARIO_RACOON_IDLE_RIGHT	120000
#define ID_ANI_MARIO_RACOON_IDLE_LEFT	120010

#define ID_ANI_MARIO_RACOON_WALKING_RIGHT	120100
#define ID_ANI_MARIO_RACOON_WALKING_LEFT	120110

#define ID_ANI_MARIO_RACOON_RUNNING_RIGHT	120300
#define ID_ANI_MARIO_RACOON_RUNNING_LEFT	120310

#define ID_ANI_MARIO_RACOON_BRACE_RIGHT	120400
#define ID_ANI_MARIO_RACOON_BRACE_LEFT	120410

#define ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT	120500
#define ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT	120510

#define ID_ANI_MARIO_RACOON_JUMP_RUN_RIGHT	120600
#define ID_ANI_MARIO_RACOON_JUMP_RUN_LEFT	120610

#define ID_ANI_MARIO_RACOON_SIT_RIGHT	120700
#define ID_ANI_MARIO_RACOON_SIT_LEFT	120710

#define ID_ANI_MARIO_RACOON_FLY_RIGHT	122200
#define ID_ANI_MARIO_RACOON_FLY_LEFT	122210

#define ID_ANI_MARIO_RACOON_FLAP_RIGHT	122100
#define ID_ANI_MARIO_RACOON_FLAP_LEFT	122110

#define ID_ANI_MARIO_RACOON_GLIDE_RIGHT	122300
#define ID_ANI_MARIO_RACOON_GLIDE_LEFT	122310

#define ID_ANI_MARIO_RACOON_SPIN_RIGHT	122000
#define ID_ANI_MARIO_RACOON_SPIN_LEFT	122010

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float runningCharge;	// charge for flying

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	ULONGLONG fall_start;
	ULONGLONG attack_start;
	BOOLEAN isOnPlatform;
	int coin; 

	CKoopas* carriedKoopas;
	bool isCarryingKoopas;
	bool isHolding;
	bool inPortal;
	int portalId;
	bool chargedFlying;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithBox(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithFireball(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranha(LPCOLLISIONEVENT e);
	void OnCollisionWithSceneLoader(LPCOLLISIONEVENT e);
	void OnCollisionWithFlytrap(LPCOLLISIONEVENT e);
	void OnCollisionWithButton(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithLuckyBox(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();

public:

	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_RACOON;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;

		isCarryingKoopas = false;
		carriedKoopas = nullptr;

		isHolding = false;
		inPortal = false;
		portalId = -1;

		runningCharge = 0.0f;
		chargedFlying = false;
		fall_start = -1;
		attack_start = -1;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return level; }
	bool ChargedFlying() { return chargedFlying; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetHolding(bool r) { isHolding = r; }
};