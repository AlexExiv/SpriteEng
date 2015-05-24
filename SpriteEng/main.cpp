#include <Windows.h>
#include <windowsx.h>
#include "Game\FArithGame.h"
#include "2D\FView.h"
#include "Game\FGame.h"
#include "Core\FException.h"
#include "Core\FLog.h"
#include <gl\GL.h>
#include "2D\glext.h"
#include "Core\FStack.h"



#define FIELD_WIDTH 320
#define FIELD_HEIGHT 480

static FArithGame * lpGame = NULL;

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );


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
	try
	{
		FGame::InitMeta();
		FWndAdd sWndAdd = { hWnd };
		FView::Init( FView::F_API_OPENGL, FIELD_WIDTH, FIELD_HEIGHT, &sWndAdd );

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
			lpGame->Draw();

			FView::GetMainView()->SwapBuffers();

			CHECK_STACK;
		}
	}
	catch( FException eExcp )
	{
	}

	delete lpGame;
	FLog::Destroy();
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_MOUSEMOVE:
		lpGame->ClickMove( FVector2F( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ),  0, FGame::GAME_INVERT_Y );
		break;
	case WM_LBUTTONDOWN:
		lpGame->ClickDown( FVector2F( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ), 0, FGame::GAME_INVERT_Y );
		break;
	case WM_LBUTTONUP:
		lpGame->ClickUp( FVector2F( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ), 0, FGame::GAME_INVERT_Y );
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_KEYDOWN:
		switch( wParam )
		{
		case 0x46:
			FString::DumpStringTable();
			break;
		};
		return 0;
	};
	return DefWindowProc( hWnd, iMsg, wParam, lParam );
}