#ifndef __FRESOURCE_MANAGER_H__
#define __FRESOURCE_MANAGER_H__

#include "FList.h"
#include "FString.h"

class FResource;
//Менеджер ресурсов, идентификация типа ресурса происходит по его расширению. Память под ресурсы выделяется во временной области!! не хранить указатели на данные дольше одной функции или метода!!
//Ресурс чей декодированый размер + размер самого ресурса больше чем RESOURCE_ALLOC_SIZE не может быть открыт
class FResourceManager
{
	enum
	{
		RESOURCE_ALLOC_SIZE = (10*1024*1024)
	};

	typedef FList<FResource *> FResourcesList;
	typedef FList<FResource *>::Iterator FResourceIterator;

	static FResourceManager * lpManager;
	FResourcesList lResources;
	UI32 iResAllocSize, iCurAlloc;
	FBYTE * lpData;

	FResourceManager( UI32 iResAllocSize );
	FResourceManager( const FResourceManager & );
	FResourceManager & operator = ( const FResourceManager& );

	void RegisterResource( FResource * lpResource );

public:
	~FResourceManager();

	static FResourceManager * SharedManager();

	FResource * CreateResource( const FString & sFileName );
	void SaveResource( const FString & sFileName, FResource * lpRes );
	void SaveResource( void ** lpBuffer, UI32 & iWritedLen, FResource * lpRes );
	void * AllocForResource( UI32 iSize );//выделение памяти
	void ResetAllocator();//сбросить выделеную память
	UI32 GetAllocSize()const;//максимальный размер выделяемой памяти
};

#endif