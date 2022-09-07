/**************************************************************************************************
* \file	    StateTesting.h
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
#ifndef STATETESTING_H_
#define STATETESTING_H_

#include "State.h"
class StateTest : public State
{
	public:
		StateTest( void );
		~StateTest( void );

		void Load( void );
		void Init( void );
		void Update( void );
		void Draw( void );
		void Unload( void );
		void Free( void );
};

extern StateTest gStateTest;

#endif