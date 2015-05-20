#include "FView.h"
#include "FAnimation2D.h"
#include <vector>
#include "..\Core\FAtlasResource.h"
#include "..\Core\FException.h"
#include "..\Core\FResourceManager.h"
#include "..\Core\FLog.h"
#include "FTexture.h"
#include "FShader.h"



#define ANIM_DATA_CONT_COUNT 3

struct FAnimVertexData
{
	FVector2F vPoint;
	FVector2F vTexCoord;
	FColor4F cColor;
};


const FString sAnimShaderName( "bundle\\anim.shr" );
const FString sAtlasTex( "tAtlasTex" );

const FDrawDataCont dAnimDataCont[ANIM_DATA_CONT_COUNT] =
{
	{ FView::VERTEX_DATA, 2, FView::DATA_FLOAT, sizeof( FAnimVertexData ), FIELD_OFFSET_( FAnimVertexData, vPoint ) },
	{ FView::TEXCOORD_DATA, 2, FView::DATA_FLOAT, sizeof( FAnimVertexData ), FIELD_OFFSET_( FAnimVertexData, vTexCoord ) },
	{ FView::COLOR_DATA, 4, FView::DATA_FLOAT, sizeof( FAnimVertexData ), FIELD_OFFSET_( FAnimVertexData, cColor ) }
};

FAnimation2D::FAnimation2D( const FString & sName ) : FGraphObject( sName, FGraphObjectManager::OBJECT_ANIMATION ), aFrameArr( 128 ), iAtlasBlock( 0 )
{
	FAtlasResource * lpAtlasRes = NULL;
	try
	{
		lpAtlasRes = (FAtlasResource *)FResourceManager::SharedManager()->CreateResource( sName );
		if( !lpAtlasRes )
		{
			FLog::PutError( "Can't open atlas file, file corrupted or not exist, name %s", sName.GetChar() );
			throw FException( FException::EXCP_FILE_NOT_FOUND, sName );
		}
		for( UI32 i = 0;i < lpAtlasRes->GetFrameCount();i++ )
		{
			FFrame * lpFrame = new FFrame;
			FAtlasResource::FFrame fAtlasFrame;
			lpAtlasRes->GetFrameData( &fAtlasFrame, i );
			lpFrame->u0 = fAtlasFrame.u0;
			lpFrame->v0 = fAtlasFrame.v0;
			lpFrame->u1 = fAtlasFrame.u1;
			lpFrame->v1 = fAtlasFrame.v1;
			lpFrame->fWidth = fAtlasFrame.iWidth;
			lpFrame->fHeight = fAtlasFrame.iHeight;
			aFrameArr.Add( lpFrame );
		}
		for( UI32 i = 0;i < lpAtlasRes->GetAnimationCount();i++ )
		{
			FAnimation * lpAnim = new FAnimation;
			FAtlasResource::FAnimation aAtlasAnim;
			lpAtlasRes->GetAnimationData( &aAtlasAnim, i );
			lpAnim->iStartFrame = aAtlasAnim.iStartFrame;
			lpAnim->iEndFrame = aAtlasAnim.iEndFrame;
			lpAnim->bLooped = aAtlasAnim.bLooped;
			lpAnim->sAnimName = aAtlasAnim.sAnimName;
			dAnimatioDict.AddRecord( lpAnim->sAnimName, lpAnim );
		}
		lpTexture = FView::GetMainView()->CreateTexture( sName, lpAtlasRes->GetAtlasWidth(), lpAtlasRes->GetAtlasHeight(), FTexture::TEX_RGBA );
		lpTexture->SetData( FTexture::TEX_RGBA, lpAtlasRes->GetData() );
		lpShader = (FShader *)FGraphObjectManager::GetInstance()->CreateObject( sAnimShaderName, FGraphObjectManager::OBJECT_SHADER );
		if( !lpShader )
			throw FException( FException::EXCP_FILE_NOT_FOUND, FString( "Can't create animation object not all component are exist" ) );
		iAtlasBlock = lpShader->GetTextureBlock( sAtlasTex );
		//delete lpAtlasRes;
	}
	catch( FException eExcp )
	{
		//if( lpAtlasRes )
		//	delete lpAtlasRes;
		DeleteAll();
		//this->~FAnimation2D();
		throw eExcp;
	}
}

FAnimation2D::~FAnimation2D()
{
	DeleteAll();
}

void FAnimation2D::DeleteAll()
{
	for( UI32 i = 0;i < aFrameArr.GetCount();i++ )
		delete (*aFrameArr[i]);
	aFrameArr.Clear();

	FAnimationDict::FStringList lStringList;
	dAnimatioDict.GetKeyList( lStringList );

	FAnimationDict::FStringIterator iIt = lStringList.Begin();
	for(;iIt != lStringList.End();iIt++ )
	{
		FAnimationRecord rRec = dAnimatioDict.FindRecord( *iIt );
		delete rRec;
	}
	dAnimatioDict.Clear();

	if( lpTexture )
		delete lpTexture;
	lpTexture = NULL;
	if( lpShader )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpShader );
	lpShader = NULL;
}

F32 FAnimation2D::GetWidth( UI32 iFrame )const
{
	if( iFrame >= aFrameArr.GetCount() )
	{
		FLog::PutError( "Frame index is out of maximum index %i, max %i", iFrame, aFrameArr.GetCount() );
		return 0;
	}

	return aFrameArr[iFrame]->fWidth;
}

F32 FAnimation2D::GetHeight( UI32 iFrame )const
{
	if( iFrame >= aFrameArr.GetCount() )
	{
		FLog::PutError( "Frame index is out of maximum index %i, max %i", iFrame, aFrameArr.GetCount() );
		return 0;
	}

	return aFrameArr[iFrame]->fHeight;
}

void FAnimation2D::GetAnimInf( const FString & sAnimName, FAnimation2D::FAnimation * lpInfo )
{
	FAnimationRecord rRec = dAnimatioDict.FindRecord( sAnimName );
	if( rRec != FAnimationRecord( NULL ) )
	{
		lpInfo->iStartFrame = rRec->iStartFrame;
		lpInfo->iEndFrame = rRec->iEndFrame;
		lpInfo->sAnimName = rRec->sAnimName;
		lpInfo->bLooped = rRec->bLooped;
	}
	else
	{
		lpInfo->iStartFrame = 0;
		lpInfo->iEndFrame = 0;
		lpInfo->bLooped = false;
		FLog::PutError( "Unknown animation name\"%s\" see avaible in object \"%s\"", sAnimName.GetChar(), GetName().GetChar() );
	}
}

void FAnimation2D::Draw( const FVector2F & vPos, F32 fScale, UI32 iFrameIndex, F32 fAlpha, void * lpData, UI32 * lpIndeces, UI32 iStartVert )
{
	FColor4F cColor( 0.5f, 0.6f, 0.1f, fAlpha );

	//3--2
	//|\ |
	//| \|
	//0--1
	//

	lpIndeces[0] = iStartVert + 0;
	lpIndeces[1] = iStartVert + 1;
	lpIndeces[2] = iStartVert + 3;
	lpIndeces[3] = iStartVert + 1;
	lpIndeces[4] = iStartVert + 2;
	lpIndeces[5] = iStartVert + 3;

	FFrame * lpFrame = *aFrameArr[iFrameIndex];
	FAnimVertexData * lpVertData = (FAnimVertexData *)lpData;
	//#0
	lpVertData->cColor = cColor;
	lpVertData->vPoint = vPos;
	lpVertData->vTexCoord = FVector2F( lpFrame->u0, lpFrame->v1 );
	lpVertData++;
	//#1
	lpVertData->cColor = cColor;
	lpVertData->vPoint = FVector2F( vPos.x + lpFrame->fWidth*fScale, vPos.y );
	lpVertData->vTexCoord = FVector2F( lpFrame->u1, lpFrame->v1 );
	lpVertData++;
	//#2
	lpVertData->cColor = cColor;
	lpVertData->vPoint = FVector2F( vPos.x + lpFrame->fWidth*fScale, vPos.y + lpFrame->fHeight*fScale );
	lpVertData->vTexCoord = FVector2F( lpFrame->u1, lpFrame->v0 );
	lpVertData++;
	//#3
	lpVertData->cColor = cColor;
	lpVertData->vPoint = FVector2F( vPos.x, vPos.y + lpFrame->fHeight*fScale );
	lpVertData->vTexCoord = FVector2F( lpFrame->u0, lpFrame->v0 );
}

UI32 FAnimation2D::GetDrawContCount()const
{
	return ANIM_DATA_CONT_COUNT;
}

const FDrawDataCont * FAnimation2D::GetDrawDataCont()const
{
	return dAnimDataCont;
}

UI32 FAnimation2D::GetAnimDataSize()const
{
	return 4*sizeof( FAnimVertexData );
}

void FAnimation2D::Bind()
{
	if( lpShader )
		lpShader->Bind();
	if( lpTexture )
		lpTexture->Bind( iAtlasBlock );
}

void FAnimation2D::UnBind()
{
	if( lpTexture )
		lpTexture->UnBind( iAtlasBlock );
	if( lpShader )
		lpShader->UnBind();
}
