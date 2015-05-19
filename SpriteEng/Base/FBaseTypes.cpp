#include "..\Core\FString.h"
#include "FBaseTypes.h"

IMPLEMENT_OBJ_DERIVERED( FInteger );
IMPLEMENT_OBJ_DERIVERED( FUInteger );
IMPLEMENT_OBJ_DERIVERED( FFloat );
IMPLEMENT_OBJ_DERIVERED( FColor4F_ );
IMPLEMENT_OBJ_DERIVERED( FVector2F_ );
IMPLEMENT_OBJ_DERIVERED( FRect_ );



FColor4F_::FColor4F_( const FColor4F & cColor ) : FObject( true ), cColor( cColor )
{
}

FColor4F_::FColor4F_( F32 r, F32 g, F32 b, F32 a ) : FObject( true ), cColor( r, g, b, a )
{
}

FColor4F_::~FColor4F_()
{
}

void FColor4F_::SetValue( const FColor4F & cColor_ )
{
	cColor = cColor_;
}

FColor4F FColor4F_::GetValue()const
{
	return cColor;
}



FVector2F_::FVector2F_( const FVector2F & vVec ) : FObject( true ), vVec( vVec )
{
}

FVector2F_::FVector2F_( F32 x, F32 y ) : FObject( true ), vVec( x, y )
{
}

FVector2F_::~FVector2F_()
{
}

void FVector2F_::SetValue( const FVector2F & vVec_ )
{
	vVec = vVec_;
}

FVector2F FVector2F_::GetValue()const
{
	return vVec;
}


FRect_::FRect_( const FRect & rRect ) : FObject( true ), rRect( rRect )
{
}

FRect_::FRect_( F32 fLeft, F32 fRight, F32 fBottom, F32 fTop ) : FObject( true ), rRect( fLeft, fRight, fBottom, fTop )
{
}

FRect_::~FRect_()
{
}

void FRect_::SetValue( const FRect & rRect_ )
{
	rRect = rRect_;
}

FRect FRect_::GetValue()const
{
	return rRect;
}



//FInteger::FInteger( I32 iInt ) : iInt( iInt )
//{
//}
//
//FInteger::~FInteger()
//{
//}
//
//I32 FInteger::GetInt()const
//{
//	return iInt;
//}
//
//void FInteger::SetInt( I32 iInt_ )
//{
//	iInt = iInt_;
//}
//
//
//FUInteger::FUInteger( UI32 iUInt ) : iUInt( iUInt )
//{
//}
//
//FUInteger::~FUInteger()
//{
//}
//
//
//UI32 FUInteger::GetUInt()const
//{
//	return iUInt;
//}
//
//void FUInteger::SetUInt( UI32 iUInt_ )
//{
//	iUInt = iUInt_;
//}
//
//FFloat::FFloat( F32 fFloat ) : fFloat( fFloat )
//{
//}
//
//FFloat::~FFloat()
//{
//}
//
//
//UI32 FFloat::GetFloat()const
//{
//	return fFloat;
//}
//
//void FFloat::SetFloat( F32 fFloat_ )
//{
//	fFloat = fFloat_;
//}


