//========================================
// *** main.h ***
//========================================

#ifndef _MAIN_H_
#define _MAIN_H_             

#include<windows.h>
#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include <assert.h>
#include"XInput.h"
#include <iostream>	// 外部の装置と入出力
#include <fstream>	// ファイル の入出力
#include <vector>
#include <string>
#include <sstream>	// カンマで区切るために必要

using namespace std;
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include"d3dx9.h"  //描画処理に必要
#define DIRECTINPUT_VERSION (0X0800) //ビルド時の警告対処用のマクロ
#include"dinput.h"//入力処理に必要//２重インクルード帽子のマクロを定義する

//#ifdef _DEBUG
//#define   new                   new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif
//========================================
// ライブラリ
//========================================

#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻所得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment(lib,"xinput.lib")	//入力処理に必要
#pragma comment(lib,"Opengl32.lib")	//フォグ処理に必要
#include"xaudio2.h"					//サウンド処理に必要
//========================================
// マクロ定義
//========================================
#define SCREEN_WIDTH		(1280.0f)														// ウインドウの幅
#define SCREEN_HEIGHT		(720.0f)														// ウインドウの高さ
#define SWITCH_TIME			(600)															// 切り替わる時間(1秒 = 60)
#define INIT_FLOAT			(0.0f)															// floatの初期値
#define INIT_VEC			D3DXVECTOR3(0.0f,1.0f,0.0f)										// VECの初期化
#define INIT_D3DXVECTOR2	D3DXVECTOR2(0.0f,0.0f)											// D3DXVECTOR2の初期値
#define INIT_D3DXVECTOR3	D3DXVECTOR3(0.0f,0.0f,0.0f)										// D3DXVECTOR3の初期値
#define INIT_POSITIONVEC4	PositionVec4(0.0f,1.0f,0.0f,1.0f)								// PositionVec4の初期値
#define INIT_D3DXCOLOR		D3DXCOLOR(1,1,1,1)												// D3DXCOLORの初期値
#define TXT_MAX				(256)															// 文字列の最大数(汎用)


#ifdef _DEBUG

#define WINDOW_MODE			(false)								// ウインドウモードの切り替え

#else

#define WINDOW_MODE			(true)								// ウインドウモードの切り替え

#endif // _DEBUG
//========================================
// 情報構造体
//========================================

struct PositionVec4
{
	float     P0, P1, P2, P3;
	constexpr PositionVec4() : P0(0.0f), P1(0.0f), P2(0.0f), P3(0.0f) { }
	constexpr PositionVec4(float _P0, float _P1, float _P2, float _P3) : P0(_P0), P1(_P1), P2(_P2), P3(_P3) { }
};

// メイン処理の情報構造体
typedef struct
{
	HWND				hWnd;		// ウインドウハンドル(識別子)
	HINSTANCE			hinstance;	// インスタンスハンドル
	D3DXVECTOR3			windowPos;	// 位置
}Main;

//========================================
// プロトタイプ宣言
//========================================

/* ウインドウのモード	*/void WindowMode(bool bScreen);
/* ウインドウハンドル	*/HWND *GetWindowHandle(void);
/* インスタンスハンドル	*/HINSTANCE *GetHinstance(void);
/* ウインドウ位置		*/D3DXVECTOR3 GetWindowPos(void);
/* FPS					*/int GetFps(void);
#endif