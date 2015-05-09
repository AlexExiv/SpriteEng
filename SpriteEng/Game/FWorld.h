#ifndef __FWORLD_H__
#define __FWORLD_H__


#include "..\types.h"
#include "..\Core\FList.h"
#include "..\Base\FObjectAllocator.h"
#include "..\Base\FObject.h"


class FScene;
class FGame;

class FWorld : public FObject
{
protected:
	typedef FList<FObject *> FObjectList;
	typedef FList<FObject *>::Iterator FObjectIterator;

	FScene * lpScene;
	FGame * lpGame;
	FObjectList lObjectList;
	F32 fWorldWidth, fWorldHeight;

	FWorld( const FWorld & wWorld ) ;
	FWorld & operator = ( const FWorld & wWorld ) ;

public:
	FWorld( FScene * lpScene, FGame * lpGame );
	~FWorld();   

	void Update( F32 fDTime );

	//FObjectAllocator * GetAllocator()const;
	FScene * GetScene()const;
	F32 GetWorldWidth()const;
	F32 GetWorldHeight()const;

	static void SetCurrentWorld( FWorld * lpWorld );
	static FWorld * GetCurrentWorld();
};

#endif