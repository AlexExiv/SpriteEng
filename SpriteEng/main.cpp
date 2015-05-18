#include <Windows.h>
#include <windowsx.h>
#include "Game\FArithGame.h"
#include "2D\FView.h"
#include "Game\FGame.h"
#include "Core\FException.h"
#include "Core\FLog.h"
#include <gl\GL.h>
#include "Core\FStack.h"



#define FIELD_WIDTH 320
#define FIELD_HEIGHT 480

static FArithGame * lpGame = NULL;

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

UI32 InitPixelFormat( HDC hDC )
{
	I32 iPixelFormat;
	PIXELFORMATDESCRIPTOR pfd = {0};

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;

	iPixelFormat = ChoosePixelFormat( hDC, &pfd );
	if( !iPixelFormat )
	{
	//Error: ChoosePixelFormat failed
		return 0;
	}

	if( !SetPixelFormat( hDC, iPixelFormat, &pfd ))
	{
	//Error: SetPixelFormat failed"
		return 0;
	}

	DescribePixelFormat( hDC, iPixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &pfd );

	return 1;
}
void InitGL( HWND hWnd )
{
	HDC hDC = GetWindowDC( hWnd );
	if( !InitPixelFormat( hDC ) )
		return ;

	HGLRC hGLrc = wglCreateContext( hDC );
	wglMakeCurrent( hDC, hGLrc );

	glHint    ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	//glHint( GL_GENERATE_MIPMAP_HINT_SGIS, GL_FASTEST );
	glEnable        ( GL_DEPTH_TEST );							// enable z-buffering
	glEnable        ( GL_BLEND );
	glDisable( GL_CULL_FACE );
	glDepthFunc     ( GL_LEQUAL );								// set depth comparison (<=)
	glBlendFunc     ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	//glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &iMaxAniso );
	//iCurAniso = 0;
}

void ReleaseGL( HWND hWnd )
{
	HDC hDC = GetWindowDC( hWnd );
	HGLRC hGLrc = wglGetCurrentContext();
	if( hGLrc )
	{
		wglMakeCurrent( hDC, 0 );
		wglDeleteContext( hGLrc );
		hGLrc = NULL;
	}
	ReleaseDC( hWnd, hDC );
}


void TestStrings()
{
	FString sOne( "ONE" );
	FString sTwo( "TWO" );

	FString sOneTwo = sOne + sTwo;
	FString sPrint = FString::PrintString( "ONE + TWO = %s", sOneTwo.GetChar() );

	if( sOne == FString( "ONE" ) )
		return;

}

int WINAPI WinMain(	HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow )
{
	HWND hWnd;
	WNDCLASSEX wWndClass;
	ZeroMemory( &wWndClass, sizeof( wWndClass ) );
	wWndClass.cbSize = sizeof( wWndClass );
	wWndClass.style = CS_HREDRAW | CS_VREDRAW;
	wWndClass.lpfnWndProc = WndProc;
	wWndClass.hInstance = hInst;
	wWndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wWndClass.lpszClassName = "Arithmetic Numbers";

	if( RegisterClassEx( &wWndClass ) == 0 )
		return 0;

	hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "Arithmetic Numbers", "Game", WS_POPUP, 0, 0,
		FIELD_WIDTH, FIELD_HEIGHT, NULL, NULL, hInst, NULL );
	if( hWnd == 0 )
		return 0;

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	FLog::Init();
	TestStrings();
	InitGL( hWnd );
	try
	{
		FGame::InitMeta();
		FView::Init( FIELD_WIDTH, FIELD_HEIGHT );

		lpGame = new FArithGame();

		MSG mMsg;

		UI32 iOldTick = GetTickCount();
		while( true )
		{
			if( PeekMessage( &mMsg, NULL, 0, 0, PM_NOREMOVE ) )
			{
				if( !GetMessage( &mMsg, NULL, 0, 0 ) )
					return mMsg.wParam;

				TranslateMessage( &mMsg );
				DispatchMessage( &mMsg );
			}
			UI32 iTickCount = GetTickCount();
			if( iTickCount - iOldTick < 1000/60 )
			{
				Sleep( 1000/60 - (iTickCount - iOldTick) );
				iTickCount = GetTickCount();
			}
			F32 fDTime = F32(iTickCount - iOldTick)/30.f;
			iOldTick = iTickCount;

			lpGame->Update( fDTime );

			HDC hDC = GetWindowDC( hWnd );
			//HGLRC hGLrc = wglGetCurrentContext();
			//wglMakeCurrent( hDC, hGLrc  );
			
			lpGame->Draw();

			SwapBuffers( hDC );
			ReleaseDC( hWnd, hDC );

			CHECK_STACK;
		}
	}
	catch( FException eExcp )
	{
	}

	delete lpGame;
	ReleaseGL( hWnd );
	FLog::Destroy();
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_MOUSEMOVE:
		lpGame->ClickMove( FVector2F( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ),  0 );
		break;
	case WM_LBUTTONDOWN:
		lpGame->ClickDown( FVector2F( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ), 0 );
		break;
	case WM_LBUTTONUP:
		lpGame->ClickUp( FVector2F( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ), 0 );
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	};
	return DefWindowProc( hWnd, iMsg, wParam, lParam );
}