#ifndef __FBASETYPES_H__
#define __FBASETYPES_H__


#include "FObject.h"

//class FInteger : public FObject
//{
//	I32 iInt;
//public:
//	FInteger( I32 iInt );
//	~FInteger();
//
//	I32 GetInt()const;
//	void SetInt( I32 iInt );
//
//	operator I32 () const
//	{
//		return iInt;
//	}
//
//	DEFINE_OBJ_DERIVERED( FInteger );
//};
//
//DEFINE_OBJ_NAME( FInteger );
//
//class FUInteger : public FObject
//{
//	UI32 iUInt;
//
//public:
//	FUInteger( UI32 iUInt );
//	~FUInteger();
//
//	UI32 GetUInt()const;
//	void SetUInt( UI32 iInt );
//
//	operator UI32 () const
//	{
//		return iUInt;
//	}
//
//	DEFINE_OBJ_DERIVERED( FUInteger );
//};
//
//DEFINE_OBJ_NAME( FUInteger );
//
//class FFloat : public FObject
//{
//	F32 fFloat;
//
//public:
//	FFloat( F32 fFloat );
//	~FFloat();
//
//	UI32 GetFloat()const;
//	void SetFloat( F32 fFloat );
//
//	operator F32 () const
//	{
//		return fFloat;
//	}
//
//	DEFINE_OBJ_DERIVERED( FFloat );
//};
//
//DEFINE_OBJ_NAME( FFloat );
//
//#include "..\Core\FColor.h"
//
//class FColor_
//{
//	FColor4F cColor;
//
//public:
//	FColor_
//};

#include "..\Core\FColor.h"
#include "..\Math\FVector2D.h"


#define BASETYPE_DEFINE( TYPE_NAME, TYPE )\
class TYPE_NAME : public FObject\
{\
	TYPE tValue;\
public:\
	TYPE_NAME( TYPE tValue_ ) : FObject( true ), tValue( tValue_ ){}\
	~TYPE_NAME(){}\
\
	void SetValue( TYPE tValue_ ) { tValue = tValue_; }\
	TYPE GetValue()const { return tValue; }\
	\
	operator TYPE () const { return tValue; }\
	DEFINE_OBJ_DERIVERED( TYPE_NAME );\
};\
DEFINE_OBJ_NAME( TYPE_NAME );\
typedef TYPE_NAME * P##TYPE_NAME;

BASETYPE_DEFINE( FInteger, I32 );
BASETYPE_DEFINE( FUInteger, UI32 );
BASETYPE_DEFINE( FFloat, F32 );
#define RFInteger &FInteger
#define RFUInteger &FUInteger
#define RFFloat &FFloat

class FColor4F_ : public FObject
{
	FColor4F cColor;

public:
	FColor4F_( const FColor4F & cColor );
	FColor4F_( F32 r, F32 g, F32 b, F32 a = 1.f );
	~FColor4F_();

	void SetValue( const FColor4F & cColor );
	FColor4F GetValue()const;

	operator FColor4F () const 
	{
		return cColor;
	}
	DEFINE_OBJ_DERIVERED( FColor4F_ );
};

DEFINE_OBJ_NAME( FColor4F_ );

#define RFColor4F &FColor4F_
typedef FColor4F_ * PFColor4F;

class FVector2F_ : public FObject
{
	FVector2F vVec;

public:
	FVector2F_( const FVector2F & vVec );
	FVector2F_( F32 x, F32 y );
	~FVector2F_();

	void SetValue( const FVector2F & vVec );
	FVector2F GetValue()const;

	operator FVector2F () const 
	{
		return vVec;
	}
	DEFINE_OBJ_DERIVERED( FVector2F_ );
};

DEFINE_OBJ_NAME( FVector2F_ );

#define RFVector2F &FVector2F_
typedef FVector2F_ * PFVector2F;


class FRect_ : public FObject
{
	FRect rRect;

public:
	FRect_( const FRect & rRect );
	FRect_( F32 fLeft, F32 fRight, F32 fBottom, F32 fTop );
	~FRect_();

	void SetValue( const FRect & rRect );
	FRect GetValue()const;

	operator FRect () const
	{
		return rRect;
	}

	DEFINE_OBJ_DERIVERED( FRect_ );
};

DEFINE_OBJ_NAME( FRect_ );
#define RFRect &FRect_
typedef FRect_ * PFRect;

#endif