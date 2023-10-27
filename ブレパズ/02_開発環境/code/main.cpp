//========================================
// メインの処理
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "manager.h"
//========================================
// マクロ定義
//========================================
#define CLASS_NAME    "WindowClass"		//ウインドウクラスの名前
#define WINDOW_NAME   ""				//ウインドウの名前(キャプションに表示)

//========================================
// プロトタイプ宣言
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawFPS(void);

//========================================
// グローバル変数
//========================================
LPD3DXFONT g_pFont = NULL;						// フォントへのポインタ
int DisplayMode = 1;							// 操作方法表示　ON/OFF
int g_nCountFPS = 0;							// FPSカウンタ

Main	g_main;									// メイン処理の情報構造体

CManager *g_Physics = NULL;						// マネージャーオブジェクトへのポインタ

//========== *** ウインドウハンドルを取得 ***
HWND *GetWindowHandle(void)
{
	return &g_main.hWnd;
}

//========== *** インスタンスハンドルを取得 ***
HINSTANCE *GetHinstance(void)
{
	return &g_main.hinstance;
}

//========== *** ウインドウの位置を取得 ***
D3DXVECTOR3 GetWindowPos(void)
{
	return g_main.windowPos;
}

//========== *** FPSを取得 ***
int GetFps(void)
{
	return g_nCountFPS;
}

//========================================
// 情報の所得
//========================================

//メイン処理の情報
Main *GetMain(void)
{
	return &g_main;
}

//========================================
// メイン関数
//========================================
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	// メモリリーク ログを表示
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ウインドウクラスの構造体
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),																//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,																		//ウインドウのスタイル
		WindowProc,																		//ウインドウプロシージャ
		0,																				//0にする(通常は使用しない)
		0,																				//0にする(通常は使用しない)
		hinstance,																		//インスタンスハンドル
		(HICON)LoadImage(NULL,"data\\ICO\\icon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE),	//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),														//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),														//クライアント領域の背景色
		NULL,																			//メニューバー
		CLASS_NAME,																		//ウインドウクラスの名前
		(HICON)LoadImage(NULL,"data\\ICO\\icon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE),	//ファイルのアイコン
	};
	MSG msg;									  //メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//画面サイズの構造体
												   // ウインドウクラスの登録

	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウの生成
	g_main.hWnd = CreateWindowEx(0, // 拡張ウィンドウスタイル
		CLASS_NAME,					// ウインドウクラスの名前
		WINDOW_NAME,				// ウインドウの名前
		WS_OVERLAPPEDWINDOW,		// ウインドウスタイル
		CW_USEDEFAULT,				// ウインドウの左上X座標
		CW_USEDEFAULT,				// ウインドウの左上Y座標
		SCREEN_WIDTH,				// ウインドウの幅
		SCREEN_HEIGHT,				// ウインドウの高さ
		NULL,						// 親ウインドウのハンドル
		NULL,						// メニューハンドルまたは子ウインドウID
		hinstance,					// インスタンスハンドル
		NULL);						// ウインドウ作成デー

	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	//ウインドウモードの切り替え
	WindowMode(WINDOW_MODE);

	if (g_Physics == NULL)
	{
		// マネージャーの生成
		g_Physics = new CManager;

		g_main.hinstance = hinstance;
		g_Physics->Init(g_main.hinstance, g_main.hWnd, TRUE);
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;				//初期化する
	dwExecLastTime = timeGetTime();	//現在時刻を所得
	dwFrameCount = 0;				//初期化する
	dwFPSLastTime = timeGetTime();	//現在時刻を所持

									//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//WM_QUITメッセージを受け取ったらメッセージループを抜ける

			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg); //仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);  //ウインドウプロシージャへメッセージを送出
			}

		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();//現在時刻を所得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//005秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;//FPSを測定した時刻を保存
				dwFrameCount = 0;//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過したら

				dwExecLastTime = dwCurrentTime;//処理開始の時刻(現在時刻)を保存

				RECT rec;	// 大きさを受け取るRECT構造体

				// ウインドウの情報を取得
				GetWindowRect(g_main.hWnd, &rec);

				// ウインドウの位置を設定
				g_main.windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);

				// マネージャーの更新処理
				g_Physics->Update();

				// マネージャーの描画処理
				g_Physics->Draw();

				dwFrameCount++;//フレームカウントを加算
			}
		}
	}

	if (g_Physics != NULL)
	{
		// マネージャーの終了処理
		g_Physics->Uninit();

		delete g_Physics;
		g_Physics = NULL;
	}

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	_CrtDumpMemoryLeaks();

	return(int)msg.wParam;
}

//========================================
// ウインドウプロシージャ
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY: //ウインドウ破棄もメッセージ

					 //WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroyメッセージを送る
			}
			else
			{
				return 0;
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroyメッセージを送る
		}
		else
		{
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);//既定の処理を返す
}

//========================================
// FPS処理
//========================================
void DrawFPS(void)
{
	RECT rect = { 0,230,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[521];

	//文字列に代入
	sprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
// ウインドウモードの切り替え
//========================================
void WindowMode(bool bScreen)
{
	HDC hDC			// デスクトップのハンドル
		= GetDC(GetDesktopWindow());
	int nScrSizeX	// デスクトップの画面サイズX
		= GetDeviceCaps(hDC, HORZRES);
	int nScrSizeY	// デスクトップの画面サイズY
		= GetDeviceCaps(hDC, VERTRES);

	// デスクトップのハンドルを手放す
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bScreen)
	{// 全画面フラグが真の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(g_main.hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(g_main.hWnd);
}
