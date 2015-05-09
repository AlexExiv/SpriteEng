#ifndef __FGRAPH_OBJECT_H__
#define __FGRAPH_OBJECT_H__


#include "..\Core\FString.h"

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

	FNode * lpNodeArr, * lpFreeArr;
	UI32 iNodeCount, iCurNode;
	FNode * lpObjectHash[MAX_HASH];

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

	static FGraphObjectManager * GetInstance();
};

#endif