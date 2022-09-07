/**************************************************************************************************
* \file	    NXTileMap.h
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
* \brief	Tile Map generator\n
*			Copyright (C) 2012 DigiPen Institute of Technology. Reproduction
* 			or disclosure of this file or its contents without the prior written consent of DigiPen
* 			Institute of Technology is prohibited.
**************************************************************************************************/
#include "NXMaths.h"

#ifndef NXTILE_H_
#define NXTILE_H_
class NXTileMap
{
	enum BLOCKTYPE
	{
		TYPE_OBJECT_EMPTY = 0,
		TYPE_OBJECT_COLLISION
	};

	public:
		NXTileMap( void );
		~NXTileMap( void );
		int	GetCellValue(const int& X, const int& Y);
		int	CheckInstanceBinaryMapCollision(const float& PosX,
											const float& PosY, 
											const float& scaleX,
											const float& scaleY);
		void SnapToCell(float *Coordinate);
		int	ImportMapDataFromFile(char *FileName);
		void FreeMapData( void );
		int * GetMapData( void ) const;
		int * GetCollisionData( void ) const;
		int  GetWidth( void ) const;
		int  GetHeight( void ) const;

	private:
		int BINARY_MAP_WIDTH;
		int BINARY_MAP_HEIGHT;
		int *MapData;
		int *BinaryCollisionArray;
};

extern NXTileMap gCollisionTile;

#endif

