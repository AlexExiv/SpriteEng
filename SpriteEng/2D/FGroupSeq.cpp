#include "FGroupSeq.h"
#include "FPrimitive.h"
#include "..\Core\FStack.h"
#include "..\Core\FLog.h"
#include "..\Core\FException.h"



FGroupSeq::FGroupSeq( UI32 iMaxDrawNode ) : iCurAlloc( 0 ), lpAllocBatches( NULL ), iMaxDrawNode( iMaxDrawNode )
{
	lpAllocBatches = (FBatchNode *) FMalloc( sizeof( FBatchNode ) * iMaxDrawNode );
}

FGroupSeq::~FGroupSeq()
{
	if( lpAllocBatches )
		FFree( lpAllocBatches );
}

void FGroupSeq::BeginBuildSeq()
{
	ZeroData();
}

void FGroupSeq::AddPrimitive( FPrimitive * lpPrim )
{
	if( !lpPrim->IsVisible() )
		return;

	PRIM_ID iID = lpPrim->GetID();
	UI32 iKey = iID % PRIM_ID( MAX_DRAW_HASH );
	UI32 iLayer = lpPrim->GetLayer();
	FBatchNode * lpBatch = lpHashBatches[iKey], * lpPrevBatch = NULL;

	while( lpBatch )
	{
		if( (lpBatch->iPrimID == iID) && (lpBatch->iLayer == iLayer) )
		{
			if( (lpBatch->iCount + 1) == FGroupSeq::MAX_BATCH_PRIM )//батч переполнен надо создать новый
				break;

			lpBatch->lpPrim[lpBatch->iCount++] = lpPrim;
			return;
		}
		lpPrevBatch = lpBatch;
		lpBatch = lpBatch->lpHashNext;
	}

	//создаем новый батч
	FBatchNode * lpNewBatch = AllocNode();
	if( lpNewBatch == NULL )//упс закончились батчи, новый не добавляем а выводим сообщение о соотв ошибке
	{
		FLog::PutError( "Batch alloc nodes has ended, need more" );
		return;
	}
	lpNewBatch->iCount = 1;
	lpNewBatch->lpPrim[0] = lpPrim;
	lpNewBatch->iLayer = iLayer;
	lpNewBatch->iPrimID = iID;

	if( lpBatch )//если батч создается в связи с переполнением старого и добавляем его перед переполененым, чтоб тот не участвовал в поиске
	{
		lpNewBatch->lpHashNext = lpBatch;
		if( lpPrevBatch )
			lpPrevBatch->lpHashNext = lpNewBatch;
		else
			lpHashBatches[iKey] = lpNewBatch;
	}
	else
	{
		//устанавливаем необходимые поля для хеш поиска
		lpNewBatch->lpHashNext = lpHashBatches[iKey];
		lpHashBatches[iKey] = lpNewBatch;
	}
	//устанавливаем необходимые поля для списка на очередь рисования и добавляем в список
	lpNewBatch->lpListNext = lpDrawBatches[iLayer];
	lpDrawBatches[iLayer] = lpNewBatch;
}

void FGroupSeq::EndBuildSeq()
{
}

void FGroupSeq::DrawSeq()
{
	//рисуем все батчи в очередности слоев от низшего к высшему
	for( UI32 i = 0;i < FScene::MAX_LAYER_COUNT;i++ )
	{
		FBatchNode * lpBatch = lpDrawBatches[i];
		while( lpBatch )
		{
			DrawBatch( lpBatch );
			lpBatch = lpBatch->lpListNext;
		}
	}
}

void FGroupSeq::ZeroData()
{
	iCurAlloc = 0;
	for( UI32 i = 0;i < FGroupSeq::MAX_DRAW_HASH;i++ )
		lpHashBatches[i] = NULL;
	for( UI32 i = 0;i < FScene::MAX_LAYER_COUNT;i++ )
		lpDrawBatches[i] = NULL;
}

void FGroupSeq::DrawBatch( FGroupSeq::FBatchNode * lpBatch )
{
	UI8 * lpData = NULL;
	UI32 * lpIndeces = NULL;

	try//возможно исключение с недостатком памяти в стеке, в этом случае батч не отрисуется, но будет оставлена запись в логе
	{
		UI32 iPrimCount = 0;
		if( lpBatch->lpPrim[0]->DrawType() & FPrimitive::DRAW_NEED_COUNTING )//размер примитива не статичен, необходимо подсчитать общее кол-во, для выделения памяти в стеке
		{
			UI32 iSumSize = 0, iSumInd = 0;
			UI32 iBlocksSizes[FGroupSeq::MAX_BATCH_PRIM], iBlockInds[FGroupSeq::MAX_BATCH_PRIM];
			if( lpBatch->lpPrim[0]->DrawType() & FPrimitive::DRAW_INDEXED )//если примитив индексированый
			{
				for( UI32 i = 0;i < lpBatch->iCount;i++ )
				{
					iBlocksSizes[i] = lpBatch->lpPrim[i]->GetDataSizePerInst();
					iBlockInds[i] = lpBatch->lpPrim[i]->GetIndexCount();
					iSumSize += iBlocksSizes[i];
					iSumInd += iBlockInds[i];
				}
			}
			else
			{
				for( UI32 i = 0;i < lpBatch->iCount;i++ )
				{
					iBlocksSizes[i] = lpBatch->lpPrim[i]->GetDataSizePerInst();
					iSumSize += iBlocksSizes[i];
				}
			}

			lpData = (UI8 *)PUSH_BLOCKT( iSumSize );
			UI8 * lpDstData = lpData;

			if( lpBatch->lpPrim[0]->DrawType() & FPrimitive::DRAW_INDEXED )
			{
				UI32 iStartVert = 0, 
					iIndCount = lpBatch->lpPrim[0]->GetIndexCount(), 
					iVertCount = lpBatch->lpPrim[0]->GetVertexCount();
				lpIndeces = (UI32 *)PUSH_BLOCKT( iSumInd*sizeof( UI32 ) );
				UI32 * lpDstInd = lpIndeces;

				for( UI32 i = 0;i < lpBatch->iCount;i++ )
				{
					iPrimCount += lpBatch->lpPrim[i]->AddDataToSeq( lpDstData, lpDstInd, iStartVert );

					iStartVert += lpBatch->lpPrim[i]->GetVertexCount();
					lpDstInd += iBlockInds[i];
					lpDstData += iBlocksSizes[i];
				}
			}
			else
			{
				for( UI32 i = 0;i < lpBatch->iCount;i++ )
				{
					iPrimCount += lpBatch->lpPrim[i]->AddDataToSeq( lpDstData, NULL, 0 );
					lpDstData += iBlocksSizes[i];
				}
			}
			lpBatch->lpPrim[0]->DrawCallback( iPrimCount, lpData, lpIndeces );

			if( lpIndeces )
				POP_BLOCK;
			POP_BLOCK;
		}
		else
		{
			UI32 iBlockSize = lpBatch->lpPrim[0]->GetDataSizePerInst();
			lpData = (UI8 *)PUSH_BLOCKT( lpBatch->iCount*iBlockSize );
			UI8 * lpDstData = lpData;

			if( lpBatch->lpPrim[0]->DrawType() & FPrimitive::DRAW_INDEXED )//если примитив индексированый
			{
				UI32 iStartVert = 0, 
					iIndCount = lpBatch->lpPrim[0]->GetIndexCount(), 
					iVertCount = lpBatch->lpPrim[0]->GetVertexCount();
				lpIndeces = (UI32 *)PUSH_BLOCKT( lpBatch->iCount*iIndCount*sizeof( UI32 ) );
				UI32 * lpDstInd = lpIndeces;

				for( UI32 i = 0;i < lpBatch->iCount;i++, lpDstData += iBlockSize, iStartVert += iVertCount, lpDstInd += iIndCount )
					iPrimCount += lpBatch->lpPrim[i]->AddDataToSeq( lpDstData, lpDstInd, iStartVert );
			}
			else
			{
				for( UI32 i = 0;i < lpBatch->iCount;i++, lpDstData += iBlockSize )
					iPrimCount += lpBatch->lpPrim[i]->AddDataToSeq( lpDstData, NULL, 0 );
			}
			lpBatch->lpPrim[0]->DrawCallback( iPrimCount, lpData, lpIndeces );

			if( lpIndeces )
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