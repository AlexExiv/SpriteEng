#ifndef __FVECTOR2D_H__
#define __FVECTOR2D_H__


#include "..\types.h"
#include <math.h>


template<class T>
class FVector2
{
public:
	union
	{
		struct
		{
			T x;
			T y;
		};
		struct
		{
			T fWidth;
			T fHeight;
		};
	};

	inline FVector2() : x( 0 ), y( 0 ) {}
	inline FVector2( T x, T y ) : x( x ), y( y ) {}
	inline FVector2( const FVector2<T> & vVec ) : x( vVec.x ), y( vVec.y ) {}
	~FVector2() {}

	inline FVector2<T> & operator = ( const FVector2<T> & vVec )
	{
		x = vVec.x;
		y = vVec.y;

		return * this;
	}

	inline FVector2<T> & operator += ( const FVector2<T> & vVec )
	{
		x += vVec.x;
		y += vVec.y;

		return * this;
	}

	inline FVector2<T> & operator -= ( const FVector2<T> & vVec )
	{
		x -= vVec.x;
		y -= vVec.y;

		return * this;
	}

	inline FVector2<T> operator + ()const
	{
		return *this;
	}

	inline FVector2<T> operator - ()const
	{
		return FVector2<T>( -x, -y );
	}

	inline FVector2<T> & operator *= ( const FVector2<T> & vVec )
	{
		x *= vVec.x;
		y *= vVec.y;

		return * this;
	}

	inline FVector2<T> & operator /= ( const FVector2<T> & vVec )
	{
		x /= vVec.x;
		y /= vVec.y;

		return * this;
	}

	inline FVector2<T> & operator *= ( const T tVal )
	{
		x *= tVal;
		y *= tVal;

		return * this;
	}

	inline FVector2<T> & operator /= ( const T tVal )
	{
		x /= tVal;
		y /= tVal;

		return * this;
	}

	inline T Length()const
	{
		return sqrt( x*x + y*y );
	}

	inline T LengthSq()const
	{
		return x*x + y*y;
	}

	inline void Normalize()
	{
		T tLen = Length();

		x /= tLen;
		y /= tLen;
	}

	inline T GetMaxComp()const
	{
		return max( fabs( x ), fabs( y ) );
	}

	inline T Distance( const FVector2<T> & vVec )const
	{
		return sqrt( (x - vVec.x)*(x - vVec.x) + (y - vVec.y)*(y - vVec.y) );
	}

	inline T DistanceSq( const FVector2<T> & vVec )const
	{
		return ( (x - vVec.x)*(x - vVec.x) + (y - vVec.y)*(y - vVec.y) );
	}

	inline T operator [] ( UI32 iIndex )
	{
		return *( &x + iIndex );
	}

	inline T operator [] ( UI32 iIndex ) const
	{
		return *( &x + iIndex );
	}

	inline operator T * ()
	{
		return &x;
	}

	inline operator const T * () const
	{
		return &x;
	}

	inline bool operator == ( const FVector2<T> & vVec )const
	{
		return (x == vVec.x) && (y == vVec.y);
	}

	inline bool operator != ( const FVector2<T> & vVec )const
	{
		return (x != vVec.x) && (y != vVec.y);
	}
};


template<class T>
inline FVector2<T> operator + ( const FVector2<T> & vVec0, const FVector2<T> & vVec1 )
{
	return FVector2<T>( vVec0.x + vVec1.x, vVec0.y + vVec1.y ); 
}

template<class T>
inline FVector2<T> operator - ( const FVector2<T> & vVec0, const FVector2<T> & vVec1 )
{
	return FVector2<T>( vVec0.x - vVec1.x, vVec0.y - vVec1.y ); 
}

template<class T>
inline FVector2<T> operator * ( const FVector2<T> & vVec0, const FVector2<T> & vVec1 )
{
	return FVector2<T>( vVec0.x * vVec1.x, vVec0.y * vVec1.y ); 
}

template<class T>
inline FVector2<T> operator / ( const FVector2<T> & vVec0, const FVector2<T> & vVec1 )
{
	return FVector2<T>( vVec0.x / vVec1.x, vVec0.y / vVec1.y ); 
}

template<class T>
inline FVector2<T> operator * ( const FVector2<T> & vVec0, T t )
{
	return FVector2<T>( vVec0.x * t, vVec0.y * t ); 
}

template<class T>
inline FVector2<T> operator * ( T t, const FVector2<T> & vVec0 )
{
	return FVector2<T>( vVec0.x * t, vVec0.y * t ); 
}

template<class T>
inline FVector2<T> operator / ( const FVector2<T> & vVec0, T t )
{
	return FVector2<T>( vVec0.x / t, vVec0.y / t ); 
}

template<class T>
inline T operator & ( const FVector2<T> & vVec0, const FVector2<T> & vVec1 )
{
	return vVec0.x * vVec1.x + vVec0.y * vVec1.y; 
}

template<class T>
inline T operator ^ ( const FVector2<T> & vVec0, const FVector2<T> & vVec1 )
{
	return vVec0.x*vVec1.y - vVec0.y*vVec1.x; 
}

template<class T>
inline FVector2<T> SLerp( const FVector2<T> & vVec0, const FVector2<T> & vVec1, T t )
{
	return vVec0*t + (1.f - t)*vVec1;
}

template<class T>
inline bool AreCollinear( const FVector2<T> & vVec0, const FVector2<T> & vVec1 )
{
	return (vVec0 ^ vVec1) < EPS * EPS;
}


typedef FVector2<F32> FVector2F;
typedef FVector2<F64> FVector2D;
typedef FVector2<I32> FVector2I;


#endif