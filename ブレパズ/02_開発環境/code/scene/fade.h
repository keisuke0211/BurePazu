//========================================
// 
// フェード
// 
//========================================
// *** fade.h ***
//========================================
#ifndef _FADE_H_
#define _FADE_H_

#include "../main.h"
#include "../manager.h"
#include "../object/object2D.h"

//****************************************
// クラス
//****************************************
class CFade : public CObject2D
{

public:
	static const int FADE_TRANSITION = 25;	// フェード推移値  FADE_TRANSITION / 1000

	// ***** 列挙型 *****

	// フェードの状態
	enum FADE
	{
		FADE_NONE = 0,	// 何もしていない状態
		FADE_IN,		// フェードイン状態
		FADE_OUT,		// フェードアウト状態
		FADE_MAX,
	};

	// ***** 関数 *****
	CFade(int nPriority = PRIO_FADE);
	virtual~CFade();

	// -- メイン ------------------------------------------
	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 --------------------------------------------
	/* フェード	*/void SetFade(CScene::MODE modeNext);

	// -- 取得 --------------------------------------------
	/* 状態	*/static FADE GetFade(void) { return m_fade; }


private:

	// ***** 変数 *****
	static FADE m_fade;				// フェード状態
	static CScene::MODE m_modeNext;	// 次の画面(モード)
	D3DXCOLOR m_color;				// フェードカラー
};

#endif