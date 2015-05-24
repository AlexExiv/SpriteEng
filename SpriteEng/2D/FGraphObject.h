#ifndef __FGRAPH_OBJECT_H__
#define __FGRAPH_OBJECT_H__


#include "..\Core\FString.h"
#include "..\Core\FDictionary.h"

class FTexture;
class FImageResource;

class FGraphObject
{
	FString sName;
	UI32 iRefCount, iObjType;

public:
	FGraphObject( const FString & sName, UI32 iObjType );
	virtual ~FGraphObject();

	const FString & GetName()const;
	void AddRef();
	void DecrRef();
	bool IsRef()const;
	UI32 GetType()const;
};


class FGraphObjectManager
{
public:
	enum
	{
		OBJECT_ANIMATION = 1,
		OBJECT_SHADER, 
		OBJECT_FONT, 
		OBJECT_TEXTURE,
		OBJECT_IMAGE
	};

protected:
	struct FNode
	{
		FNode * lpNext, * lpPrev;
		FGraphObject * lpObject;
	};

	typedef FDictionary<FGraphObject *> FObjectChache;
	typedef FDictionary<FGraphObject *>::FRecord FObjectRecord;

	FNode * lpNodeArr, * lpFreeArr;
	UI32 iNodeCount, iCurNode;
	FNode * lpObjectHash[MAX_HASH];

	FObjectChache dObjectChache;

	FNode * AllocNode();
	void FreeNode( FNode * lpNode );
	FGraphObject * Find( const FString & sName );
public:
	FGraphObjectManager();
	~FGraphObjectManager();

	FGraphObject * CreateObject( const FString & sName, UI32 iObjType );
	FTexture * CreateTexture( const FString & sName, const FImageResource * lpImg );
	void ReleaseObject( FGraphObject * lpObject );
	void Clear();

	void ChacheObject( const FString & sName, UI32 iObjType );
	void UnChacheObject( const FString & sName );
	void ClearChache();

	static FGraphObjectManager * GetInstance();
};

#endif