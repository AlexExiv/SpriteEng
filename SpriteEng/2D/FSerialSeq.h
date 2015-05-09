#ifndef __FSERIAL_SEQ_H__
#define __FSERIAL_SEQ_H__



#include "FDrawSeq.h"
#include "..\types.h"

class FSerialSeq : public FDrawSeq
{
	struct FNode
	{
		FNode * lpNext;
		FPrimitive * lpPrim;
		UI32 iSize, iCount, iIndexCount;
	};

	FNode * lpAllocNodes, * lpHeadNode, * lpLastNode;
	UI32 iAllocCount, iMaxPrimitives;
	FNode * lpLastAdded;

public:
	FSerialSeq( UI32 iMaxPrim );
	~FSerialSeq();

	void BeginBuildSeq() ;
	void AddPrimitive( FPrimitive * lpPrim );
	void EndBuildSeq();
	void DrawSeq();
};

#endif