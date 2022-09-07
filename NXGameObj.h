/**************************************************************************************************
* \file	    NXGameObj.h
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
#ifndef NXGAMEOBJ_H_
#define NXGAMEOBJ_H_

#include "NXEngineMain.h"
#include "NXGraphicEngine.h"
#include "NXMaths.h"
#include "NXCollision.h"
#include "NXAnimation.h"
#include "NXPhysics.h"
#include "NXInterpolant.h"
#include <vector>

typedef	std::vector<Vec3>	ForceList;

class NXGameObj
{
	public:
		NXGameObj(	const std::wstring& meshID = L"",
							const std::wstring& spriteID = L"");

		virtual ~NXGameObj( void );	
		
		virtual void Init( void );
		virtual void Destroy( void );
		virtual void Update();    //float g_dt = 0){};
		virtual void RenderDebugInfo( void );

		void SetAlive ( void );
		void SetDestroy ( void );

		//------Settors------//
		void SetPosition(const Vec3& pos);

		void SetPitch(float pitch);
		void SetYaw  (float yaw);
		void SetRoll (float roll);

		void SetScale   (const Vec3& scale);
		void SetOneToOneScale(float screen_width_percentage); //Maintain aspect ratio of texture

		/*void SetAABB    (const Vec3& pos, const Vec3& scale);*/
		void SetAABBPercentage(float xMin = -1.0f, float xMax = 1.0f, 
													float yMin = -1.0f, float yMax = 1.0f,
													float zMin = -1.0f, float zMax = 1.0f);
		void UpdateAABB( void );

		void SetVelocity(const Vec3& velocity);

		void SetZRenderingLayer (int layer);

		void SetParallaxScale (float scale);

		D3DXMATRIXA16 CreateTransformMatrix( void );
		D3DXMATRIXA16 CreateCollisionMatrix( void );

		//------Animation------//
		void SetDefaultAnimation( void );
		void SetCurrentAnimation(const std::wstring& ID, const float& animationSpeed, bool animationLoop = true);
		void SetAnimationFrame(unsigned num) { mCurrentCellNo = num; isAnimationMatrixChanged = true;}
		void SetAnimationHorizontalFlip(bool setFlip);
		void SetAnimationVerticalFlip  (bool setFlip);
		bool IsAnimationChanged(void) const { return isAnimationChanged; }
		bool IsAnimationMatrixChanged(void) const { return isAnimationMatrixChanged; }
		void PauseAnimation(bool setPause);

		void SetColorModulation(int a = 255, int r = 255, int g = 255, int b = 255);
		NXCOLOR GetColorModulation( void ) const { return colorModulate; }
		void SetEnableColorModulation(bool enable) { isColorModulating = enable; }
		bool IsColorModulating( void ) { return isColorModulating; }
		
		void SetZWriting( bool enable ) { isZWriting = enable; }
		bool IsZWriting( void ) { return isZWriting; }

		//-------Mesh--------//
		void SetMeshID   (const std::wstring& ID);
		void SetSpriteID (const std::wstring& ID);

		void SetEnableAdditiveBlend( bool enable );

		void SetRenderMode( void );
		void Render( void );
			
		void SetVisible(bool setVisible);
		void SetDrawDebugInfo(bool setDraw);

		void AddVel(const Vec3& vel);

		void AddForce(const Vec3& force);

		void UpdatePhysics(float dt);

		void SetLifetime ( const float& lifetime );
		void ResetLifetime( void );

		void SetFollow(NXGameObj& obj, float offsetX = 0, float offsetY = 0, float offsetZ = 0);

		//-----Gettors------//
		bool IsAlive( void ) const { return isAlive; }

		Vec3 GetPosition( void ) const { return mPos; }
		Vec3 GetOrthoPosition( void ) const; 
		//Vec3 GetCenter( void ) const { return mCenter; }
		
		bool IsAdditiveBlend( void ) const { return isAdditiveBlend; }
		
		float GetPitch( void ) const { return mPitch; }
		float GetYaw( void ) const { return mYaw; }
		float GetRoll( void ) const { return mRoll; }
		
		Vec3 GetVelocity( void ) const { return mVel; }
		Vec3 GetScale( void ) const { return mScale; }
		AABB GetAABB( void ) const { return mAABB; }

		float GetLifetimeRemaining( void ) const { return mLifetime; }
		float GetLifetimeStarting( void ) const { return mLifetimeM; }
		float GetLifetimePercentage( void ) const { return mLifetime / mLifetimeM; }

		int GetZLayer( void ) const { return mLayer; }
		float GetParallaxScale( void ) const { return mParallaxScale; }

		bool IsVisible( void ) const;
		bool IsDrawingDebugInfo( void ) const { return isDrawingDebugInfo; }

		bool IsAnimationVerticalFlip ( void ) const { return isVerticalFlip; }
		bool IsAnimationHorizontalFlip ( void ) const { return isHorizontalFlip; }

		unsigned GetCurrentNumberOfFrames( void ) const { return mMaxCellNo; }
		unsigned GetCurrentFrameNumber( void ) const { return mCurrentCellNo; }
		
		bool IsAnimationFinished( void ) { return mCurrentCellNo == mMaxCellNo - 1;	}
		bool IsCurrentAnimation(const std::wstring& animation ){ return mCurrentAnimation == animation; }

		//Animation is within 0-100% finished, normalized to 0-1 (0.75f = 75%)
		bool IsAnimationPercentageFinished( float percent ) { return mCurrentCellNo >= (mMaxCellNo*percent); }

		std::wstring GetMeshID ( void ) const { return mMeshID; }
		std::wstring GetSpriteID ( void ) const { return mSpriteID; }
		std::wstring GetCurrentAnimation ( void ) const { return mCurrentAnimation; }

		unsigned long flag;
	
	protected:
		Vec3 mPos;
		Vec3 mOffset;
		float mPitch;
		float mYaw;
		float mRoll;
		Vec3 mScale;
		Vec3 flippedScale;
		Vec3 flippedScaleAABB;
		Vec3 mVel;
		Vec3 mEndForce;
		//Vec3 mCenter;
		AABB mAABB;
		int mLayer;
		float mParallaxScale;

		float mRollPrev;
		float mYawPrev;
		float mPitchPrev;
		Vec3 flippedScalePrev;
		Vec3 posPrev;
		Matrix3x3 pScale,finalRotation,tempSR;
		D3DXMATRIX tempMatrix;

		float mRollAABBPrev;
		float mYawAABBPrev;
		float mPitchAABBPrev;
		Vec3 flippedScaleAABBPrev;
		Vec3 posAABBPrev;
		Matrix3x3 pScaleAABB,tempSRAABB;
		D3DXMATRIX tempMatrixAABB;
		D3DXMATRIX mTextureTransform;

		NXGameObj* mFollowedObj;
		Vec3 mFollowOffset;

		bool isAlive;		

		std::wstring mMeshID;
		std::wstring mSpriteID;
			
		ForceList	mForceList;

		std::wstring mCurrentAnimation;
		bool isColorModulating;
		NXCOLOR colorModulate;
		bool isZWriting;
		bool isAdditiveBlend;


		unsigned mCurrentCellNo;
		unsigned mMaxCellNo;
		float mCurrentAnimationTime;
		float mMaxAnimationTime;
		bool isAnimationLooping;
		bool isAnimationPaused;
		bool isHorizontalFlip;
		bool isVerticalFlip;
		bool isAnimationChanged;
		bool isAnimationMatrixChanged;

		bool isVisible;
		bool isDrawingDebugInfo;

		float mLifetimeM;
		float mLifetime;

		/*NXInterpolant<float> testInt1;
		NXInterpolant<float> testInt2;
		NXInterpolant<float> testInt3;*/
	private:
		void UpdateAnimation( void );
		void SetAnimationTransformation( void );

		//Physics stuff
		float physics_dt;
		float physics_fixed_dt;
};

#endif
