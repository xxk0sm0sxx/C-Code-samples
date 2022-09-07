/**************************************************************************************************
* \file	    GameObjManager.cpp
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
* \brief	GameObjManager object class definitions.\n
*			Copyright (C) 2012 DigiPen Institute of Technology. Reproduction
* 			or disclosure of this file or its contents without the prior written consent of DigiPen
* 			Institute of Technology is prohibited.
**************************************************************************************************/
#ifndef OBJMANAGER_H_
#define OBJMANAGER_H_

#include <list>

template <class T>
class ObjManager
{
	public:
		ObjManager(size_t listSize = 512);
		~ObjManager( void );

		
		T* CreateGameObj(	const std::wstring& meshID = L"",
										const std::wstring& spriteID = L"");

		void Update( void );

		void RenderSameObjects( void ); 
		//If all objects in this manager has the same sprite, use this one as it's faster
		void Render( void ); 
		//If all objects in this manager can have different sprites, use this one

		void RenderDebugInfo( void );
		void Free(void);

		std::vector<T>& GetManagerList(void) {return mObjList; }
		size_t GetObjManagerSize(void) const {return mObjList.size(); }
	private:
		std::vector<T> mObjList;
		size_t mListSize;
		size_t mCurrentIndex;
		size_t mObjectsInUse;
};

template <class T>
ObjManager<T>::ObjManager(size_t listSize) : 
	mListSize(listSize), mCurrentIndex(0), mObjectsInUse(0)
{
	for (size_t i = 0; i < listSize; ++i)
	{
		mObjList.push_back(T(i));
	}
}

template <class T>
ObjManager<T>::~ObjManager( void )
{
	mObjList.clear();
	mListSize = 0;
}

template <class T>
T*  ObjManager<T>::CreateGameObj(	const std::wstring& meshID,
									const std::wstring& spriteID)
{
	size_t prevIndex = mCurrentIndex;
	while (mObjList[mCurrentIndex].IsAlive())
	{
		++mCurrentIndex;

		if (mCurrentIndex >= mObjList.size())
		{
			mCurrentIndex = 0;
		}
		if (prevIndex == mCurrentIndex)
		{
			NX_MESG(L"ObjManager: Out of memory\n");
			return 0;
		}
		//Out of memory if loop went through twice
	}

	if (mObjectsInUse < mCurrentIndex)
		mObjectsInUse = mCurrentIndex;

	T *obj = &mObjList[mCurrentIndex];
	obj->Init();
	obj->SetMeshID(meshID);
	obj->SetSpriteID(spriteID);
	//HARDCORE
	obj->SetCurrentAnimation(L"", 0);
	obj->SetAlive();
	++mObjectsInUse;
	return obj;
}

template <class T>
void ObjManager<T>::Render( void )
{
	size_t index = 0;
	std::wstring previousSprite = L"";
	std::wstring currentSprite = L"";
	std::wstring previousMesh = L"";
	std::wstring currentMesh = L"";

	for (size_t i = 0; i < mObjList.size(); ++i)
	{
		++index;
		if (index > mObjectsInUse)
		{
			break;
		}

		if (!mObjList[i].IsAlive())
		{
			continue;
		}
			
		if (mObjList[i].IsVisible())
		{
			currentSprite = mObjList[i].GetSpriteID();
			currentMesh = mObjList[i].GetMeshID();

			if (currentSprite != previousSprite)
			{
				gEngine.GetGraphicEngine()->SetTexture(gEngine.GetMeshManager()->GetTexture( currentSprite ) );
				previousSprite = currentSprite;
			}
			if (currentMesh != previousMesh)
			{
				gEngine.GetGraphicEngine()->SetVertices(gEngine.GetMeshManager()->GetMesh ( currentMesh )->GetBuffer() );
				previousMesh = currentMesh;
				mObjList[i].SetRenderMode();
			}
			mObjList[i].Render();
			++DrawCall;
		}
	}
}

template <class T>
void ObjManager<T>::RenderSameObjects( void )
{
	bool firstObj = true;
	size_t index = 0;

	for (size_t i = 0; i < mObjList.size(); ++i)
	{
		++index;
		if (index > mObjectsInUse)
		{
			break;
		}

		if (!mObjList[i].IsAlive())
		{
			continue;
		}
			
		if (mObjList[i].IsVisible())
		{
			if (firstObj)
			{
				gEngine.GetGraphicEngine()->SetTexture(gEngine.GetMeshManager()->GetTexture( mObjList[i].GetSpriteID() ) );
				gEngine.GetGraphicEngine()->SetVertices(gEngine.GetMeshManager()->GetMesh ( mObjList[i].GetMeshID() )->GetBuffer() );
				mObjList[i].SetRenderMode();
				firstObj = false;
			}
			mObjList[i].Render();
			++DrawCall;
		}		
	}
}

template <class T>
void ObjManager<T>::RenderDebugInfo( void )
{
	bool firstObj = true;
	size_t index = 0;

	for (size_t i = 0; i < mObjList.size(); ++i)
	{
		++index;	
		if (index > mObjectsInUse)
		{
			break;
		}

		if (!mObjList[i].IsAlive())
		{
			continue;
		}

		if (firstObj)
		{
			gEngine.GetGraphicEngine()->DisableTexture();
			gEngine.GetGraphicEngine()->SetBox();
			firstObj = false;
		}

		mObjList[i].RenderDebugInfo();
		++DrawCall;			
  }
}

template <class T>
void ObjManager<T>::Update( void )
{
	size_t index = 0;

	for (size_t i = 0; i < mObjList.size(); ++i)
	{
		++index;
		if (index > mObjectsInUse)
		{
			break;
		}

		if (!mObjList[i].IsAlive())
		{
			continue;
		}
				
		mObjList[i].Update();
		++UpdateCall;		
	}
}

template <class T>
void ObjManager<T>::Free( void )
{
	size_t index = 0;

	for (size_t i = 0; i < mObjList.size(); ++i)
	{
		++index;
		if (index > mObjectsInUse)
		{
			break;
		}

		if (!mObjList[i].IsAlive())
		{
			continue;
		}

		mObjList[i].SetDestroy();		
	}

	mObjectsInUse = 0;
}
#endif