//========================================
// 
// ポーズ
// 
//========================================
// *** pause.h ***
//========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "../main.h"
#include "../manager.h"

//****************************************
// 前方宣言
//****************************************
class CWords;
class CText;
class CBlackout;

//****************************************
// クラス
//****************************************
class CPause : public CScene
{
	// ***** 定義 *****
	static const int TEXT_MAX = 4;	// テキストの最大数

public:

	// ***** 列挙型 *****

	// メニュー
	enum MENU
	{
		MENU_RESUME = 0,// 再開
		MENU_RESET,		// やり直す
		MENU_TITLE,		// タイトルに戻る
		MENU_MAX
	};

	// ***** 関数 *****
	CPause();
	~CPause();

	// -- メイン ------------------------------------------
	/* 初期化		*/HRESULT Init(void);
	/* 終了			*/void Uninit(void);
	/* 更新			*/void Update(void);
	/* 描画			*/void Draw(void);
	/* 生成			*/static CPause *Create(void);
	/* メニュー生成 */void MenuCreate(void);
	/* メニュー		*/void Menu(void);

	// -- 設定 --------------------------------------------
	/* ポーズ */static void SetPause(bool bPause) { m_bPause = bPause; }

	// -- 取得 --------------------------------------------
	/* ポーズ	*/static bool IsPause(void) { return m_bPause; }

private:

	// ***** 変数 *****
	static bool m_bPause;
	bool m_Menu;
	int m_nSelectMenu;	// 現在選択しているメニュー
	CText *m_Text[TEXT_MAX];
	CBlackout *m_Blackout;

};

#endif