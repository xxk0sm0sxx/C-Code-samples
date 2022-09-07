/**************************************************************************************************
* \file	    NXInterpolant.h
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
* \brief	It is use for interpolation\n
*			Copyright (C) 2012 DigiPen Institute of Technology. Reproduction
* 			or disclosure of this file or its contents without the prior written consent of DigiPen
* 			Institute of Technology is prohibited.
**************************************************************************************************/
#ifndef NXINTERPOLANT_H_
#define NXINTERPOLANT_H_

enum NXINTERPOLANT_BEHAVIOUR
{
	NXINTERPOLANT_LOOP,
	NXINTERPOLANT_ONCE,
	NXINTERPOLANT_CIRCULAR	
};

template <class T>
class NXInterpolant
{
	public:
		NXInterpolant();
		NXInterpolant(const T& start, const T& end, float Start_To_End_Seconds, NXINTERPOLANT_BEHAVIOUR behaviour);
		
		void Update(float dt);

		void Init(const T& start, const T& end, float Start_To_End_Seconds, NXINTERPOLANT_BEHAVIOUR behaviour);
		void SetEnd(const T& end);
		void SetRange(const T& start, const T& end);
		void SetSpeed(float Start_To_End_Seconds);
		void SetCurrentPercentage(float percentage);
		void SetBehaviour( NXINTERPOLANT_BEHAVIOUR behaviour );
		void SetPause( bool pause );

		void Reset();

		T GetValue( void );
		float GetPercentage( void );
		bool IsPaused( void );

	private:
		T mStart;
		T mEnd;
		T mDelta;
		T mInterp;
		T mValue;
		NXINTERPOLANT_BEHAVIOUR mBehaviour;
		float mDirection;
		float mRate;
		bool isPaused;
};

template <class T>
NXInterpolant<T>::NXInterpolant()
{
}

template <class T>
NXInterpolant<T>::NXInterpolant(const T& start, const T& end, float Start_To_End_Seconds, NXINTERPOLANT_BEHAVIOUR behaviour)
{
	Init(start, end, Start_To_End_Seconds, behaviour);
}

template <class T>
void NXInterpolant<T>::Init(const T& start, const T& end, float Start_To_End_Seconds, NXINTERPOLANT_BEHAVIOUR behaviour)
{
	mStart = start; mEnd = end;
	mDelta = mEnd - mStart;
	mRate = Start_To_End_Seconds;
	if (abs(mRate) > 0.0f)
	{
		mInterp = mDelta/mRate;
	}
	else
	{
		mInterp = mDelta;
	}
	mBehaviour = behaviour;
	mValue = mStart;
	mDirection = 1.0f;
	isPaused = false;
}

template <class T>
void NXInterpolant<T>::SetEnd(const T& end)
{
	T temp = mValue;
	SetRange(mStart, end);
	temp = mValue;
}

template <class T>
void NXInterpolant<T>::Update(float dt)
{
	if (isPaused)
	{
		return;
	}

	mValue += mInterp * mDirection * dt;
	if (mBehaviour == NXINTERPOLANT_LOOP)
	{
		if (mEnd > mStart)
		{
			if (mValue > mEnd)
			{
				T remain = mValue - mEnd;
				mValue = mStart + remain;
			}
		}
		else //if (mStart > mEnd)
		{
			if (mValue > mStart)
			{
				T remain = mValue - mStart;
				mValue = mEnd + remain;
			}
		}
	}
	if (mBehaviour == NXINTERPOLANT_CIRCULAR)
	{
		if (mEnd > mStart)
		{
			if (mValue > mEnd)
			{
				T remain = mValue - mEnd;
				mValue = mEnd - remain;
				mDirection = mDirection * -1.0f;
			}
			if (mValue < mStart)
			{
				T remain = mStart - mValue;
				mValue = mStart + remain;
				mDirection = mDirection * -1.0f;
			}
		}
		else //if (mEnd < mStart)
		{
			if (mValue > mStart)
			{
				T remain = mValue - mStart;
				mValue = mStart - remain;
				mDirection = mDirection * -1.0f;
			}
			if (mValue < mEnd)
			{
				T remain = mEnd - mValue;
				mValue = mEnd + remain;
				mDirection = mDirection * -1.0f;
			}
		}
	}
	if (mBehaviour == NXINTERPOLANT_ONCE)
	{
		if (mEnd > mStart)
		{
			if (mValue > mEnd)
			{
				mValue = mEnd;
				isPaused = true;
				//T remain = mValue - mEnd;
				//mValue = mStart + remain;
			}
		}
		else //if (mStart > mEnd)
		{
			if (mValue < mEnd)
			{
				mValue = mEnd;
				isPaused = true;
			}
		}
	}
}

template <class T>
void NXInterpolant<T>::SetSpeed(float Start_To_End_Seconds)
{
	mRate = Start_To_End_Seconds
	if (abs(mRate) > 0.0f)
	{
		mInterp = mStart/mRate;
	}
	else
	{
		mInterp = mStart;
	}
}

template <class T>
void NXInterpolant<T>::SetRange(const T& start, const T& end)
{
	mStart = start; mEnd = end;
	mDelta = mEnd - mStart;

	if (mRate > 0.0f)
	{
		mInterp = (mDelta)/mRate;
	}
	else
	{
		mInterp = mDelta;
	}
	mValue = start;
	mDirection = 1;
	isPaused = false;
}

template <class T>
void NXInterpolant<T>::SetCurrentPercentage(float percentage)
{
	mValue = mStart + ( mDelta * percentage );
}

template <class T>
void NXInterpolant<T>::SetBehaviour( NXINTERPOLANT_BEHAVIOUR behaviour )
{
	mBehaviour = behaviour;
}

template <class T>
void SetPause( bool pause )
{
	isPaused = pause;
}

template <class T>
T NXInterpolant<T>::GetValue( void )
{
	return mValue;
}

template <class T>
float NXInterpolant<T>::GetPercentage( void )
{
	if (abs(mDelta) > 0)
	{
		if (mEnd > mStart)
		{
			return (mValue - mStart) / mDelta;
		}
		else
		{
			return (mValue - mEnd) / mDelta;
		}
	}
	else
	{
		return 1.0f;
	}
}

template <class T>
bool NXInterpolant<T>::IsPaused( void )
{
	return isPaused;
}

template <class T>
void NXInterpolant<T>::Reset()
{
	mValue = mStart;
	mDirection = 1;
	isPaused = false;
}

#endif