/**************************************************************************************************
* \file	    GameObj.h
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
* \brief	Game object class definitions.\n
*			Copyright (C) 2012 DigiPen Institute of Technology. Reproduction
* 			or disclosure of this file or its contents without the prior written consent of DigiPen
* 			Institute of Technology is prohibited.
**************************************************************************************************/
#ifndef GAMEOBJ_H_
#define GAMEOBJ_H_

#include "NXEngine.h"
#include "GameObjManager.h"

enum ObjType
{
	TYPE_NONE = 0,
	TYPE_PLAYER,
	TYPE_RAW_GERM,
	TYPE_MELEE_ENEMY_BASIC,
	TYPE_MELEE_ENEMY_ARMOR,
	TYPE_MELEE_ENEMY_JEWEL,
	TYPE_MELEE_ENEMY_CHARGER,
	TYPE_RANGE_ENEMY_BASIC,
	TYPE_MELEE_ENEMY_FAIRY,
	TYPE_MELEE_ENEMY_FATTY,
	TYPE_FIRE,		
	TYPE_ICE,
	TYPE_WIND,
	TYPE_COMBOSPELLFIRE,
	TYPE_COMBOSPELLICE,
	TYPE_COMBOSPELLWIND,
	TYPE_PROJECTILE,

	TYPE_TOTAL
};

enum POWER{
	FIREPOW = 0,
	ICEPOW,
	WINDPOW,
	NOPOW
};

//FLAGS
const unsigned int FLAG_ACTIVE          = 0x00000001;
const unsigned int FLAG_VISIBLE			= 0x00000002;

class GameObj : public NXGameObj
{
	public:
		GameObj( size_t index,
						const std::wstring& meshID = L"",
						const std::wstring& spriteID = L"");
		virtual ~GameObj( void );

		virtual void Init( void );
		virtual void Update( void );
		virtual void UpdateData(void) {}

		bool IsInAir(void) const;
		bool IsOnGround(void) const;
		bool IsHitGround(void) const; //True when object lands on ground for first time
		float GetCurrentRegionY(void);

		void SetBoundingVolume( const Vec3& min , const Vec3& max );
		void SetBoundingActive( bool active ) { isBoundingActive = active; }

		void SetZoneBoundingActive( bool active ) { isZoneBoundActive = active; }

		Vec3 GetBoundingMin( void ) const { return mBoundingMin; }
		Vec3 GetBoundingMax( void ) const { return mBoundingMax; }
		bool IsBoundingActive( void ) const { return isBoundingActive; }
		bool IsZoneBoundActive( void ) const { return isZoneBoundActive; }
		
		//void UpdateState(ObjType inState);
		void UpdateTileCollision( void );
		
		size_t GetObjectIndex( void ) const { return mObjectIndex; }

		ObjType mType;
	private:
		size_t mObjectIndex;
		Vec3 mBoundingMin;
		Vec3 mBoundingMax;
		bool isBoundingActive;
		bool isZoneBoundActive;
		NXTimer clock;
		bool isOnGroundCurr;
		bool isOnGroundPrev;
};

extern ObjManager<GameObj> gObjManager;
#endif