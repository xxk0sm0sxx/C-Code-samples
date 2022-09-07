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
#include "NXTileMap.h"
#include <fstream>
#include "tinyxml.h"
#include "NXAssert.h"

NXTileMap gCollisionTile;

/**************************************************************************************************
 * \fn	NXTileMap::NXTileMap( void )
 *
 * \brief	Default constructor.
**************************************************************************************************/

NXTileMap::NXTileMap( void ):
     BINARY_MAP_WIDTH (0),
	 BINARY_MAP_HEIGHT(0),
	 MapData(0),
	 BinaryCollisionArray(0)
{
}

/**************************************************************************************************
 * \fn	NXTileMap::~NXTileMap( void )
 *
 * \brief	Destructor.
**************************************************************************************************/

NXTileMap::~NXTileMap( void )
{
	//FreeMapData();
}

//Tile Collision and getting data etc
/******************************************************************************/
/*!
\brief
	This function retrieves the value of the element (X;Y) in BinaryCollisionArray.
	Before retrieving the value, it should check that the supplied X and Y values
	are not out of bounds (in that case return 0)
\param X
	The which colum in which you want to find out
\param Y
	The which row in which you want to find out
\return
	Return the value in which it is stored if its out of bound will return 0

*/
/******************************************************************************/
int NXTileMap::GetCellValue(const int& X, const int& Y)
{
	int tempReturn = 0;

	if(X < BINARY_MAP_WIDTH  && 
	   Y < BINARY_MAP_HEIGHT &&
	   X >= 0 && Y >= 0)

	tempReturn = BinaryCollisionArray[X + Y * BINARY_MAP_WIDTH];
	
	return tempReturn;
}



/******************************************************************************/
/*!
\brief
	This function creates 2 hot spots on each side of the object instance, 
	and checks if each of these hot spots is in a collision area (which means 
	the cell if falls in has a value of 1).
	At the beginning of the function, a "Flag" integer should be initialized to 0.
	Each time a hot spot is in a collision area, its corresponding bit 
	in "Flag" is set to 1.
	Finally, the function returns the integer "Flag"
	The position of the object instance is received as PosX and PosY
	The size of the object instance is received as scaleX and scaleY

	Note: This function assume the object instance's size is 1 by 1 
	(the size of 1 tile)
	Creating the hotspots:\n
	-Handle each side separately.\n
	-2 hot spots are needed for each collision side.\n
	-These 2 hot spots should be positioned on 1/4 above the center 
	and 1/4 below the center

\param PosX
	The X position of the object
\param PosY
	The Y position of the object
\param scaleX
	The X scale of the object to find the end scaled position
\param scaleY
	The Y scale of the object to find the end scaled position
\return
	The Corresponding bit that in which they are being activated.\n
	Example:\n
	Left will be 0001\n
	Right will be 0010\n
	Top will be 0100\n
	Down will be 1000\n
*/
/******************************************************************************/
int NXTileMap::CheckInstanceBinaryMapCollision(	const float& PosX,
												const float& PosY, 
												const float& scaleX,
												const float& scaleY)
{
	float x1, y1, x2, y2;
	float x3, y3, x4, y4;
	float x5, y5, x6, y6;
	float x7, y7, x8, y8;
	int flag = 0;
	//hotspot 1
	x1 = PosX + scaleX/2;    //To reach the right side
	y1 = PosY + scaleY/4;	//To go up 1/4 of the height

	//hotspot 2
	x2 = PosX + scaleX/2;	//To reach the right side
	y2 = PosY - scaleY/4;	//To go down 1/4 of the height

	//hotspot 3
	x3 = PosX + scaleX/4;    //To go up 1/4 of the width
	y3 = PosY - scaleY/2;	//To reach the down side

	//hotspot 4
	x4 = PosX - scaleX/4;	//To go down 1/4 of the width
	y4 = PosY - scaleY/2;	//To reach the down side

	//hotspot 5
	x5 = PosX - scaleX/2;    //To reach the left side
	y5 = PosY + scaleY/4;	//To go up 1/4 of the height

	//hotspot 6
	x6 = PosX - scaleX/2;	//To reach the left side
	y6 = PosY - scaleY/4;	//To go down 1/4 of the height

	//hotspot 7
	x7 = PosX + scaleX/4;    //To go up 1/4 of the width
	y7 = PosY + scaleY/2;	//To reach the up side

	//hotspot 8
	x8 = PosX - scaleX/4;	//To go down 1/4 of the width
	y8 = PosY + scaleY/2;	//To reach the up side


	if(GetCellValue(int(x1),int(y1))||GetCellValue(int(x2),int(y2)))
	{
		flag = flag | COLLISION_RIGHT;
	}

	if(GetCellValue(int(x3),int(y3))||GetCellValue(int(x4),int(y4)))
	{
		flag = flag | COLLISION_BOTTOM;
	}

	if(GetCellValue(int(x5),int(y5))||GetCellValue(int(x6),int(y6)))
	{
		flag = flag | COLLISION_LEFT;
	}

	if(GetCellValue(int(x7),int(y7))||GetCellValue(int(x8),int(y8)))
	{
		flag = flag | COLLISION_TOP;
	}

	return flag;
}


/******************************************************************************/
/*!
\brief
	This function snaps the value sent as parameter to the center of the cell.
	It is used when a sprite is colliding with a collision area from one 
	or more side.\n
	To snap the value sent by "Coordinate", find its integral part by type 
	casting it to an integer, then add 0.5 (which is half the cell's width 
	or height)
\param Coordinate
	The value in which to snap to. 
*/
/******************************************************************************/
void NXTileMap::SnapToCell(float *Coordinate)
{
	*Coordinate = float(int(*Coordinate))+0.5f;
}

/******************************************************************************/
/*!
\brief
	This function opens the file name "FileName" and retrieves all the map 
	data.\n
	It allocates memory for the 2 arrays: MapData & BinaryCollisionArray\n
	The first line in this file is the width of the map.\n
	The second line in this file is the height of the map.\n
	The remaining part of the file is a series of numbers\n
	Each number represents the ID (or value) of a different element in the
	double dimensionaly array.\n
	\n
	Example:\n
	\n
	Width 5\n
	Height 5\n
	1 1 1 1 1\n
	1 1 1 3 1\n
	1 4 2 0 1\n
	1 0 0 0 1\n
	1 1 1 1 1\n
	\n
	\n
	After importing the above data, "MapData" and " BinaryCollisionArray" 
	should be\n
	\n
	1 1 1 1 1\n
	1 1 1 3 1\n
	1 4 2 0 1\n
	1 0 0 0 1\n
	1 1 1 1 1\n
	\n
	and\n
	\n
	1 1 1 1 1\n
	1 1 1 0 1\n
	1 0 0 0 1\n
	1 0 0 0 1\n
	1 1 1 1 1\n
	\n
	respectively.\n
	\n
	Finally, the function returns 1 if the file named "FileName" exists, 
	otherwise it returns 0\n
	\param FileName
	This is the file in which to read the data from
\return
	return a 1 if the data is read correctly
*/
/******************************************************************************/
int NXTileMap::ImportMapDataFromFile(char *FileName)
{
	// Memory allocated to open xml document
	TiXmlDocument doc;
	// Fails to load
	if(!doc.LoadFile(FileName))
	{			
		// Display error info, free memory
		NX_MESG(doc.ErrorDesc());
		doc.Clear();
		return 0;
	}

	// Pointer to elements in the xml file
	TiXmlElement* NXState, *Dimension, *Map;

	// Get root element
	NXState = doc.FirstChildElement("NXState");	
	
	// Get map info elements
	Dimension = NXState->FirstChildElement("Dimension");
	Map       = NXState->FirstChildElement("Map"); 
		
	// Load Width and Height
	if(Dimension->QueryIntAttribute("Width",&BINARY_MAP_WIDTH)
	   != TIXML_SUCCESS)
		NX_MESG("Unable to get width attribute!");

	if(Dimension->QueryIntAttribute("Height",&BINARY_MAP_HEIGHT)
	   != TIXML_SUCCESS)
		NX_MESG("Unable to get height attribute!");

	// Load map array
	const char *mapText = Map->GetText();
	
	char buffer[512];
	sprintf_s(buffer,"%s",mapText);	
	
	// Allocate memory for map and binary
	MapData = new int[BINARY_MAP_WIDTH*BINARY_MAP_HEIGHT];
	BinaryCollisionArray = new int[BINARY_MAP_WIDTH*BINARY_MAP_HEIGHT];

	// store map data
	int i = 0, j=0;
	while(buffer[i] != NULL)
	{
		if(buffer[i] != ' ')
		{
			MapData[j] = buffer[i] - '0';
			BinaryCollisionArray[j] = buffer[i] - '0';
			++j;
		}
			++i;
	}
	
	

	//free tinyxml doc memory
	doc.Clear();		
	return 1;
}

/**************************************************************************************************
 * \fn	int * NXTileMap::GetMapData( void ) const
 *
 * \brief	Gets the map data.
 *
 * \return	null if it fails, else the map data.
**************************************************************************************************/
int * NXTileMap::GetMapData( void ) const
{
	return MapData;
}

/**************************************************************************************************
 * \fn	int * NXTileMap::GetCollisionData( void ) const
 *
 * \brief	Gets the collision data.
 *
 * \return	null if it fails, else the collision data.
**************************************************************************************************/

int * NXTileMap::GetCollisionData( void ) const
{
	return BinaryCollisionArray;
}

/**************************************************************************************************
 * \fn	int NXTileMap::GetWidth( void ) const
 *
 * \brief	Gets the width.
 *
 * \return	The width.
**************************************************************************************************/

int  NXTileMap::GetWidth( void ) const
{
	return BINARY_MAP_WIDTH;
}

/**************************************************************************************************
 * \fn	int NXTileMap::GetHeight( void ) const
 *
 * \brief	Gets the height.
 *
 * \return	The height.
**************************************************************************************************/

int  NXTileMap::GetHeight( void ) const
{
	return BINARY_MAP_HEIGHT;
}



/******************************************************************************/
/*!
\brief
	This function frees the memory that was allocated for the 2 arrays MapData 
	& BinaryCollisionArray which was allocated in the "ImportMapDataFromFile" 
	function\n
*/
/******************************************************************************/
void NXTileMap::FreeMapData( void )
{
	if(MapData)
		delete [] MapData;

	if(BinaryCollisionArray)
		delete [] BinaryCollisionArray;
}



