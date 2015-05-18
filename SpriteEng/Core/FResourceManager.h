#ifndef __FRESOURCE_MANAGER_H__
#define __FRESOURCE_MANAGER_H__

#include "FList.h"
#include "FString.h"

class FResource;
//�������� ��������, ������������� ���� ������� ���������� �� ��� ����������. ������ ��� ������� ���������� �� ��������� �������!! �� ������� ��������� �� ������ ������ ����� ������� ��� ������!!
//������ ��� ������������� ������ + ������ ������ ������� ������ ��� RESOURCE_ALLOC_SIZE �� ����� ���� ������
class FResourceManager
{
	enum
	{
		RESOURCE_ALLOC_SIZE = (10*1024*1024),
		RESOURCE_COUNT = 16
	};

	typedef FList<FResource *> FResourcesList;
	typedef FList<FResource *>::Iterator FResourceIterator;

	static FResourceManager * lpManager;
	FResourcesList lResources;
	UI32 iResAllocSize, iCurAlloc;
	FBYTE * lpData;
	UI32 iMaxResSize, iAllocCount;
	FBYTE * lpResData;

	FResourceManager( UI32 iResAllocSize );
	FResourceManager( const FResourceManager & );
	FResourceManager & operator = ( const FResourceManager& );

	void RegisterResource( FResource * lpResource );
	void * AllocRes();
	void DestructAllRes();

public:
	~FResourceManager();

	static FResourceManager * SharedManager();

	FResource * CreateResource( const FString & sFileName );
	void SaveResource( const FString & sFileName, FResource * lpRes );
	void SaveResource( void ** lpBuffer, UI32 & iWritedLen, FResource * lpRes );
	void * AllocForResource( UI32 iSize );//��������� ������
	void ResetAllocator();//�������� ��������� ������
	UI32 GetAllocSize()const;//������������ ������ ���������� ������
};

#endif