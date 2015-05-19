#ifndef __FOBJECT_H__
#define __FOBJECT_H__

//class FObject;

//#include "..\Core\FString.h"
#include "..\types.h"

class FString;

class FObjectAllocator;

class FObject
{
	friend class FObjectAllocator;

	//enum
	//{

	//};

	UI32 iRefCount;
	FObjectAllocator * lpAllocator;
	UI32 iObjFlags;

	void SetAllocator( FObjectAllocator * lpAllocator );
	
protected:
	FObject * AllocObject( const FString & sObjName, FObject * lpFirst, ... );
	FObject * AllocObjectT( const FString & sObjName, FObject * lpFirst, ... );//выделение памяти под объек генерирует исключение
	void Delete( FObject * lpObject );

	bool CheckCallTypes( const CHAR_ * lpCallTypes, const CHAR_ * lpObjTypes )const;
	FObject( bool bResetAlloc = false );
	FObject( UI32 iObjReserved );

public:
	virtual ~FObject();

	virtual void Update( F32 fDTime );

	void AddRef();
	void Release();

	virtual FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) ;
	virtual FString GetObjName()const;

	static FString GetNameFObject();
	static void InitMeta();
};

#define DEFINE_OBJ_NAME( Object_ ) extern const FString M##Object_;

#define DEFINE_OBJ_DERIVERED( Object_ ) public:\
										FString GetObjName()const;\
										static FString GetName##Object_();

#define IMPLEMENT_OBJ_DERIVERED( Object_ ) \
										static const FString M##Object_( #Object_ );\
										FString Object_::GetObjName()const { return M##Object_; }\
										FString Object_::GetName##Object_() { return M##Object_; }

#define GET_OBJNAME( Object_ ) Object_::GetName##Object_()

DEFINE_OBJ_NAME( FObject );

#endif