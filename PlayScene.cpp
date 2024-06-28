#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Box.h"
#include "Koopas.h"
#include "Brick.h"
#include "Goomba.h"
#include "Mario.h"
#include "EdgeDetector.h"
#include "Decoration.h"
#include "Floor.h"
#include "Mushroom.h"
#include "Collision.h"
#include "Piranha.h"
#include "Fireball.h"
#include "SceneLoader.h"
#include "Tube.h"
#include "Flytrap.h"
#include "Button.h"


#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	ScenePart = 0;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;
	CGameObject* enemy = NULL;
	CGameObject* coinBrick = NULL;
	CSceneLoader* sceneLoader = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		int type = atoi(tokens[3].c_str());
		enemy = new CGoomba(x, y, type);
		break;
	}
	case OBJECT_TYPE_BRICK:
	{
		int type = atoi(tokens[3].c_str());
		int isCoin = atoi(tokens[4].c_str());
		if (isCoin)
		{
			coinBrick = new CBrick(x, y, type, isCoin);
		}
		else obj = new CBrick(x, y, type, isCoin); 
		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_KOOPAS:
	{
		int type = atoi(tokens[3].c_str());
		int color = atoi(tokens[4].c_str());
		enemy = new CKoopas(x, y, type, color);
		break;

	}
	case OBJECT_TYPE_EDGEDETECTOR: enemy = new CEdgeDetector(x, y); break;
	case OBJECT_TYPE_MUSHROOM: obj = new CMushroom(x, y); break;
	case OBJECT_TYPE_FLYTRAP: enemy = new CFlytrap(x, y); break;
	case OBJECT_TYPE_PIRANHA:
	{
		int type = atoi(tokens[3].c_str());
		enemy = new CPiranha(x, y, type);
		break;

	}
	case OBJECT_TYPE_FIREBALL: enemy = new CFireball(x, y); break;
	case OBJECT_TYPE_DECORATION: 
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CDecoration(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_TUBE:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CTube(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}


	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	
	case OBJECT_TYPE_FLOOR:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CFloor(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	case OBJECT_TYPE_BOX: 
	{
		int type = atoi(tokens[3].c_str());
		obj = new CBox(x, y, type);
		break;
	}
	case OBJECT_TYPE_SCENELOADER:
	{
		int part = atoi(tokens[3].c_str());
		sceneLoader = new CSceneLoader(x, y, part);
		break;
	}
	case OBJECT_TYPE_BUTTON: obj = new CButton(x, y); break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (obj != NULL) 
	{
		obj->SetPosition(x, y);
		objects.push_back(obj);
	}
	if (sceneLoader != NULL)
	{
		sceneLoader->SetPosition(x, y);
		sceneLoaders.push_back(sceneLoader);
	}
	if (enemy != NULL)
	{
		enemy->SetPosition(x, y);
		enemies.push_back(enemy);
	}
	if (coinBrick != NULL)
	{
		coinBrick->SetPosition(x, y);
		coinBricks.push_back(coinBrick);
	}
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (auto object : objects)
	{
		if (object != player)
		{
			coObjects.push_back(object);
		}
	}

	for (size_t i = 0; i < sceneLoaders.size(); i++)
	{
		coObjects.push_back(sceneLoaders[i]);
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		coObjects.push_back(enemies[i]);
	}

	for (size_t i = 0; i < coinBricks.size(); i++)
	{
		coObjects.push_back(coinBricks[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < coinBricks.size(); i++)
	{
		coinBricks[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < sceneLoaders.size(); i++)
	{
		if (sceneLoaders[i]->GetPart() == ScenePart + 1)
		{
			sceneLoaders[i]->GetPosition(loadPositionX, loadPositionY);
		}
	}
	for (size_t i = 0; i < enemies.size(); i++)
	{
		float ox, oy;
		enemies[i]->GetPosition(ox, oy);
		if (ox <= loadPositionX)
		{
			enemies[i]->Update(dt, &coObjects);
		}
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (size_t i = 0; i < sceneLoaders.size(); i++)
	{
		sceneLoaders[i]->Render();
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		float ox, oy;
		enemies[i]->GetPosition(ox, oy);
		if (ox <= loadPositionX && enemies[i]->GetType() == "piranha")
		{
			enemies[i]->Render();
		}
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	for (size_t i = 0; i < coinBricks.size(); i++)
	{
		coinBricks[i]->Render();
	}


	for (size_t i = 0; i < enemies.size(); i++)
	{
		float ox, oy;
		enemies[i]->GetPosition(ox, oy);
		if (ox <= loadPositionX && enemies[i]->GetType() != "piranha")
		{
			enemies[i]->Render();
		}
	}

}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();

	for (it = coinBricks.begin(); it != coinBricks.end(); it++)
	{
		delete (*it);
	}
	coinBricks.clear();

	for (it = enemies.begin(); it != enemies.end(); it++)
	{
		delete (*it);
	}
	enemies.clear();

	vector<LPSCENELOADER>::iterator lt;
	for (lt = sceneLoaders.begin(); lt != sceneLoaders.end(); it++)
	{
		delete (*lt);
	}
	sceneLoaders.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	for (int i = 0; i < enemies.size(); i++)
		delete enemies[i];

	enemies.clear();

	for (int i = 0; i < coinBricks.size(); i++)
		delete coinBricks[i];

	coinBricks.clear();

	for (int i = 0; i < sceneLoaders.size(); i++)
		delete sceneLoaders[i];

	sceneLoaders.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	for (it = coinBricks.begin(); it != coinBricks.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	for (it = enemies.begin(); it != enemies.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	vector<LPSCENELOADER>::iterator lt;
	for (lt = sceneLoaders.begin(); lt != sceneLoaders.end(); lt++)
	{
		LPSCENELOADER o = *lt;
		if (o->IsDeleted())
		{
			delete o;
			*lt = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());

	coinBricks.erase(
		std::remove_if(coinBricks.begin(), coinBricks.end(), CPlayScene::IsGameObjectDeleted),
		coinBricks.end());


	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(), CPlayScene::IsGameObjectDeleted),
		enemies.end());

	sceneLoaders.erase(
		std::remove_if(sceneLoaders.begin(), sceneLoaders.end(), CPlayScene::IsGameObjectDeleted),
		sceneLoaders.end());
}

void CPlayScene::AddGameObject(LPGAMEOBJECT obj)
{
	objects.push_back(obj);
}

void CPlayScene::GetPlayerPosition(float& x, float& y)
{
	player->GetPosition(x, y);
}

void CPlayScene::SetScenePart(int part)
{
	ScenePart = part;
	Render();
}

void CPlayScene::BrickToCoin()
{
	float bx, by;
	if (coinBricks.size() > 0)
	{
		for (int i = 0; i < coinBricks.size(); i++)
		{
			coinBricks[i]->GetPosition(bx, by);
			LPGAMEOBJECT coin = new CCoin(bx, by);
			coinBricks[i]->Delete();
			coin->SetState(COIN_STATE_TOBRICK);
			AddGameObject(coin);
		}
	}
	else return;
}

void CPlayScene::CoinToBrick(LPGAMEOBJECT obj)
{
	coinBricks.push_back(obj);
}