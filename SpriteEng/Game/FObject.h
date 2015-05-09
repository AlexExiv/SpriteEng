#ifndef __FOBJECT_H__
#define __FOBJECT_H__

#include "..\types.h"

class FWorld;

template <class FInheritableObject>
class FObject
{
protected:
	FWorld * lpWorld;

	UI32 GetObjectSize()const
	{
		return sizeof( FInheritableObject );
	}

	FObject();

public:
	virtual ~FObject();

	virtual void Update( F32 fDTime );
};

#endif