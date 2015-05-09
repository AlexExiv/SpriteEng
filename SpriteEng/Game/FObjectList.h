#ifndef __FOBJECT_LIST_H__
#define __FOBJECT_LIST_H__


#include "..\Core\FString.h"

class FObject;

class FObjectList
{
protected:
	struct FNode
	{
		FNode * lpNext;
		FObject * lpObj;
	};

	struct FObjNode
	{
		FObjNode * lpNext;
	};
	UI32 iHashSize;
	FNode ** lpHashNodes;

	UI32 iBlockSize;
	UI32 iMaxObjCount, iCurObjCount;
	UI8 * lpData;

	FObjNode * lpFreeList;

public:
	FObjectList( UI32 iHashSize );
	virtual ~FObjectList();

	void RegisterObject( FObject * lpObject );
	void Reserve( UI32 iCount );

	FObject * AllocObject( const FString & sObjName );
	void DeleteObject( FObject * lpObject );
};

#endif