#ifndef __FCOLOR_H__
#define __FCOLOR_H__


#include "..\types.h"

template<class T, typename MaxValue>
class FColor
{
public:
	T r, g, b, a;

	inline FColor() : r( 0 ), g( 0 ), b( 0 ), a( MaxValue::GetMax() ){}
	inline FColor( T r, T g, T b, T a ) : r( r ), g( g ), b( b ), a( a ) {}
	inline FColor( const FColor<T, MaxValue> & cColor ) : r( cColor.r ), g( cColor.g ), b( cColor.b ), a( cColor.a ) {}
	~FColor(){}

	inline FColor<T, MaxValue> & operator = ( const FColor<T, MaxValue> & cColor )
	{
		r = cColor.r;
		g = cColor.g;
		b = cColor.b;
		a = cColor.a;

		return *this;
	}

	//FColor<T> operator + ()const
	//{
	//	return *this;
	//}

	//FColor<T> operator - ()const
	//{
	//	return FColor<T>( -r, -g, -b, -a );
	//}

	inline FColor<T, MaxValue> & operator += ( const FColor<T, MaxValue> & cColor )
	{
		r += cColor.r;
		r = min( r, MaxValue::GetMax() );
		g += cColor.g;
		g = min( g, MaxValue::GetMax() );
		b += cColor.b;
		b = min( b, MaxValue::GetMax() );
		a += cColor.a;
		a = min( a, MaxValue::GetMax() );

		return *this;
	}

	inline FColor<T, MaxValue> & operator -= ( const FColor<T, MaxValue> & cColor )
	{
		if( r < cColor.r )
			r = MaxValue::GetMin();
		else
			r -= cColor.r;

		if( g < cColor.g )
			g = MaxValue::GetMin();
		else
			g -= cColor.g;

		if( b < cColor.b )
			b = MaxValue::GetMin();
		else
			b -= cColor.b;

		if( a < cColor.a )
			a = MaxValue::GetMin();
		else
			a -= cColor.a;

		return *this;
	}

	inline FColor<T, MaxValue> & operator *= ( const FColor<T, MaxValue> & cColor )
	{
		r *= cColor.r;
		r = min( r, MaxValue::GetMax() );
		g *= cColor.g;
		g = min( g, MaxValue::GetMax() );
		b *= cColor.b;
		b = min( b, MaxValue::GetMax() );
		a *= cColor.a;
		a = min( a, MaxValue::GetMax() );

		return *this;
	}

	inline FColor<T, MaxValue> & operator /= ( const FColor<T, MaxValue> & cColor )
	{
		r /= cColor.r;
		r = min( r, MaxValue::GetMax() );
		g /= cColor.g;
		g = min( g, MaxValue::GetMax() );
		b /= cColor.b;
		b = min( b, MaxValue::GetMax() );
		a /= cColor.a;
		a = min( a, MaxValue::GetMax() );

		return *this;
	}

	inline FColor<T, MaxValue> & operator *= ( F32 t )
	{
		r *= t;
		r = min( r, MaxValue::GetMax() );
		g *= t;
		g = min( g, MaxValue::GetMax() );
		b *= t;
		b = min( b, MaxValue::GetMax() );
		a *= t;
		a = min( a, MaxValue::GetMax() );

		return *this;
	}

	inline FColor<T, MaxValue> & operator /= ( F32 t )
	{
		r /= t;
		r = min( r, MaxValue::GetMax() );
		g /= t;
		g = min( g, MaxValue::GetMax() );
		b /= t;
		b = min( b, MaxValue::GetMax() );
		a /= t;
		a = min( a, MaxValue::GetMax() );

		return *this;
	}
};


template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> operator + ( const FColor<T, MaxFunc> & cLeft, const FColor<T, MaxFunc> & cRight )
{
	return FColor<T, MaxFunc>( cLeft ) += cRight;
}

template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> operator - ( const FColor<T, MaxFunc> & cLeft, const FColor<T, MaxFunc> & cRight )
{
	return FColor<T, MaxFunc>( cLeft ) -= cRight;
}

template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> operator * ( const FColor<T, MaxFunc> & cLeft, const FColor<T, MaxFunc> & cRight )
{
	return FColor<T, MaxFunc>( cLeft ) *= cRight;
}

template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> operator / ( const FColor<T, MaxFunc> & cLeft, const FColor<T, MaxFunc> & cRight )
{
	return FColor<T, MaxFunc>( cLeft ) /= cRight;
}

template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> operator * ( const FColor<T, MaxFunc> & cLeft, const T fRight )
{
	return FColor<T, MaxFunc>( cLeft ) *= cRight;
}

template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> operator * ( const T fLeft, const FColor<T, MaxFunc> & cRight )
{
	return FColor<T, MaxFunc>( cRight ) *= fLeft;
}

template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> operator / ( const FColor<T, MaxFunc> & cLeft, const T fRight )
{
	return FColor<T, MaxFunc>( cLeft ) /= fRight;
}

template<class T, typename MaxFunc>
inline FColor<T, MaxFunc> Lerp( const FColor<T, MaxFunc> & cLeft, const FColor<T, MaxFunc> & cRight, const F32 T )
{
	return cLeft*T + (MaxFunc::GetMax() - T)*cRight;
}


class UInt8Max
{
public:
	inline static UI8 GetMax()
	{
		return 255;
	}

	inline static UI8 GetMin()
	{
		return 0;
	}
};

class F32Max
{
public:
	inline static F32 GetMax()
	{
		return 1.f;
	}
	inline static F32 GetMin()
	{
		return 0.f;
	}
};

class F64Max
{
public:
	inline static F64 GetMax()
	{
		return 1.0;
	}
	inline static F64 GetMin()
	{
		return 0.0;
	}
};

typedef FColor<UI8, UInt8Max> FColor4U;
typedef FColor<F32, F32Max> FColor4F;
typedef FColor<F64, F64Max> FColor4D;

#endif