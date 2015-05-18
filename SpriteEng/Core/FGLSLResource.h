#ifndef __FGLSLRESOURCE_H__
#define __FGLSLRESOURCE_H__


#include "FResource.h"
#include "FArray.h"


////////////шейдер должен иметь вид///////////
/*
[BLOCK_PROGRAM]
текстуры бинд€тс€ к номеру блока
tTexture0 = 0
tTexture1 = 1
.............
tTextureN = N

[VERTEX_PROGRAM]
вершинна€ программа

[TESSELATE_PROGRAM]
тесс€л€циона€ программа

[GEOMETRIC_PROGRAM]
геометрическа€ программа

[FRAGMENT_PROGRAM]
фрагментна€ программа

*/

class FGLSLResource : public FResource
{
	friend class FResourceManager;
protected:
	struct FProgram
	{
		CHAR_ * lpProgram;
		UI32 iLen;
	};

	struct FTextureBlock
	{
		UI32 iBlock;
		FString sTexName;
	};

public:
	enum
	{
		VERTEX_PROGRAM = 0,
		FRAGMENT_PROGRAM,
		GEOMETRIC_PROGRAM,
		TESSELATE_PROGRAM,
		PROGRAM_COUNT
	};

	enum
	{
		PROGRAM_TYPE_INT = 1,
		PROGRAM_TYPE_FLOAT,
		PROGRAM_TYPE_STR
	};

protected:
	typedef FArray<FTextureBlock *> FTextureArray;
	typedef FArray<FTextureBlock *>::Iterator FTextureIterator;

	FProgram pProgram[PROGRAM_COUNT];
	FTextureArray aTextureArray;

	FResource * Make( void * lpPlacement, void * lpData, UI32 iDataLen, FResourceManager * lpCreator ) ;
	void SaveResource( void ** lpData, UI32 & iDataSize ) ;
	UI32 GetSize()const;
	UI32 ParseBlocks( CHAR_ * lpText, UI32 iLen );

	FGLSLResource();

public:
	FGLSLResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	~FGLSLResource();

	CHAR_ * GetVertexProgram()const;
	CHAR_ * GetFragmentProgram()const;
	CHAR_ * GetGeometricProgram()const;
	CHAR_ * GetTesselateProgram()const;
	UI32 GetVertexProgramLen()const;
	UI32 GetFragmentProgramLen()const;
	UI32 GetGeometricProgramLen()const;
	UI32 GetTesselateProgramLen()const;
	UI32 GetBlockCount()const;
	void GetBlockParams( UI32 iIndex, FString & sTexName, UI32 & iBlock )const;
};

#endif