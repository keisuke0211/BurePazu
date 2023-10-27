//========================================
// 
// マネージャー処理
// 
//========================================
// *** manager.cpp ***
//========================================
#include "manager.h"
#include "system/renderer.h"
#include "system/camera.h"
#include "system/light.h"
#include "system/sound.h"
#include "system/texture.h"
#include "system/input.h"
#include "system\words\font.h"
#include "object\object.h"
#include "scene\title.h"
#include "scene\tutorial.h"
#include "scene\game.h"
#include "scene\ranking.h"
#include "scene\pause.h"
#include "scene\fade.h"

// 静的メンバ変数
CRenderer *CManager::m_pRenderer = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CSound *CManager::m_pSound = NULL;
CTexture *CManager::m_pTexture = NULL;
CKeyboard *CManager::m_InputKeyboard = NULL;
CMouse *CManager::m_InputMouse = NULL;
CJoypad *CManager::m_InputJoypad = NULL;
CFont *CManager::m_pFont = NULL;
CScene *CManager::m_pScene = NULL;
CScene::MODE CScene::m_mode = MODE_TITLE;
CTitle *CScene::m_pTitle = NULL;
CTutorial *CScene::m_pTutorial = NULL;
CGame *CScene::m_pGame = NULL;
CRanking *CScene::m_pRanking = NULL;
CPause *CScene::m_pPause = NULL;
CFade *CManager::m_pFade = NULL;

//========================================
// コンストラクタ
//========================================
CManager::CManager()
{
	
}

//========================================
// デストラクタ
//========================================
CManager::~CManager()
{

}

//========================================
// 初期化
//========================================
HRESULT CManager::Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow)
{
	// レンダラー
	m_pRenderer = new CRenderer;

	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	// キーボード
	m_InputKeyboard = new CKeyboard;

	if (FAILED(m_InputKeyboard->Init(hinstance, hWnd)))
	{
		return E_FAIL;
	}

	// マウス
	m_InputMouse = new CMouse;

	if (FAILED(m_InputMouse->Init(hinstance, hWnd)))
	{
		return E_FAIL;
	}

	// ジョイパット
	m_InputJoypad = new CJoypad;

	if (FAILED(m_InputJoypad->Init()))
	{
		return E_FAIL;
	}

	// カメラ
	m_pCamera = new CCamera;

	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	// ライト
	m_pLight = new CLight;

	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	// サウンド
	m_pSound = new CSound;
	m_pSound->Load();

	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}
	
	// テクスチャ
	if (m_pTexture == NULL)
	{
		m_pTexture = new CTexture;
		m_pTexture->Load();
	}

	m_pFont = NULL;
	// フォント
	if (m_pFont == NULL)
	{
		m_pFont = new CFont;
	}

	// フェード
	if (m_pFade == NULL)
	{
		m_pFade = new CFade;
		m_pFade->Init();
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CManager::Uninit(void)
{
	// シーン
	if (m_pScene != NULL)
	{
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = NULL;
	}

	// キーボード
	if (m_InputKeyboard != NULL)
	{
		m_InputKeyboard->Uninit();

		delete m_InputKeyboard;
		m_InputKeyboard = NULL;
	}

	// マウス
	if (m_InputMouse != NULL)
	{
		m_InputMouse->Uninit();

		delete m_InputMouse;
		m_InputMouse = NULL;
	}

	// ジョイパット
	if (m_InputJoypad != NULL)
	{
		m_InputJoypad->Uninit();

		delete m_InputJoypad;
		m_InputJoypad = NULL;
	}

	// レンダラー
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// ライト
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	// カメラ
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}

	// サウンド
	if (m_pSound != NULL)
	{
		m_pSound->StopSound();
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	// テクスチャ
	if (m_pTexture != NULL)
	{
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = NULL;
	}

	// フォント
	if (m_pFont != NULL)
	{
		m_pFont->ReleaseAll();

		delete m_pFont;
		m_pFont = NULL;
	}
}

//========================================
// 更新
//========================================
void CManager::Update(void)
{
	m_pScene->Update();				// シーン
	m_InputKeyboard->Update();		// キーボード
	m_InputMouse->Update();			// マウス
	m_InputJoypad->Update();		// ジョイパット(ボタン)
	m_InputJoypad->UpdateStick();	// ジョイパット(スティック)	
	m_pCamera->Update();			// カメラ
	m_pLight->Update();				// ライト
	m_pRenderer->Update();			// レンダラー
}

//========================================
// 描画
//========================================
void CManager::Draw(void)
{
	m_pScene->Draw();		// シーン
	m_pRenderer->Draw();	// レンダラー
}

//========================================
//----------------------------------------
// scene クラス
//----------------------------------------
//========================================

//========================================
// コンストラクタ
//========================================
CScene::CScene()
{

}

//========================================
// デストラクタ
//========================================
CScene::~CScene()
{

}

//========================================
// シーン生成
//========================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = new CScene;
	m_mode = mode;

	pScene->Init();

	return pScene;
}

//========================================
// 初期化
//========================================
HRESULT CScene::Init()
{
	// 現在のモードの破棄
	switch (m_mode)
	{
	case CScene::MODE_TITLE:
	{
		m_pTitle = CTitle::Create();
		break;
	}
	case CScene::MODE_TUTORIAL:
	{
		m_pPause = CPause::Create();
		m_pTutorial = CTutorial::Create();
		break;
	}
	case CScene::MODE_GAME:
	{
		m_pPause = CPause::Create();
		m_pGame = CGame::Create();
		break;
	}
	case CScene::MODE_RANKING:
	{
		m_pRanking = CRanking::Create();
		break;
	}
	break;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CScene::Uninit(void)
{
	// 現在のモードの破棄
	switch (m_mode)
	{
	case CScene::MODE_TITLE:
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = NULL;

		break;
	}
	case CScene::MODE_TUTORIAL:
	{
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;

		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;

		break;
	}
	case CScene::MODE_GAME:
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;

		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;

		break;
	}
	case CScene::MODE_RANKING:
	{
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;

		break;
	}
	break;
	}
}

//========================================
// 更新
//========================================
void CScene::Update(void)
{
	// 現在のモードの破棄
	switch (m_mode)
	{
	case CScene::MODE_TITLE:
	{
		m_pTitle->Update();
		break;
	}
	case CScene::MODE_TUTORIAL:
	{
		if (!m_pPause->IsPause())
		{
			m_pTutorial->Update();
		}
		else
		{
			m_pPause->Update();
		}
		break;
	}
	case CScene::MODE_GAME:
	{

		if (!m_pPause->IsPause())
		{
			m_pGame->Update();
		}
		else
		{
			m_pPause->Update();
		}

		break;
	}
	case CScene::MODE_RANKING:
	{
		m_pRanking->Update();
		break;
	}
	}
}

//========================================
// 描画
//========================================
void CScene::Draw(void)
{
	// 現在のモードの破棄
	switch (m_mode)
	{
	case CScene::MODE_TITLE:
	{
		m_pTitle->Draw();
		break;
	}
	case CScene::MODE_TUTORIAL:
	{
		m_pGame->Draw();
		m_pTutorial->Draw();
		break;
	}
	case CScene::MODE_GAME:
	{
		m_pGame->Draw();
		m_pPause->Draw();

		break;
	}
	case CScene::MODE_RANKING:
	{
		m_pRanking->Draw();
		break;
	}
	}
}

//========================================
// モード設定
//========================================
void CManager::SetMode(CScene::MODE mode)
{
	// サウンド停止
	m_pSound->StopSound();

	if (m_pScene != NULL)
	{
		// 現在のモードの破棄
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = NULL;
	}

	// 新しいモードの生成
	m_pScene = CScene::Create(mode);
}

//========================================
// モード取得
//========================================
CScene::MODE CScene::GetMode(void)
{
	return m_mode;
}