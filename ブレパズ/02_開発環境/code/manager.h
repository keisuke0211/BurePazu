//========================================
// 
// マネージャー処理
// 
//========================================
// *** manager.h ***
//========================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "object\object.h"

//****************************************
// 前方宣言
//****************************************
class CRenderer;		// レンダラー
class CCamera;			// カメラ
class CLight;			// ライト
class CSound;			// サウンド
class CTexture;			// テクスチャ
class CKeyboard;		// キーボード
class CMouse;			// マウス
class CJoypad;			// ジョイパット
class CFont;			// フォント
class CTitle;			// タイトル
class CTutorial;		// チュートリアル
class CGame;			// ゲーム
class CResult;			// リザルト
class CRanking;			// ランキング
class CPause;			// ポーズ
class CFade;			// フェード

//****************************************
// クラス
//****************************************

// 画面クラス
class CScene : public CPhysics
{
public:

	// ***** 列挙型 *****

	// モード
	enum MODE
	{
		MODE_TITLE = 0,	// タイトル
		MODE_TUTORIAL,	// チュートリアル
		MODE_GAME,		// ゲーム
		MODE_RESULT,	// リザルト
		MODE_RANKING,	// ランキング
		MODE_MAX
	};

	// ***** 関数 *****
	CScene();
	virtual~CScene();

	// -- メイン ------------------------------------------
	/* 生成			*/static CScene *Create(MODE mode);
	/* 初期化		*/HRESULT Init(void);
	/* 終了			*/void Uninit(void);
	/* 更新			*/void Update(void);
	/* 描画			*/void Draw(void);

	// -- 取得 ------------------------------------------
	/* モード取得		*/static CScene::MODE GetMode(void);
	/* タイトル			*/static CTitle *GetTitle() { return m_pTitle; }
	/* チュートリアル	*/static CTutorial *GetTutorial() { return m_pTutorial; }
	/* ゲーム			*/static CGame *GetGame() { return m_pGame; }
	/* ランキング		*/static CRanking *GetRanking() { return m_pRanking; }
	/* ポーズ			*/static CPause *GetPause() { return m_pPause; }

private:

	// ***** 変数 *****
	/* タイトル			*/static CTitle *m_pTitle;
	/* チュートリアル	*/static CTutorial *m_pTutorial;
	/* ゲーム			*/static CGame *m_pGame;
	/* ランキング		*/static CRanking *m_pRanking;
	/* ポーズ			*/static CPause	*m_pPause;

	static MODE m_mode;		// モード種類
};

// マネージャークラス
class CManager
{
public:

	// ***** 関数 *****
	CManager();
	virtual~CManager();

	/* 初期化	*/HRESULT Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 ----------
	/* モード */static void SetMode(CScene::MODE mode);

	// -- 取得 --------------------------------------
	/* レンダラー	*/static CRenderer *GetRenderer() { return m_pRenderer; }
	/* カメラ		*/static CCamera *GetCamera() { return m_pCamera; }
	/* ライト		*/static CLight *GetLight() { return m_pLight; }
	/* サウンド		*/static CSound *GetSound() { return m_pSound; }
	/* テクスチャ	*/static CTexture* GetTexture(){ return m_pTexture; }
	/* キーボード	*/static CKeyboard* GetInputKeyboard() { return m_InputKeyboard; }
	/* マウス		*/static CMouse* GetInputMouse() { return m_InputMouse; }
	/* ジョイパット	*/static CJoypad* GetInputJoypad() { return m_InputJoypad; }
	/* フォント		*/static CFont*	GetFont() { return m_pFont; }
	/* フェード		*/static CFade *GetFade() { return m_pFade; }

private:
	/* レンダラー	*/static CRenderer *m_pRenderer;
	/* カメラ		*/static CCamera *m_pCamera;
	/* ライト		*/static CLight *m_pLight;
	/* サウンド		*/static CSound *m_pSound;
	/* テクスチャ	*/static CTexture *m_pTexture;
	/* キーボード	*/static CKeyboard *m_InputKeyboard;
	/* フォント		*/static CFont *m_pFont;
	/* マウス		*/static CMouse* m_InputMouse;
	/* ジョイパット	*/static CJoypad* m_InputJoypad;
	/* シーン		*/static CScene *m_pScene;
	/* フェード		*/static CFade *m_pFade;

};

#endif