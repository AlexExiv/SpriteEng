#include "FText.h"
#include "FView.h"
#include "FScene.h"
#include "FFont.h"
#include <new.h>
#include <stdarg.h>



FText::FText( const FVector2F & vPos, const FColor4F & cColor, F32 fFontSize, const FString & sText, const FString & sFontName, 
			 UI32 iLayer, FScene * lpScene ) : FPrimitive( iLayer, lpScene ), sText( sText ), vPos( vPos ), cTextColor( cColor ), fFontSize( fFontSize ), iPrimCount( 0 ), iDataSize( 0 ), iIndCount( 0 ), iVertCount( 0 )
{
	lpFont = (FFont *)FGraphObjectManager::GetInstance()->CreateObject( sFontName, FGraphObjectManager::OBJECT_FONT );
	fAlpha = cTextColor.a;
	if( !lpFont )
		return;

	iPrimCount = lpFont->GetTextPrimCount( sText.GetChar() );
	iDataSize = iPrimCount * lpFont->GetCharDataSize();
	iVertCount = iPrimCount * lpFont->GetCharVertCount();
	iIndCount = iPrimCount * lpFont->GetCharIndCount();
	
	FRect rRect;
	lpFont->GetTextRect( sText.GetChar(), &rRect, fFontSize );
	fTextWidth = rRect.fRight;
	fTextHeight = rRect.fTop;
}

FText::~FText()
{
	if( lpFont )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpFont );
}

void FText::DrawCallback( UI32 iPrimCount_, void * lpData, UI32 * lpIndeces )
{
	if( !lpFont )
		return;

	FView * lpView = FView::GetMainView();

	lpFont->Bind();
	lpView->SetPrimDataArr( lpFont->GetDrawDataCont(), lpFont->GetDrawContCount(), lpData );
	lpView->DrawIndexed( FView::PRIM_TRIANGLES, iPrimCount_*6, lpIndeces );
	lpFont->UnBind();
}

UI32 FText::AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert )
{
	if( !lpFont )
		return 0;

	//FVector2F vPos_ = vPos + 0.5f*(1.f - fScale)*FVector2F( fTextWidth, fTextHeight );
	lpFont->DrawInLine( vPos, sText.GetChar(), fFontSize*fScale, cTextColor, lpData, lpIndeces, iStartVert );
	return iPrimCount;
}

UI32 FText::GetDataSizePerInst()const
{
	return iDataSize;
}

UI32 FText::GetIndexCount()const
{
	return iIndCount;
}

UI32 FText::GetVertexCount()const
{
	return iVertCount;
}

UI32 FText::DrawType()const
{
	return FPrimitive::DRAW_INDEXED | FPrimitive::DRAW_NEED_COUNTING;
}


void FText::Draw( const FRect & rViewport )
{
	lpScene->AddToDrawSeq( this );
}

void FText::Update( F32 fDTime )
{
}


PRIM_ID FText::GetID()const
{
	return PRIM_ID( lpFont );
}


void FText::SetText( const FString & sText0 )
{
	sText = sText0;

	if( !lpFont )
		return;

	iPrimCount = lpFont->GetTextPrimCount( sText.GetChar() );
	iDataSize = iPrimCount * lpFont->GetCharDataSize();
	iVertCount = iPrimCount * lpFont->GetCharVertCount();
	iIndCount = iPrimCount * lpFont->GetCharIndCount();

	FRect rRect;
	lpFont->GetTextRect( sText.GetChar(), &rRect, fFontSize );
	fTextWidth = rRect.fRight;
	fTextHeight = rRect.fTop;
}

void FText::Move( const FVector2F & vDPos )
{
	vPos += vDPos;
}

void FText::SetPos( const FVector2F & vPos_ )
{
	vPos = vPos_;
}

void FText::SetColor( const FColor4F & cColor )
{
	cTextColor = cColor;
	cTextColor.a = fAlpha;
}

void FText::ChangeFont( const FString & sFontName )
{
	if( lpFont )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpFont );
	lpFont = (FFont *)FGraphObjectManager::GetInstance()->CreateObject( sFontName, FGraphObjectManager::OBJECT_FONT );

	if( !lpFont )
		return;

	iPrimCount = lpFont->GetTextPrimCount( sText.GetChar() );
	iDataSize = iPrimCount * lpFont->GetCharDataSize();
	iVertCount = iPrimCount * lpFont->GetCharVertCount();
	iIndCount = iPrimCount * lpFont->GetCharIndCount();

	FRect rRect;
	lpFont->GetTextRect( sText.GetChar(), &rRect, fFontSize );
	fTextWidth = rRect.fRight;
	fTextHeight = rRect.fTop;
}

void FText::SetFontSize( F32 fFontSize_ )
{
	fFontSize = fFontSize_;
}

FString FText::GetText()const
{
	return sText;
}

FVector2F FText::GetPos()const
{
	return vPos;
}

FColor4F FText::GetColor()const
{
	return cTextColor;
}

F32 FText::GetFontSize()const
{
	return fFontSize;
}

F32 FText::GetTextWidth()const
{
	return fTextWidth;
}

F32 FText::GetTextHeight()const
{
	return fTextHeight;
}

F32 FText::GetScaleTextWidth()const
{
	return fTextWidth*fScale;
}

F32 FText::GetScaleTextHeight()const
{
	return fTextHeight*fScale;
}

//FObject * FText::CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs )
//{
//	CHECK_OBJ_TYPES( lpTypes, lpContrTypes );
//
//	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
//	FColor4F * lpColor = va_arg( lpArgs, FColor4F * );
//	F32 fFontSize = va_arg( lpArgs, F32 );
//	FString * lpText = va_arg( lpArgs, FString * );
//	FString * lpFontName = va_arg( lpArgs, FString * );
//	UI32 iLayer = va_arg( lpArgs, UI32 );
//	FScene * lpScene = va_arg( lpArgs, FScene * );
//
//	return new (lpPlacement) FText( *lpPos, *lpColor, fFontSize, lpText, lpFontName, iLayer, lpScene );
//}