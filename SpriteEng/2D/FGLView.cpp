#include "FGLView.h"
#include "FGLShader.h"
#include "FGLTexture.h"
#include "libExt.h"
#include <new.h>



FGLView::FGLView( UI32 iWidth, UI32 iHeight ) : FView( iWidth, iHeight ), iCurState( 0 )
{
	initExtensions();
	glViewport( 0, 0, iWidth, iHeight );
	glOrtho( 0.f, 1.f, 0.f, 1.f, 0.1f, 1.f );
}

FGLView::~FGLView()
{
}


void FGLView::LineWidth( F32 fWidth )
{
	glLineWidth( fWidth );
}

void FGLView::SetViewport( UI32 x, UI32 y, UI32 iWidth0, UI32 iHeight0 )
{
	glViewport( x, y, iWidth0, iHeight0 );
}

void FGLView::ResetViewport() 
{
	glViewport( 0, 0, iWidth, iHeight );
}

void FGLView::SetPrimData( const FDrawDataCont * lpContainers, UI32 iContCount ) 
{
	for( UI32 i = 0;i < iContCount;i++ )
	{
		GLenum eType;
		switch( lpContainers[i].iDataType )
		{
		case FView::DATA_FLOAT:
			eType = GL_FLOAT;
			break;
		case FView::DATA_INT32:
			eType = GL_INT;
			break;
		case FView::DATA_UINT32:
			eType = GL_UNSIGNED_INT;
			break;
		}

		switch( lpContainers[i].iAttribType )
		{
		case FView::VERTEX_DATA:
			glEnableClientState( GL_VERTEX_ARRAY );
			glVertexPointer( lpContainers[i].iCompCount, eType, lpContainers[i].iStride, lpContainers[i].lpData );
			eState[iCurState++] = GL_VERTEX_ARRAY;
			break;
		case FView::COLOR_DATA:
			glEnableClientState( GL_COLOR_ARRAY );
			glColorPointer( lpContainers[i].iCompCount, eType, lpContainers[i].iStride, lpContainers[i].lpData );
			eState[iCurState++] = GL_COLOR_ARRAY;
			break;
		case FView::TEXCOORD_DATA:
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glTexCoordPointer( lpContainers[i].iCompCount, eType, lpContainers[i].iStride, lpContainers[i].lpData );
			eState[iCurState++] = GL_TEXTURE_COORD_ARRAY;
			break;
		};
	}
}

void FGLView::SetPrimDataArr( const FDrawDataCont * lpContainers, UI32 iContCount, void * lpData ) 
{
	for( UI32 i = 0;i < iContCount;i++ )
	{
		GLenum eType;
		switch( lpContainers[i].iDataType )
		{
		case FView::DATA_FLOAT:
			eType = GL_FLOAT;
			break;
		case FView::DATA_INT32:
			eType = GL_INT;
			break;
		case FView::DATA_UINT32:
			eType = GL_UNSIGNED_INT;
			break;
		}

		switch( lpContainers[i].iAttribType )
		{
		case FView::VERTEX_DATA:
			glEnableClientState( GL_VERTEX_ARRAY );
			glVertexPointer( lpContainers[i].iCompCount, eType, lpContainers[i].iStride, (UI8 *)lpData + lpContainers[i].iDataStart );
			eState[iCurState++] = GL_VERTEX_ARRAY;
			break;
		case FView::COLOR_DATA:
			glEnableClientState( GL_COLOR_ARRAY );
			glColorPointer( lpContainers[i].iCompCount, eType, lpContainers[i].iStride, (UI8 *)lpData + lpContainers[i].iDataStart );
			eState[iCurState++] = GL_COLOR_ARRAY;
			break;
		case FView::TEXCOORD_DATA:
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glTexCoordPointer( lpContainers[i].iCompCount, eType, lpContainers[i].iStride, (UI8 *)lpData + lpContainers[i].iDataStart );
			eState[iCurState++] = GL_TEXTURE_COORD_ARRAY;
			break;
		};
	}
}

void FGLView::Draw( UI32 iPrimType, UI32 iPrimCount )
{
	GLenum ePrim;
	switch( iPrimType )
	{
	case FView::PRIM_POINTS:
		ePrim = GL_POINTS;
		break;
	case FView::PRIM_LINES:
		ePrim = GL_LINES;
		break;
	case FView::PRIM_LINES_STRIP:
		ePrim = GL_LINE_STRIP;
		break;
	case FView::PRIM_TRIANGLES:
		ePrim = GL_TRIANGLES;
		break;
	case FView::PRIM_TRIANGLES_FAN:
		ePrim = GL_TRIANGLE_FAN;
		break;
	case FView::PRIM_TRIANGLES_STRIP:
		ePrim = GL_TRIANGLE_STRIP;
		break;
	};

	glDrawArrays( ePrim, 0, iPrimCount );
	ResetStates();
}

void FGLView::DrawIndexed( UI32 iPrimType, UI32 iPrimCount, const void * lpIndeces ) 
{
	GLenum ePrim;
	switch( iPrimType )
	{
	case FView::PRIM_POINTS:
		ePrim = GL_POINTS;
		break;
	case FView::PRIM_LINES:
		ePrim = GL_LINES;
		break;
	case FView::PRIM_LINES_STRIP:
		ePrim = GL_LINE_STRIP;
		break;
	case FView::PRIM_TRIANGLES:
		ePrim = GL_TRIANGLES;
		break;
	case FView::PRIM_TRIANGLES_FAN:
		ePrim = GL_TRIANGLE_FAN;
		break;
	case FView::PRIM_TRIANGLES_STRIP:
		ePrim = GL_TRIANGLE_STRIP;
		break;
	};

	glDrawElements( ePrim, iPrimCount, GL_UNSIGNED_INT, lpIndeces );
	ResetStates();
}

void FGLView::BeginDraw()
{
	glClearColor( 1.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );
	glDisable( GL_DEPTH_TEST );
}

void FGLView::EndDraw()
{
	glFlush();
}

void FGLView::ResetStates()
{
	for( UI32 i = 0;i < iCurState;i++ )
		glDisableClientState( eState[i] );
	iCurState = 0;
}

FTexture * FGLView::CreateTexture( FGraphObject * lpPlacement, const FString & sName )
{
	return new (lpPlacement) FGLTexture( sName );
}

FTexture * FGLView::CreateTexture( FGraphObject * lpPlacement, const FString & sName, const FImageResource * lpImg )
{
	return new (lpPlacement ) FGLTexture( sName, lpImg );
}

FTexture * FGLView::CreateTexture( const FString & sName, UI32 iWdth, UI32 iHeight, UI32 iFormat )
{
	return new FGLTexture( sName, iWidth, iHeight, iFormat );
}

FShader * FGLView::CreateShader( FGraphObject * lpPlacement, const FString & sShaderName )
{
	return new (lpPlacement) FGLShader( sShaderName );
}

UI32 FGLView::GetTextureObjSize()const
{
	return sizeof( FGLTexture );
}

UI32 FGLView::GetShaderObjSize()const
{
	return sizeof( FGLShader );
}
