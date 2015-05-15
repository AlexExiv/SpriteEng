#include "FSerialSeq.h"
#include "..\Core\FLog.h"
#include "..\Core\FException.h"
#include "..\Core\FStack.h"
#include "FPrimitive.h"




FSerialSeq::FSerialSeq( UI32 iMaxPrim ) : iMaxPrimitives( iMaxPrim ), lpAllocNodes( NULL ), lpHeadNode( NULL ), lpLastNode( NULL ), iAllocCount( 0 )
{
	lpAllocNodes = (FNode *)FMalloc( sizeof( FNode )*iMaxPrimitives );
	lpHeadNode = lpAllocNodes;
}

FSerialSeq::~FSerialSeq()
{
	if( lpAllocNodes )
		FFree( lpAllocNodes );
}


void FSerialSeq::BeginBuildSeq() 
{
	lpLastNode = lpHeadNode;
	iAllocCount = 0;
	lpLastAdded = NULL;
}

void FSerialSeq::AddPrimitive( FPrimitive * lpPrim )
{
	if( iAllocCount == iMaxPrimitives )
	{
		FLog::PutError( "Primitive count is insuffience" );
		return;
	}
	FNode * lpNode = &lpAllocNodes[iAllocCount++];

	lpLastNode->lpNext = lpNode;
	lpLastNode = lpNode;
	lpNode->lpNext = NULL;
	lpNode->lpPrim = lpPrim;

	//группируем соседнии примитивы имеющие одинаковые идентификаторы
	if( lpLastAdded && (lpLastAdded->lpPrim->GetID() == lpPrim->GetID()) )
	{
		lpLastAdded->iCount++;
		lpLastAdded->iSize += lpPrim->GetDataSizePerInst();
		lpLastAdded->iIndexCount += lpPrim->GetIndexCount();
	}
	else
	{
		lpLastAdded = lpNode;
		lpLastAdded->iCount = 1;
		lpLastAdded->iSize = lpPrim->GetDataSizePerInst();
		lpLastAdded->iIndexCount = lpPrim->GetIndexCount();
	}
}

void FSerialSeq::EndBuildSeq()
{
}

void FSerialSeq::DrawSeq()
{
	void * lpData = NULL;
	Index * lpIndeces = NULL;

	try
	{
		FNode * lpNode = lpHeadNode;
		for( UI32 i = 0;i < iAllocCount;i++ )
		{
			lpData = NULL;
			lpIndeces = NULL;
			lpData = PUSH_BLOCKT( lpNode->iSize );
			lpIndeces = (Index *)PUSH_BLOCKT( lpNode->iIndexCount*sizeof( Index ) );
			FNode * lpLastNode;
			UI32 iPrimCount = 0;

			if( lpNode->iCount > 1 )
			{
				UI32 iStartVert = 0;
				FBYTE * lpVertData = (FBYTE *)lpData;
				UI32 * lpInd = lpIndeces;
				FNode * lpFirstNode = lpNode;

				for( UI32 j = 0;j < lpFirstNode->iCount;j++ )
				{
					iPrimCount += lpNode->lpPrim->AddDataToSeq( lpVertData, lpInd, iStartVert );
					lpVertData += lpNode->lpPrim->GetDataSizePerInst();
					lpInd += lpNode->lpPrim->GetIndexCount();
					iStartVert += lpNode->lpPrim->GetVertexCount();

					lpLastNode = lpNode;
					lpNode = lpNode->lpNext;
				}
				i += (lpFirstNode->iCount - 1);
			}
			else
			{
				iPrimCount += lpNode->lpPrim->AddDataToSeq( lpData, lpIndeces, 0 );
				lpLastNode = lpNode;
			}

			lpLastNode->lpPrim->DrawCallback( iPrimCount, lpData, lpIndeces );
			lpNode = lpLastNode->lpNext;

			POP_BLOCK;
			POP_BLOCK;
		}
	}
	catch( FException eExcp )
	{
		if( lpData )
			POP_BLOCK;
		if( lpIndeces )
			POP_BLOCK;
		FLog::PutError( "Can't draw batch, need more stack memory" );
	}
}

