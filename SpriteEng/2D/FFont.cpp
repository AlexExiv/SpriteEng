#include "FFont.h"
#include "..\Core\FColor.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FImageResource.h"
#include "..\Core\FResourceManager.h"
#include "..\Core\FLog.h"
#include "..\Core\FFile.h"
#include "FView.h"
#include "..\Core\FStack.h"
#include "..\Core\FException.h"
#include "FShader.h"
#include "FTexture.h"
#include <string.h>
#include "..\Math\FVector2D.h"



#define FONT_DATA_CONT_COUNT 3

struct FFontVertexData
{
	FVector2F vPoint;
	FVector2F vTexCoord;
	FColor4F cColor;
};


const FString sFontShaderName( "bundle\\font.shr" );
const FString sFontAlphaBlock( "tAlphaTex" );

const FDrawDataCont dFontDataCont[FONT_DATA_CONT_COUNT] =
{
	{ FView::VERTEX_DATA, 2, FView::DATA_FLOAT, sizeof( FFontVertexData ), FIELD_OFFSET_( FFontVertexData, vPoint ) },
	{ FView::TEXCOORD_DATA, 2, FView::DATA_FLOAT, sizeof( FFontVertexData ), FIELD_OFFSET_( FFontVertexData, vTexCoord ) },
	{ FView::COLOR_DATA, 4, FView::DATA_FLOAT, sizeof( FFontVertexData ), FIELD_OFFSET_( FFontVertexData, cColor ) }
};



struct FFontInfo
{
    UI16 iFontSize;
    UI8 iBitField;
    UI8 iCharSet;
    UI16 iStretchH;
    UI8 iAa;
    UI8 iPaddingUp, iPaddingRight, iPaddingDown, iPaddingLeft;
    UI8 iSpacingH, iSpacingV;
    UI8 iOutline;
    CHAR_ cFontName[1];
};

struct FFontCommon
{
    UI16 iLineHeight;
    UI16 iBase;
    UI16 iScaleW, iScaleH;
    UI16 iPages;
    UI8 iBitFields;
    UI8 iAlphaC, iRedC, iGreenC, iBlueC;
};

struct FFontPages
{
    CHAR_ cPageNames[1];
};

struct FFontChar
{
    UI32 iID;
    UI16 iX, iY;
    UI16 iWidth, iHeight;
    UI16 iXOff, iYOff;
    UI16 iXAdv;
    UI8 iChnl;
};

struct FFontKerning
{
    UI32 iFirstID, iSecondID;
    UI16 iAmmount;
};



FFont::FFont( const FString & sFontName ) : FGraphObject( sFontName, FGraphObjectManager::OBJECT_FONT ),
	lpTexture( NULL ), lpShader( NULL ), iAlphaBlock( 0 )
{    
	FFile * lpFile = NULL;
	FImageResource * lpImg = NULL;
	UI8 * lpData = NULL;

	memset( cAlphabit, 0, sizeof( cAlphabit ) );
	try
	{
		lpFile = FFile::OpenFile( sFontName, FFile::F_FILE_READ );
		if( !lpFile )
		{
			FLog::PutError( "Can't load font data file, name: %s", sFontName.GetChar() );
			throw FException( FException::EXCP_FILE_NOT_FOUND, FString::PrintString( "%s", sFontName.GetChar() ) );
		}

		UI32 iFileSize = lpFile->GetSize();
		lpData = (UI8 *)PUSH_BLOCKT( iFileSize );
		lpFile->Read( lpData, iFileSize );
		UI8 * lpSrc = lpData + 4;
		FString sFileName;
    
		for( I32 i = 4;i < iFileSize; )
		{
			UI8 iBlockType = *lpSrc++;
			UI32 iBlockSize = *(UI32 *)lpSrc;
			lpSrc += 4;
        
			switch ( iBlockType )
			{
				case 1:
					lpSrc += LoadInfo( iBlockSize, lpSrc );
					break;
				case 2:
					lpSrc += LoadCommon( iBlockSize, lpSrc);
					break;
				case 3:
					lpSrc += LoadPages( iBlockSize, lpSrc, sFileName );
					lpImg = (FImageResource *)FResourceManager::SharedManager()->CreateResource( sFileName );
					if( !lpImg )
					{
						FLog::PutError( "Can't load font image, name: %s", sFileName.GetChar() );
						throw FException( FException::EXCP_FILE_NOT_FOUND, FString::PrintString( "%s", sFileName.GetChar() ) );
					}
					break;
				case 4:
					lpSrc += LoadChars( iBlockSize, lpSrc, lpImg->GetWidth(), lpImg->GetHeight());
					break;
				case 5:
					lpSrc += LoadKerning( iBlockSize, lpSrc);
					break;
                
				default:
					break;
			}
        
			i += (iBlockSize + 5);
		}

		lpTexture = FGraphObjectManager::GetInstance()->CreateTexture( sFileName + FString( ".tex" ), lpImg );
		lpShader = (FShader *)FGraphObjectManager::GetInstance()->CreateObject( sFontShaderName, FGraphObjectManager::OBJECT_SHADER );
		if( !lpShader )
			throw FException( FException::EXCP_FILE_NOT_FOUND, FString( "Can't create font object not all component are exist" ) );
		iAlphaBlock = lpShader->GetTextureBlock( sFontAlphaBlock );

		POP_BLOCK;
		delete lpFile;
		delete lpImg;
	}
	catch( FException eExcp )
	{
		if( lpData )
			POP_BLOCK;
		if( lpFile )
			delete lpFile;
		if( lpImg )
			delete lpImg;
		//this->~FFont();
		FGraphObjectManager::GetInstance()->ReleaseObject( lpShader );
		FGraphObjectManager::GetInstance()->ReleaseObject( lpTexture );

		throw eExcp;
	}
}

FFont::~FFont()
{
	FGraphObjectManager::GetInstance()->ReleaseObject( lpShader );
	FGraphObjectManager::GetInstance()->ReleaseObject( lpTexture );
}

UI32 FFont::LoadInfo( UI32 iBlockSize, UI8 * lpData )
{
    return iBlockSize;
}

UI32 FFont::LoadCommon( UI32 iBlockSize, UI8 * lpData )
{
    return iBlockSize;
}

UI32 FFont::LoadPages( UI32 iBlockSize, UI8 * lpData, FString & sFileName_ )
{
	FFontPages * lpPage = (FFontPages *)lpData;
	FString sPath = GetName().GetPath();
	sFileName_ = sPath + lpPage->cPageNames;

    return iBlockSize;
}

UI32 FFont::LoadChars( UI32 iBlockSize, UI8 * lpData, F32 fWidth, F32 fHeight )
{
    FFontChar * lpChars = (FFontChar *)lpData;
    for( I32 i = 0;i < iBlockSize/sizeof( FFontChar );i++ )
    {
        UI8 iID = lpChars[i].iID;
        cAlphabit[iID].u0 = (F32)lpChars[i].iX/fWidth;
        cAlphabit[iID].v0 = (F32)lpChars[i].iY/fHeight;
        cAlphabit[iID].u1 = (F32)(lpChars[i].iX + lpChars[i].iWidth)/fWidth;
        cAlphabit[iID].v1 = (F32)(lpChars[i].iY + lpChars[i].iHeight)/fHeight;
        cAlphabit[iID].fHeight = 1.f;
        cAlphabit[iID].fWidth = (F32)lpChars[i].iWidth/(F32)lpChars[i].iHeight;
		cAlphabit[iID].cChar = TOCHARA( iID );
    }
    
    return iBlockSize;
}

UI32 FFont::LoadKerning( UI32 iBlockSize, UI8 * lpData )
{
    return iBlockSize;
}

F32 FFont::GetCharHeight( CHAR_ cChar, F32 fFontSize )const
{
	if( cAlphabit[TOUCHARA( cChar )].cChar == 0 )
		return 0.f;

	return cAlphabit[TOUCHARA( cChar )].fHeight*fFontSize;
}

F32 FFont::GetCharWidth( CHAR_ cChar, F32 fFontSize )const
{
	if( cAlphabit[TOUCHARA( cChar )].cChar == 0 )
		return 0.f;

	return cAlphabit[TOUCHARA( cChar )].fWidth*fFontSize;
}

void FFont::GetTextRect( const CHAR_ * lpText, FRect * lpRect, F32 fFontSize )
{
	F32 x = 0.f, fXMax = 0.f, y = 0.f, fMaxY = 0.f;

	for( UI32 i = 0;lpText[i] != 0;i++ )
	{
		if( lpText[i] == '\n' )
		{
			fXMax = max( fXMax, x );
			x = 0.f;
			y += fMaxY;
			fMaxY = 0.f;
#ifdef WINDOWS_FAMILY
			i++;
#endif
			continue;
		}
		if( cAlphabit[TOUCHARA( lpText[i] )].cChar == 0 )
			continue;

		F32 fWidth = cAlphabit[TOUCHARA( lpText[i] )].fWidth*fFontSize;
		F32 fHeight = cAlphabit[TOUCHARA( lpText[i] )].fHeight*fFontSize;
		fMaxY = max( fMaxY, fHeight );
		x += fWidth;
	}

	lpRect->fRight = lpRect->fLeft + fXMax;
	lpRect->fTop = lpRect->fBottom + y;
}

void FFont::DrawInLine( const FVector2F & vPos, const CHAR_ * lpString, F32 fFontSize, const FColor4F & cColor, void * lpOutput, UI32 * lpIndeces, UI32 iStartVert )
{
	const CHAR_ * lpSym = lpString;
	FFontVertexData * lpVertData = (FFontVertexData *)lpOutput;
	UI32 * lpDstInd = lpIndeces;
	F32 fXSym = vPos.x, fCharHeight = GetCharHeight( ' ', fFontSize ), fYSym = vPos.y - fCharHeight;

	while( *lpSym != 0 )
	{
		if( (*lpSym) == '\n' )
		{
			fXSym = vPos.x;
			fYSym -= fCharHeight;
			lpSym++;
#ifdef WINDOWS_FAMILY
			lpSym++;
#endif

			continue;
		}

		UI8 iSym = TOUCHARA( *lpSym );
		FChar * lpChar = &cAlphabit[iSym];
		if( lpChar->cChar == 0 )
		{
			lpSym++;
			continue;
		}

		F32 fWidth = lpChar->fWidth*fFontSize;
		F32 fHeight = lpChar->fHeight*fFontSize;


		//3--2
		//|\ |
		//| \|
		//0--1
		//
		//0
		lpVertData->vPoint = FVector2F( fXSym, fYSym );
		lpVertData->vTexCoord = FVector2F( lpChar->u0, lpChar->v1 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//1
		lpVertData->vPoint = FVector2F( fXSym + fWidth, fYSym );
		lpVertData->vTexCoord = FVector2F( lpChar->u1, lpChar->v1 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//2
		lpVertData->vPoint = FVector2F( fXSym + fWidth, fYSym + fHeight );
		lpVertData->vTexCoord = FVector2F( lpChar->u1, lpChar->v0 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//3
		lpVertData->vPoint = FVector2F( fXSym, fYSym  + fHeight );
		lpVertData->vTexCoord = FVector2F( lpChar->u0, lpChar->v0 );
		lpVertData->cColor = cColor;
		lpVertData++;

		lpDstInd[0] = iStartVert;
		lpDstInd[1] = iStartVert + 1;
		lpDstInd[2] = iStartVert + 3;

		lpDstInd[3] = iStartVert + 1;
		lpDstInd[4] = iStartVert + 2;
		lpDstInd[5] = iStartVert + 3;

		lpDstInd += 6;
		lpSym++;
		fXSym += fWidth;
	}
}

void FFont::DrawInRect( const FRect & rOutputRect, const CHAR_ * lpText, F32 fFontSize, const FColor4F & cColor, void * lpOutput, UI32 * lpIndeces, UI32 iStartVert )
{
	F32 fXSym = rOutputRect.fLeft,
		fCharHeight = GetCharHeight( ' ', fFontSize ),
		fYSym = rOutputRect.fTop - fCharHeight;
	const CHAR_ * lpSym = lpText;
	FFontVertexData * lpVertData = (FFontVertexData *)lpOutput;
	UI32 * lpDstInd = lpIndeces;

	while( *lpSym != 0 )
	{
		if( (*lpSym) == '\n' )
		{
			fXSym = rOutputRect.fLeft;
			fYSym -= fCharHeight;
#ifdef WINDOWS_FAMILY
			lpSym++;
#endif

			continue;
		}

		UI8 iSym = TOUCHARA( *lpSym );
		FChar * lpChar = &cAlphabit[iSym];
		if( lpChar->cChar == 0 )
			continue;
		F32 fWidth = lpChar->fWidth*fFontSize;
		F32 fHeight = lpChar->fHeight*fFontSize;

		if( (fXSym + fWidth) >= rOutputRect.fRight )
		{
			fXSym = rOutputRect.fLeft;
			fYSym -= fCharHeight;
		}
		if( fYSym <= rOutputRect.fBottom )
			break;

		//3--2
		//|\ |
		//| \|
		//0--1
		//
		//0
		lpVertData->vPoint = FVector2F( fXSym, fYSym );
		lpVertData->vTexCoord = FVector2F( lpChar->u0, lpChar->v1 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//1
		lpVertData->vPoint = FVector2F( fXSym + fWidth, fYSym );
		lpVertData->vTexCoord = FVector2F( lpChar->u1, lpChar->v1 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//2
		lpVertData->vPoint = FVector2F( fXSym + fWidth, fYSym + fHeight );
		lpVertData->vTexCoord = FVector2F( lpChar->u1, lpChar->v0 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//3
		lpVertData->vPoint = FVector2F( fXSym, fYSym  + fHeight );
		lpVertData->vTexCoord = FVector2F( lpChar->u0, lpChar->v0 );
		lpVertData->cColor = cColor;
		lpVertData++;


		lpDstInd[0] = iStartVert;
		lpDstInd[1] = iStartVert + 1;
		lpDstInd[2] = iStartVert + 3;

		lpDstInd[3] = iStartVert + 1;
		lpDstInd[4] = iStartVert + 2;
		lpDstInd[5] = iStartVert + 3;

		lpDstInd += 6;
		lpSym++;
		fXSym += fWidth;
	}
}

void FFont::DrawInRectStart( const FVector2F & vPos, const FRect & rOutputRect, const CHAR_ * lpText, F32 fFontSize, const FColor4F & cColor, void * lpOutput, UI32 * lpIndeces, UI32 iStartVert )
{
	F32 fXSym = rOutputRect.fLeft,
		fCharHeight = GetCharHeight( ' ', fFontSize ),
		fYSym = rOutputRect.fTop - fCharHeight;
	const CHAR_ * lpSym = lpText;
	FFontVertexData * lpVertData = (FFontVertexData *)lpOutput;
	UI32 * lpDstInd = lpIndeces;

	while( *lpSym != 0 )
	{
		if( (*lpSym) == '\n' )
		{
			fXSym = rOutputRect.fLeft;
			fYSym -= fCharHeight;
#ifdef WINDOWS_FAMILY
			lpSym++;
#endif

			continue;
		}

		UI8 iSym = TOUCHARA( *lpSym );
		FChar * lpChar = &cAlphabit[iSym];
		if( lpChar->cChar == 0 )
			continue;

		F32 fWidth = lpChar->fWidth*fFontSize;
		F32 fHeight = lpChar->fHeight*fFontSize;

		if( (fXSym + fWidth) >= rOutputRect.fRight )
		{
			fXSym = rOutputRect.fLeft;
			fYSym -= fCharHeight;
		}
		if( fYSym <= rOutputRect.fBottom )
			break;

		//3--2
		//|\ |
		//| \|
		//0--1
		//
		//0
		lpVertData->vPoint = FVector2F( fXSym, fYSym );
		lpVertData->vTexCoord = FVector2F( lpChar->u0, lpChar->v1 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//1
		lpVertData->vPoint = FVector2F( fXSym + fWidth, fYSym );
		lpVertData->vTexCoord = FVector2F( lpChar->u1, lpChar->v1 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//2
		lpVertData->vPoint = FVector2F( fXSym + fWidth, fYSym + fHeight );
		lpVertData->vTexCoord = FVector2F( lpChar->u1, lpChar->v0 );
		lpVertData->cColor = cColor;
		lpVertData++;
		//3
		lpVertData->vPoint = FVector2F( fXSym, fYSym  + fHeight );
		lpVertData->vTexCoord = FVector2F( lpChar->u0, lpChar->v0 );
		lpVertData->cColor = cColor;
		lpVertData++;


		lpDstInd[0] = iStartVert;
		lpDstInd[1] = iStartVert + 1;
		lpDstInd[2] = iStartVert + 3;

		lpDstInd[3] = iStartVert + 1;
		lpDstInd[4] = iStartVert + 2;
		lpDstInd[5] = iStartVert + 3;

		lpDstInd += 6;
		lpSym++;
		fXSym += fWidth;
	}
}

UI32 FFont::GetDrawContCount()const
{
	return FONT_DATA_CONT_COUNT;
}

const FDrawDataCont * FFont::GetDrawDataCont()const
{
	return dFontDataCont;
}

UI32 FFont::GetCharDataSize()const
{
	return GetCharVertCount()*sizeof( FFontVertexData );
}

UI32 FFont::GetCharIndCount()const
{
	return 6;
}

UI32 FFont::GetCharVertCount()const
{
	return 4;
}

UI32 FFont::GetTextPrimCount( const CHAR_ * lpText )const
{
	UI32 iPrimCount = 0;
	for( UI32 i = 0;lpText[i] != 0;i++ )
	{
		UI8 iSym = TOUCHARA( lpText[i] );
		if( cAlphabit[iSym].cChar == 0 )
			continue;
		if( cAlphabit[iSym].cChar == '\r' || cAlphabit[iSym].cChar == '\n' )
			continue;
		iPrimCount++;
	}

	return iPrimCount;
}

void FFont::Bind()
{
	if( lpShader )
		lpShader->Bind();
	if( lpTexture )
		lpTexture->Bind( iAlphaBlock );
}

void FFont::UnBind()
{
	if( lpTexture )
		lpTexture->UnBind( iAlphaBlock );
	if( lpShader )
		lpShader->UnBind();
}


/*
void FFont::DrawString( F32 x, F32 y, const CHAR_ * lpString )
{
    SetPos( x, y );
    PutLine( lpString );
}

void FFont::SetFontColor( const FColor4F & cColor )
{
	lpShader->SetColor( sFontColor, cColor );
}

void FFont::SetClipRect( FRect rClipRect0 )
{
    rClipRect = rClipRect0;
}

void FFont::SetClipByRect( I32 iClipByRect0 )
{
    iClipByRect = iClipByRect0;
}

void FFont::SetPos( F32 x0, F32 y0 )
{
    xs = x0;
    ys = y0;
    x = x0;
    y = y0;
}

void FFont::SetFontSize( F32 fFontSize0 )
{
    fFontSize = fFontSize0;
}

F32 FFont::GetCharHeight( CHAR_ cChar, F32 fFontSize )const
{
    return fFontSize * cAlphabit[TOUCHARA( cChar )].fHeight;
}

F32 FFont::GetCharWidth(CHAR_ cChar, F32 fFontSize )const
{
    return fFontSize * cAlphabit[TOUCHARA( cChar )].fWidth*1.1f;
}

F32 FFont::GetStringWidth( F32 fFontSize, const FString & sStr )
{
    F32 fLen = 0.f;
    CHAR_ * lpStr = sStr.GetChar();
    
    for( I32 i = 0;i < sStr.Length();i++ )
    {
        fLen += cAlphabit[TOUCHARA( lpStr[i] )].fWidth*fFontSize*1.1f;
        if( lpStr[i] == ' ' )
            fLen += cAlphabit[TOUCHARA( lpStr[i] )].fWidth*fFontSize*1.1f;
    }
    
    return fLen;
}

void FFont::PutLine( const CHAR_ * lpString )
{
    Vertex2D vVertices[MAX_BUFFER], vTexCoords[MAX_BUFFER];
    UI32 iIndeces[(MAX_BUFFER/4 - 1)*6];
    I32 iVertCount = 0, iIndexCount = 0;;
    
    lpDraw->BindTexture( iTexID );
    lpDraw->TexOp( FDraw::TEXTURE_ADD );
    
    for( I32 i = 0;i < strlen( lpString );i++, iVertCount += 4, iIndexCount += 6 )
    {
        if( iVertCount == MAX_BUFFER )
        {
            lpDraw->DrawArray( (F32 *)&vVertices[0], (F32 *)&vTexCoords[0], (UI32 *)&iIndeces[0], iIndexCount, cFontColor );
            iVertCount = 0;
            iIndexCount = 0;
        }
        
        vVertices[iVertCount].x = x;
        vVertices[iVertCount].y = y;
        vTexCoords[iVertCount].x = cAlphabit[TOUCHARA( lpString[i] )].u0;
        vTexCoords[iVertCount].y = cAlphabit[TOUCHARA( lpString[i] )].v1;
        
        vVertices[iVertCount + 1].x = x + cAlphabit[TOUCHARA( lpString[i] )].fWidth*fFontSize;
        vVertices[iVertCount + 1].y = y;
        vTexCoords[iVertCount + 1].x = cAlphabit[TOUCHARA( lpString[i] )].u1;
        vTexCoords[iVertCount + 1].y = cAlphabit[TOUCHARA( lpString[i] )].v1;
        
        vVertices[iVertCount + 2].x = x + cAlphabit[TOUCHARA( lpString[i] )].fWidth*fFontSize;
        vVertices[iVertCount + 2].y = y + cAlphabit[TOUCHARA( lpString[i] )].fHeight*fFontSize;
        vTexCoords[iVertCount + 2].x = cAlphabit[TOUCHARA( lpString[i] )].u1;
        vTexCoords[iVertCount + 2].y = cAlphabit[TOUCHARA( lpString[i] )].v0;
        
        vVertices[iVertCount + 3].x = x;
        vVertices[iVertCount + 3].y = y + cAlphabit[TOUCHARA( lpString[i] )].fHeight*fFontSize;
        vTexCoords[iVertCount + 3].x = cAlphabit[TOUCHARA( lpString[i] )].u0;
        vTexCoords[iVertCount + 3].y = cAlphabit[TOUCHARA( lpString[i] )].v0;
        
        iIndeces[iIndexCount] = iVertCount;
        iIndeces[iIndexCount + 1] = iVertCount + 1;
        iIndeces[iIndexCount + 2] = iVertCount + 2;
        iIndeces[iIndexCount + 3] = iVertCount;
        iIndeces[iIndexCount + 4] = iVertCount + 3;
        iIndeces[iIndexCount + 5] = iVertCount + 2;
        
        x += cAlphabit[TOUCHARA( lpString[i] )].fWidth*fFontSize*1.1f;
        if( lpString[i] == ' ' )
            x += cAlphabit[TOUCHARA( lpString[i] )].fWidth*fFontSize*1.1f;
    }
    
    if( iVertCount != 0 )
    {
        lpDraw->DrawArray( (F32 *)&vVertices[0], (F32 *)&vTexCoords[0], (UI32 *)&iIndeces[0], iIndexCount, cFontColor );
        iVertCount = 0;
    }
    
    lpDraw->BindTexture( 0 );

}

void FFont::PutInt( I32 iInt )
{
    CHAR_ cBuffer[16];
    itoa( iInt, cBuffer );
    PutLine( cBuffer );
}

void FFont::PutFloat( F32 fFloat )
{
    FString sFloat( fFloat );
    PutLine( sFloat );
}

void FFont::PutChar( CHAR_ cChar )
{
    CHAR_ cBuffer[2] = { cChar, 0 };
    PutLine( cBuffer );
}

void FFont::EndLine()
{
    y += fFontSize;
}


FFontManager::FFontManager( FDraw * lpDraw) : dFontDict( MAX_HASH ), lpDraw( lpDraw )
{
    
}

FFontManager::~FFontManager()
{
    
}

FFont * FFontManager::CreateFont( const FString & sFileName, bool bDel )
{
    FFont * lpFont = (FFont *)dFontDict.FindObj( sFileName );
    if( lpFont )
        return lpFont;
    
    lpFont = new FFont( sFileName, lpDraw, bDel );
    dFontDict.AddObject( lpFont );
    
    return lpFont;
}

void FFontManager::ReleaseFont( FFont * lpFont )
{
    dFontDict.ReleaseObject( lpFont );
}
*/