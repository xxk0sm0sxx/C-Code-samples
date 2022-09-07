/**************************************************************************************************
* \file	    StateTesting.cpp
* \author	Lim Hao Jie Sherman, 250003311\n
* 			Lim Yen Wei, 250002911\n
* 			Scott Lim, 250005111\n
* 			Peh Zhe Rong, 250004911\n
*\par   	email:	haojie.lim\@digipen.edu\n
* 		            yenwei.lim\@digipen.edu\n
*        		    scott.lim\@digipen.edu\n
* 		            peh.rong\@digipen.edu\n
*\par       Course: GAM200
*\par       Game Project BlastBasher
*\date      10/08/2012
* \brief	A game level state for play testing.\n
*			Copyright (C) 2012 DigiPen Institute of Technology. Reproduction
* 			or disclosure of this file or its contents without the prior written consent of DigiPen
* 			Institute of Technology is prohibited.
**************************************************************************************************/
#include "StateTesting.h"
#include "StateManager.h"
#include "Globals.h"
#include "InputManager.h"
#include "DebugConsole.h"
#include "GameEditor.h"
#include "NXAssert.h"
#include "tinyxml.h"
#include <string>

StateTest gStateTest;
static EnemyObj* objEnemy = 0;
static bool isCollided = false;

/**************************************************************************************************
 * \fn	StateTest::StateTest()
 *
 * \brief	Default constructor.
**************************************************************************************************/

StateTest::StateTest()
{
}

/**************************************************************************************************
 * \fn	StateTest::~StateTest()
 *
 * \brief	Destructor.
**************************************************************************************************/

StateTest::~StateTest()
{
	State::~State();
}

/**************************************************************************************************
 * \fn	void StateTest::Load( void )
 *
 * \brief	Loads this object.
**************************************************************************************************/

void StateTest::Load( void )
{	
}

/**************************************************************************************************
 * \fn	void StateTest::Init( void )
 *
 * \brief	Initialises this object.
**************************************************************************************************/

void StateTest::Init( void )
{
	Vec3 pos(0,0,-1);

	Vec3 size2(4.0f,4.0f,1.0f);
	player = playerObjManager.CreateGameObj(L"SQUARETOP", L"SONIC");
	player->SetPosition(pos);
	player->SetScale(size2);
	player->SetCurrentAnimation(L"Walk",0.1f);
	
	NX_ASSERT(player);
	
	SetVector(pos, 10.0f, 0.0f, -1);
	size2 = Vec3(2.0f, 2.0f, 1.0f);
	objEnemy = meleeEnemyObjManager.CreateGameObj(L"SQUARETOP", L"PLAYER1_2");
	objEnemy->FSMInit();
	objEnemy->SetPosition(pos);
	objEnemy->SetScale(size2);
	objEnemy->SetCurrentAnimation(L"Walk",0.1f);
	objEnemy->SetTarget(player);

	NX_ASSERT(objEnemy);
	
	//gConsole.DebugInit();
}

/**************************************************************************************************
 * \fn	void StateTest::Update( void )
 *
 * \brief	Updates this object.
**************************************************************************************************/

void StateTest::Update( void )
{
	if (NXKeyIsReleased(NXVK_ESCAPE))
		gStateManager.SetNextState(STATE_QUIT);

	if (NXKeyIsTriggered(NXVK_F8))
		gStateManager.SetNextState(STATE_SANDBOX);
	/*
	player->SetAABB(player->GetPosition(), player->GetScale());
	objEnemy->SetAABB(objEnemy->GetPosition(), objEnemy->GetScale());
	gConsole.DebugUpdate();

	if (CollisionIntersection_OverlapRectRect(player->GetAABB(), objEnemy->GetAABB()))
	{
		isCollided = true;
	}
	else
		isCollided = false;

	if (NXKeyIsPressed(NXVK_W))
		player->MoveUp(1.0f);
	if (NXKeyIsPressed(NXVK_S))
		player->MoveDown(1.0f);
	if (NXKeyIsPressed(NXVK_A))
		player->MoveLeft(1.0f);
	if (NXKeyIsPressed(NXVK_D))
		player->MoveRight(1.0f);
	if (NXKeyIsPressed(NXVK_Q))
		player->AddForce(Force(MakeVec(0,1,0), 1.0, 0.1f));
	if (NXKeyIsPressed(NXVK_E))
		player->AddForce(Force(MakeVec(0,-1,0), 1.0, 0.1f));

	player->SetVelocity(player->GetVelocity() * 0.99f);

	player->SetPosition(player->GetPosition() += player->GetVelocity() * g_dt);
	*/
	UpdateAllObjManagers();
}

/**************************************************************************************************
 * \fn	void StateTest::Draw( void )
 *
 * \brief	Draws this object.
**************************************************************************************************/

void StateTest::Draw( void )
{
	NXGraphicEngine* ge = gEngine.GetGraphicEngine();
	if (ge == 0)
	{
		return;
	}

	// Begin the scene
	ge->BeginScene();
	
	char buffer[256];

	ge->SetBackBufferColor(NXCOLOR_ARGB(255, 0, 0, 0 ));

	sprintf_s(buffer,"player pos: %.2f,%.2f,%.2f",player->GetPosition().x,player->GetPosition().y,player->GetPosition().z);	
	gEngine.GetGraphicEngine()->NXPrint(5,30,0xffffffff,buffer,g_Font);

	sprintf_s(buffer,"enemy pos: %.2f,%.2f,%.2f",objEnemy->GetPosition().x,objEnemy->GetPosition().y,objEnemy->GetPosition().z);	
	gEngine.GetGraphicEngine()->NXPrint(5,32,0xffffffff,buffer,g_Font);

	if (isCollided)
	{
		sprintf_s(buffer,"isCollided: true");
		gEngine.GetGraphicEngine()->NXPrint(5,40,0xffffffff,buffer,g_Font);
	}
	else
	{
		sprintf_s(buffer,"isCollided: false");
		gEngine.GetGraphicEngine()->NXPrint(5,40,0xffffffff,buffer,g_Font);
	}
	gConsole.DebugDraw();
	ge->EndScene();

	RenderAllObjManagers();
}

/**************************************************************************************************
 * \fn	void StateTest::Unload( void )
 *
 * \brief	Unloads this object.
**************************************************************************************************/

void StateTest::Unload( void )
{
}

/**************************************************************************************************
 * \fn	void StateTest::Free( void )
 *
 * \brief	Frees this object.
**************************************************************************************************/

void StateTest::Free( void )
{

	FreeAllObjManagers();
}