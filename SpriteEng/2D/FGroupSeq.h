#ifndef __FGROUPSEQ_H__
#define __FGROUPSEQ_H__



#include "FDrawSeq.h"
#include "..\Math\FVector2D.h"
#include "FScene.h"

class FGroupSeq : public FDrawSeq
{
public:
	enum
	{
		MAX_DRAW_HASH = 2048,
		MAX_DRAW_NODE = 512,
		MAX_BATCH_PRIM = 256
	};

private:
	struct FBatchNode
	{
		FBatchNode * lpHashNext, * lpListNext;
		FPrimitive * lpPrim[MAX_BATCH_PRIM];
		UI32 iCount;
		PRIM_ID iPrimID;
		UI32 iLayer;
	};

	FBatchNode * lpHashBatches[MAX_DRAW_HASH], 
		* lpDrawBatches[FScene::MAX_LAYER_COUNT], 
		* lpAllocBatches;
	UI32 iCurAlloc, iMaxDrawNode;

	void ZeroData();
	inline FBatchNode * AllocNode()
	{
		if( iCurAlloc == iMaxDrawNode )
			return NULL;
		return &lpAllocBatches[iCurAlloc++];
	}
	void DrawBatch( FBatchNode * lpBatch );

public:
	FGroupSeq( UI32 iMaxDrawNode );
	~FGroupSeq();

	void BeginBuildSeq();
	void AddPrimitive( FPrimitive * lpPrim );
	void EndBuildSeq();
	void DrawSeq();
};

#endif