/**************************************************************************************************
* \file	    NXGameObj.cpp
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
* \brief	A game object base class\n
*			Copyright (C) 2012 DigiPen Institute of Technology. Reproduction
* 			or disclosure of this file or its contents without the prior written consent of DigiPen
* 			Institute of Technology is prohibited.
**************************************************************************************************/
#include "NXGameObj.h"
#include "NXInputEngine.h"
#include "tinyxml.h"
#include "NXAssert.h"
#include "NXCamera.h"

/**************************************************************************************************
 * \fn	NXGameObj::NXGameObj( const std::wstring& meshID, const std::wstring& spriteID)
 *
 * \brief	Constructor.
 *
 * \param	meshID		Identifier for the mesh.
 * \param	spriteID	Identifier for the sprite.
**************************************************************************************************/

NXGameObj::NXGameObj(	const std::wstring& meshID,
										const std::wstring& spriteID) :
	mPos(Vec3(0.0f,0.0f,0.0f)), 
	mOffset(Vec3(0.0f,0.0f,0.0f)),
	mPitch(0.0f),
	mYaw(0.0f),
	mRoll(0.0f),
	mScale(Vec3(0.0f,0.0f,0.0f)),
	mLayer(0),
	mParallaxScale(0),
	mMeshID(meshID),
	mSpriteID(spriteID),
	mVel(0.0f, 0.0f, 0.0f),
	mRollPrev(-1),
	mYawPrev(-1),
	mPitchPrev(-1),
	flippedScalePrev(-1,-1,-1),
	posPrev(-1,-1,-1),
	mRollAABBPrev(-1),
	mYawAABBPrev(-1),
	mPitchAABBPrev(-1),
	flippedScaleAABBPrev(-1,-1,-1),
	posAABBPrev(-1,-1,-1),
	mFollowedObj(0),
	flag(0),
	mCurrentAnimation(L""),
	mCurrentCellNo(0),
	mMaxCellNo(0),
	mCurrentAnimationTime(0),
	mMaxAnimationTime(0),
	isAnimationLooping(1),
	isAnimationPaused(0),
	isAnimationChanged(1),
	isAnimationMatrixChanged(1),
	isHorizontalFlip(0),
	isVerticalFlip(0),
	isColorModulating(0),
	isZWriting(1),
	isAlive(0),
	isVisible(1),	
	isDrawingDebugInfo(0),
	isAdditiveBlend(0),
	mLifetimeM(-1.0f),
	mLifetime(-1.0f)
{
	mAABB.c = mPos;
	mAABB.r_min_percent = Vec3(-1,-1,-1);
	mAABB.r_max_percent = Vec3(1,1,1);
}

/**************************************************************************************************
 * \fn	NXGameObj::~NXGameObj( void )
 *
 * \brief	Destructor.
**************************************************************************************************/

NXGameObj::~NXGameObj( void )
{
}

/**************************************************************************************************
 * \fn	void NXGameObj::Init( void )
 *
 * \brief	Initialises object.
**************************************************************************************************/

void NXGameObj::Init( void )
{
	mPos = Vec3(0.0f,0.0f,0.0f);
	mOffset = Vec3(0.0f,0.0f,0.0f);
	mPitch = 0.0f;
	mYaw = 0.0f;
	mRoll = 0.0f;
	mScale = Vec3(0.0f,0.0f,0.0f);
	mLayer = 0;
	mParallaxScale = 0;
	isAlive = 0;
	mVel = Vec3(0.0f, 0.0f, 0.0f);
	mFollowedObj = 0;
	flag = 0;
	mCurrentAnimation = L"";
	mCurrentCellNo = 0;
	isAnimationChanged = 1;
	isAnimationMatrixChanged = 1;
	mMaxCellNo = 0;
	mCurrentAnimationTime = 0;
	mMaxAnimationTime = 0;
	isAnimationLooping = 1;
	isAnimationPaused = 0;
	isHorizontalFlip = 0;
	isVerticalFlip = 0;
	isVisible = 1;
	isDrawingDebugInfo = 0;
	isColorModulating = 0;
	mLifetimeM = -1.0f;
	mLifetime = -1.0;
	mAABB.c = mPos;
	mAABB.r_min_percent = Vec3(-1,-1,-1);
	mAABB.r_max_percent = Vec3(1,1,1);
	isZWriting = 1;
	isAdditiveBlend = 0;
}

void NXGameObj::Destroy( void )
{
	if (mCurrentAnimation != L"")
		SetCurrentAnimation(L"",0);
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetAlive ( void )
 *
 * \brief	Sets object to alive.
**************************************************************************************************/

void NXGameObj::SetAlive ( void )
{
	isAlive = true;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetDestroy ( void )
 *
 * \brief	Sets object to destroy.
**************************************************************************************************/

void NXGameObj::SetDestroy ( void )
{
	isAlive = false;
	Destroy();
}

/**************************************************************************************************
 * \fn	void NXGameObj::Update()
 *
 * \brief	Updates object.
**************************************************************************************************/

void NXGameObj::Update()
{
	if (mFollowedObj != 0)
	{
		Vec3 pos = mFollowedObj->GetPosition();
		SetPosition(pos + mFollowOffset);
	}
	else
	{
		mPos += mVel * (float)gEngine.NXGetDeltaTime();
	}

	UpdateAABB();

	if (mLifetimeM > 0)
	{
		mLifetime -= (float)gEngine.NXGetDeltaTime();
		if (mLifetime < 0)
		{
			SetDestroy();
		}
	}
	
	UpdateAnimation();
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetPosition(const Vec3& pos)
 *
 * \brief	Sets obj position.
 *
 * \param	pos	The position.
**************************************************************************************************/

void NXGameObj::SetPosition(const Vec3& pos)
{
	mPos = pos;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetPitch(float pitch)
 *
 * \brief	Sets obj pitch.
 *
 * \param	pitch	The pitch.
**************************************************************************************************/

void NXGameObj::SetPitch(float pitch)
{
	mPitch = pitch;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetYaw(float yaw)
 *
 * \brief	Sets obj yaw.
 *
 * \param	yaw	The yaw.
**************************************************************************************************/

void NXGameObj::SetYaw(float yaw)
{
	mYaw = yaw;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetRoll(float roll)
 *
 * \brief	Sets obj roll.
 *
 * \param	roll	The roll.
**************************************************************************************************/

void NXGameObj::SetRoll(float roll)
{
	mRoll = roll;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetScale(const Vec3& scale)
 *
 * \brief	Sets obj scale.
 *
 * \param	scale	The scale.
**************************************************************************************************/

void NXGameObj::SetScale(const Vec3& scale)
{
	mScale  = scale;
	SetAABBPercentage( mAABB.r_min_percent.x, 
											mAABB.r_max_percent.x, 
											mAABB.r_min_percent.y,
											mAABB.r_max_percent.y, 
											mAABB.r_min_percent.z, 
											mAABB.r_max_percent.z );
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetOneToOneScale(float screen_width_percentage)
 *
 * \brief	Sets an one to one scale.
 *
 * \param	screen_width_percentage	The screen width percentage.
**************************************************************************************************/

void NXGameObj::SetOneToOneScale(float screen_width_percentage)
{
	Vec3 scale = GetScale();
	scale.x = gEngine.GetFovX() * screen_width_percentage;
	NXTexture* texture = gEngine.GetMeshManager()->GetTexture(GetSpriteID());
	
	
	D3DSURFACE_DESC surface;
	(*texture)->GetLevelDesc(0,&surface);
	
	NXAnimation* animation = gEngine.GetMeshManager()->GetAnimation(mSpriteID);

	float texturescale = mScale.x * animation->GetColumns() /surface.Width;
	scale.y = surface.Height * texturescale;
	SetScale(scale);
}

//void NXGameObj::SetAABB(const Vec3& pos, const Vec3& scale)
//{
//	float minPercentX, minPercentY, minPercentZ;
//	float maxPercentX, maxPercentY, maxPercentZ;
//
//	minPercentX = (pos.x - mPos.x) / scale.x
//
//	SetAABBPercentage(minPercentX, minPercentY, minPercentZ, 
//										maxPercentX, maxPercentY, maxPercentZ);
//}

/**************************************************************************************************
 * \fn	void NXGameObj::SetAABBPercentage(float xMin, float xMax, float yMin, float yMax,
 * 			float zMin, float zMax)
 *
 * \brief	Sets AABB percentage.
 *
 * \param	xMin	The minimum.
 * \param	xMax	The maximum.
 * \param	yMin	The minimum.
 * \param	yMax	The maximum.
 * \param	zMin	The minimum.
 * \param	zMax	The maximum.
**************************************************************************************************/

void NXGameObj::SetAABBPercentage(float xMin, float xMax, 
								  float yMin, float yMax,
							      float zMin, float zMax)
{
	mAABB.r_min_percent.x = xMin;
	mAABB.r_min_percent.y = yMin;
	mAABB.r_min_percent.z = zMin;
	mAABB.r_max_percent.x = xMax;
	mAABB.r_max_percent.y = yMax;
	mAABB.r_max_percent.z = zMax;
	
	mAABB.r_min.x = (xMin + ((xMax - xMin)*0.5f)) * mScale.x * 0.5f;
	mAABB.r_min.y = (yMin + ((yMax - yMin)*0.5f)) * mScale.y * 0.5f;
	mAABB.r_min.z = (zMin + ((zMax - zMin)*0.5f)) * mScale.z * 0.5f;
	mAABB.r.x = (xMax - xMin) * mScale.x * 0.25f;
	mAABB.r.y = (yMax - yMin) * mScale.y * 0.25f;
	mAABB.r.z = (zMax - zMin) * mScale.z * 0.25f;
	UpdateAABB();
}

/**************************************************************************************************
 * \fn	void NXGameObj::UpdateAABB( void )
 *
 * \brief	Updates AABB.
**************************************************************************************************/

void NXGameObj::UpdateAABB( void )
{
	mAABB.c.x = mPos.x + mAABB.r_min.x;
	mAABB.c.y = mPos.y + mAABB.r_min.y;
	mAABB.c.z = mPos.z + mAABB.r_min.z;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetVelocity(const Vec3& velocity)
 *
 * \brief	Sets velocity.
 *
 * \param	velocity	The velocity.
**************************************************************************************************/

void NXGameObj::SetVelocity(const Vec3& velocity)
{
	mVel = velocity;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetZRenderingLayer (int layer)
 *
 * \brief	Sets a z coordinate rendering layer.
 *
 * \param	layer	The layer.
**************************************************************************************************/

void NXGameObj::SetZRenderingLayer (int layer)
{
	mLayer = layer;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetParallaxScale (float scale)
 *
 * \brief	Sets parallax layer amount.
 *
 * \param	scale	The parallax amount.
**************************************************************************************************/

void NXGameObj::SetParallaxScale (float scale)
{
	mParallaxScale = scale;
}

/**************************************************************************************************
 * \fn	D3DXMATRIXA16 NXGameObj::CreateTransformMatrix( void ) const
 *
 * \brief	Creates the transform matrix.
 *
 * \return	The new transform matrix.
**************************************************************************************************/

D3DXMATRIXA16 NXGameObj::CreateTransformMatrix( void )
{
	flippedScale = mScale;
	if (isHorizontalFlip)
	{
		flippedScale.x *= -1;
	}
	if (isVerticalFlip)
	{
		flippedScale.y *= -1;
	}

	//Ortho transformation
	Vec3 pos(mPos.x+gCamera.GetPosition().x*mParallaxScale, mPos.y + mPos.z/2.0f, mPos.z/100.0f - mLayer);
	//Vec3 pos(mPos.x, mPos.y + mPos.z/2.0f, -1.0f);
	//Vec3 pos(mPos.x, mPos.y, mPos.z);

	/*D3DXMATRIX transform_mtx;
	D3DXMATRIX translate_mtx;
	D3DXMATRIX rotate_mtx;
	D3DXMATRIX scale_mtx;

	D3DXMatrixIdentity(&transform_mtx);
	D3DXMatrixTranslation(&translate_mtx, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&rotate_mtx, mYaw/180.0f, mPitch/180.0f, mRoll/180.0f);
	D3DXMatrixScaling(&scale_mtx, flippedScale.x, flippedScale.y, flippedScale.z);

	transform_mtx = transform_mtx * scale_mtx;
	transform_mtx = transform_mtx * rotate_mtx;
	transform_mtx = transform_mtx * translate_mtx;*/

	bool changed = false;
	if (mRoll != mRollPrev || mYaw != mYawPrev || mPitch != mPitchPrev)
	{
		changed = true;
		Mtx33RotYawRow(finalRotation,mRoll,mYaw,mPitch);
		mRollPrev = mRoll;
		mYawPrev = mYaw;
		mPitchPrev = mPitch;
	}

	if (flippedScale != flippedScalePrev)
	{
		changed = true;
		Mtx33Scale(pScale, flippedScale.x, flippedScale.y, flippedScale.z);
		flippedScalePrev = flippedScale;
	}

	if (changed)
	{
		tempSR = finalRotation * pScale;
		Mtx33Transpose(tempSR, tempSR);
	}

	if (changed || pos != posPrev)
	{
		tempMatrix = D3DXMATRIX(	tempSR.m00,	tempSR.m01,	tempSR.m02,	0.0f,
														tempSR.m10,	tempSR.m11,	tempSR.m12,	0.0f,
														tempSR.m20,	tempSR.m21,	tempSR.m22,	0.0f,
														pos.x,		pos.y,		pos.z,		1.0f);
		posPrev = pos;
	}

	return tempMatrix;
}

/**************************************************************************************************
 * \fn	D3DXMATRIXA16 NXGameObj::CreateCollisionMatrix( void ) const
 *
 * \brief	Creates the collision matrix.
 *
 * \return	The new collision matrix.
**************************************************************************************************/

D3DXMATRIXA16 NXGameObj::CreateCollisionMatrix( void )
{
	flippedScaleAABB = Vec3(mAABB.r.x*2,mAABB.r.y*2,0);
	if (isHorizontalFlip)
	{
		flippedScaleAABB.x *= -1;
	}
	if (isVerticalFlip)
	{
		flippedScaleAABB.y *= -1;
	}

	//Ortho transformation
	Vec3 posAABB(mAABB.c.x+gCamera.GetPosition().x*mParallaxScale, mAABB.c.y + mAABB.c.z/2.0f, mPos.z/100.0f - mLayer);
	//Vec3 pos(mPos.x, mPos.y + mPos.z/2.0f, -1.0f);
	//Vec3 pos(mPos.x, mPos.y, mPos.z);

	/*D3DXMATRIX transform_mtx;
	D3DXMATRIX translate_mtx;
	D3DXMATRIX rotate_mtx;
	D3DXMATRIX scale_mtx;

	D3DXMatrixIdentity(&transform_mtx);
	D3DXMatrixTranslation(&translate_mtx, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&rotate_mtx, mYaw/180.0f, mPitch/180.0f, mRoll/180.0f);
	D3DXMatrixScaling(&scale_mtx, flippedScale.x, flippedScale.y, flippedScale.z);

	transform_mtx = transform_mtx * scale_mtx;
	transform_mtx = transform_mtx * rotate_mtx;
	transform_mtx = transform_mtx * translate_mtx;*/

	bool changed = false;

	if (mRoll != mRollPrev || mYaw != mYawPrev || mPitch != mPitchPrev)
	{
		changed = true;
	}

	if (mRoll != mRollAABBPrev || mYaw != mYawAABBPrev || mPitch != mPitchAABBPrev)
	{
		changed = true;
		mRollAABBPrev = mRoll;
		mYawAABBPrev = mYaw;
		mPitchAABBPrev = mPitch;
	}

	if (flippedScaleAABB != flippedScaleAABBPrev)
	{
		changed = true;
		Mtx33Scale(pScaleAABB, flippedScaleAABB.x, flippedScaleAABB.y, flippedScaleAABB.z);
		flippedScaleAABBPrev = flippedScaleAABB;
	}

	if (changed)
	{
		tempSRAABB = finalRotation * pScaleAABB;
		Mtx33Transpose(tempSRAABB, tempSRAABB);
	}

	if (changed || posAABB != posAABBPrev)
	{
		tempMatrixAABB = D3DXMATRIX(	tempSRAABB.m00,	tempSRAABB.m01,	tempSRAABB.m02,	0.0f,
														tempSRAABB.m10,	tempSRAABB.m11,	tempSRAABB.m12,	0.0f,
														tempSRAABB.m20,	tempSRAABB.m21,	tempSRAABB.m22,	0.0f,
														posAABB.x,		posAABB.y,		posAABB.z,		1.0f);
		posAABBPrev = posAABB;
	}

	return tempMatrixAABB;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetDefaultAnimation( void )
 *
 * \brief	Sets the default animation.
**************************************************************************************************/

void NXGameObj::SetDefaultAnimation( void )
{
	NXAnimation* animation = gEngine.GetMeshManager()->GetAnimation(mSpriteID);
	std::wstring ID = animation->GetAnimationList().begin()->first;
	if(ID != mCurrentAnimation)
	{
		mCurrentAnimation = ID;
		NX_ASSERT(animation);
		if (animation == 0)
		{
			return;
		}
		mMaxCellNo = animation->GetNumberOfFrames(ID);
		mCurrentCellNo = 0;
	}
	
	mMaxAnimationTime = 0.1f;
	PauseAnimation(false);
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetCurrentAnimation(const std::wstring& ID, const float& animationSpeed,
 * 			bool animationLoop)
 *
 * \brief	Sets an animation.
 *
 * \param	ID							The identifier.
 * \param	animationSpeed	The animation speed.
 * \param	animationLoop 	true to loop animation.
**************************************************************************************************/

void NXGameObj::SetCurrentAnimation(const std::wstring& ID, const float& animationSpeed, bool animationLoop)
{
	isAnimationLooping = animationLoop;

	if (ID == L"")
	{
		mCurrentAnimation = L"";
		mMaxAnimationTime = 0; 
		PauseAnimation(true);
		return;
	}

	if(ID != mCurrentAnimation)
	{
		mCurrentAnimation = ID;
		NXAnimation* animation = gEngine.GetMeshManager()->GetAnimation(mSpriteID);
		NX_ASSERT(animation);
		if (animation == 0)
		{
			return;
		}
		mMaxCellNo = animation->GetNumberOfFrames(ID);
		mCurrentCellNo = 0;
	}
	
	isAnimationChanged = true;
	isAnimationMatrixChanged = true;
	mMaxAnimationTime = animationSpeed;
	PauseAnimation(false);
}

/**************************************************************************************************
 * \fn	void NXGameObj::PauseAnimation(bool setPause)
 *
 * \brief	Pause animation.
 *
 * \param	setPause	true to pause animation.
**************************************************************************************************/

void NXGameObj::PauseAnimation(bool setPause)
{
	isAnimationPaused = setPause;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetColorModulation(int a, int r, int g, int b)
 *
 * \brief	Sets a color modulation.
 *
 * \param	a	Alpha of color.
 * \param	r	Red of color.
 * \param	g	Green of color.
 * \param	b	Blue of color.
**************************************************************************************************/

void NXGameObj::SetColorModulation(int a, int r, int g, int b)
{ 
	colorModulate = NXCOLOR_ARGB(a, r,g,b); 
	isColorModulating = true; 
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetAnimationHorizontalFlip(bool setFlip)
 *
 * \brief	Sets animation horizontal flip.
 *
 * \param	setFlip	true to flip.
**************************************************************************************************/

void NXGameObj::SetAnimationHorizontalFlip(bool setFlip)
{
	isHorizontalFlip = setFlip;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetAnimationVerticalFlip(bool setFlip)
 *
 * \brief	Sets animation vertical flip.
 *
 * \param	setFlip	true to flip.
**************************************************************************************************/

void NXGameObj::SetAnimationVerticalFlip(bool setFlip)
{
	isVerticalFlip = setFlip;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetMeshID(const std::wstring& ID)
 *
 * \brief	Sets mesh of object.
 *
 * \param	ID	The identifier.
**************************************************************************************************/

void NXGameObj::SetMeshID(const std::wstring& ID)
{
	mMeshID = ID;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetSpriteID(const std::wstring& ID)
 *
 * \brief	Sets sprite of object.
 *
 * \param	ID	The identifier.
**************************************************************************************************/

void NXGameObj::SetSpriteID(const std::wstring& ID)
{
	mSpriteID = ID;
	isAnimationChanged = true;
	isAnimationMatrixChanged = true;
	SetAnimationTransformation();
}

void NXGameObj::SetEnableAdditiveBlend(bool enable)
{
	isAdditiveBlend = enable;
}

Vec3 NXGameObj::GetOrthoPosition( void ) const
{
	return Vec3(mPos.x, mPos.y + mPos.z/2.0f, mPos.z/100.0f - mLayer);
}

/**************************************************************************************************
 * \fn	void NXGameObj::Render()
 *
 * \brief	Renders this object.
**************************************************************************************************/

void NXGameObj::SetRenderMode( void )
{
	if (!isColorModulating)
	{
		gEngine.GetGraphicEngine()->SetColorBlending(NXCOLOR_ARGB(255,255,255,255));
	}
	if (isAdditiveBlend)
	{
		gEngine.GetGraphicEngine()->SetAdditiveBlending();
	}
	else
	{
		gEngine.GetGraphicEngine()->SetNormalBlending();
	}
	if (!isZWriting)
	{
		gEngine.GetGraphicEngine()->DisableZChecking();
	}
}
void NXGameObj::Render()
{
	if (isColorModulating)
	{
		gEngine.GetGraphicEngine()->SetColorBlending(colorModulate);
	}
	SetAnimationTransformation();
	gEngine.GetGraphicEngine()->SetObjectTransform(CreateTransformMatrix());
	gEngine.GetGraphicEngine()->DrawTriangleList(2);
}

/**************************************************************************************************
 * \fn	void NXGameObj::UpdateAnimation()
 *
 * \brief	Updates the animation.
**************************************************************************************************/

void NXGameObj::UpdateAnimation()
{
	if (!isAnimationPaused)
	{
		mCurrentAnimationTime += (float)gEngine.NXGetDeltaTime();
		if (mCurrentAnimationTime >= mMaxAnimationTime)
		{
			mCurrentAnimationTime = 0;
			++mCurrentCellNo;
			isAnimationChanged = true;
			isAnimationMatrixChanged = true;
			if (mCurrentCellNo >= mMaxCellNo)
			{
				if (isAnimationLooping)
				{
					mCurrentCellNo = 0;
				}
				else
				{
					mCurrentCellNo = mMaxCellNo-1;
					isAnimationPaused = true;
				}
			}
		}
		else
		{
			isAnimationChanged = false;
		}
	}
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetAnimationTransformation( void )
 *
 * \brief	Sets animation transformation.
**************************************************************************************************/

void NXGameObj::SetAnimationTransformation( void )
{
	if (!IsAnimationMatrixChanged())
	{
		gEngine.GetGraphicEngine()->SetTextureTransform( mTextureTransform );
		return;
	}

	isAnimationMatrixChanged = false;
	if (mSpriteID == L"")
	{
		return;
	}

	NXAnimationCell cell;
	NXAnimation* animation = gEngine.GetMeshManager()->GetAnimation(mSpriteID);
	NX_ASSERT(animation);
	if (animation == 0)
	{
		return;
	}

	if (mCurrentAnimation != L"")
	{
		cell = animation->GetAnimationCell(mCurrentAnimation, 
			mCurrentCellNo);
	}
	else
	{
		cell = animation->GetDefaultAnimationCell();
	}
		D3DXMatrixScaling( &mTextureTransform, 
			1.0f/animation->GetColumns(),
			1.0f/animation->GetRows(), 
			1.0f );

		mTextureTransform._31 = cell.startX;
		mTextureTransform._32 = cell.startY;

		gEngine.GetGraphicEngine()->SetTextureTransform( mTextureTransform );
	/*}
	else
	{
		D3DXMatrixIdentity(&mTextureTransform);
		gEngine.GetGraphicEngine()->SetTextureTransform( mTextureTransform );
	}*/
}

/**************************************************************************************************
 * \fn	bool NXGameObj::IsVisible( void ) const
 *
 * \brief	Query if this object is visible.
 *
 * \return	true if visible, false if not.
**************************************************************************************************/

bool NXGameObj::IsVisible( void ) const 
{ 
	if (!isVisible)
		return false;

	if ( (abs(mPos.x+gCamera.GetPosition().x*mParallaxScale - gCamera.GetPosition().x) + mScale.x) > (gEngine.GetFovX()*2) )
	{
		return false;
	}

	return true;
}

/**************************************************************************************************
 * \fn	void NXGameObj::RenderDebugInfo()
 *
 * \brief	Renders the debug information.
**************************************************************************************************/

void NXGameObj::RenderDebugInfo()
{
	gEngine.GetGraphicEngine()->SetObjectTransform(CreateCollisionMatrix());
	gEngine.GetGraphicEngine()->DrawLineStrip(4);
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetVisible(bool setVisible)
 *
 * \brief	Sets object visible.
 *
 * \param	setVisible	true to show, false to hide the set.
**************************************************************************************************/

void NXGameObj::SetVisible(bool setVisible)
{
	isVisible = setVisible;
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetDrawDebugInfo(bool setDraw)
 *
 * \brief	Sets object draw debug information.
 *
 * \param	setDraw	true to set draw.
**************************************************************************************************/

void NXGameObj::SetDrawDebugInfo(bool setDraw)
{
	isDrawingDebugInfo = setDraw;
}

void NXGameObj::AddVel(const Vec3& vel)
{
	mVel += vel;
}

/**************************************************************************************************
 * \fn	void NXGameObj::AddForce(const Vec3& force)
 *
 * \brief	Adds a force.
 *
 * \param	force	The force.
**************************************************************************************************/

void NXGameObj::AddForce(const Vec3& force)
{
	mForceList.push_back(force);
}

/**************************************************************************************************
 * \fn	void NXGameObj::AddForce(const Force& force)
 *
 * \brief	Adds a force.
 *
 * \param	force	The force.
**************************************************************************************************/

void NXGameObj::UpdatePhysics(float dt)
{
	physics_fixed_dt = 1/60.0f;

	//Add dt into physics dt
	physics_dt += dt;

	//If physics dt exceed fixed dt, update physics simulation
	while (physics_dt >physics_fixed_dt)
	{
		for (size_t i = 0; i < mForceList.size(); ++i)
		{
			mVel += mForceList[i];
		}

		mForceList.clear();

		physics_dt -= physics_fixed_dt;
	}
}

/**************************************************************************************************
 * \fn	void NXGameObj::SetLifetime(const float& lifetime)
 *
 * \brief	Sets object lifetime.
 *
 * \param	lifetime	The lifetime.
**************************************************************************************************/

void NXGameObj::SetLifetime(const float& lifetime)
{
	mLifetimeM = mLifetime = lifetime;
}

/**************************************************************************************************
 * \fn	void NXGameObj::ResetLifetime( void )
 *
 * \brief	Resets the lifetime.
**************************************************************************************************/

void NXGameObj::ResetLifetime( void )
{
	mLifetime = mLifetimeM;
}

/**************************************************************************************************
 * \fn	void ShadowObj::Follow(GameObj& obj)
 *
 * \brief	Follows.
 *
 * \param [in,out]	obj	The object.
**************************************************************************************************/

void NXGameObj::SetFollow(NXGameObj& obj, float offsetX, float offsetY, float offsetZ)
{
	mFollowedObj = &obj;
	mFollowOffset = Vec3(offsetX, offsetY, offsetZ);
}