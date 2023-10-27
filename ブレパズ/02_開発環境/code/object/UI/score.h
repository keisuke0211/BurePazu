//========================================
//
// スコア
//
//========================================
// *** score.h ***
//========================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "../../main.h"
#include "../object.h"

//****************************************
// 前方宣言
//****************************************
class CText;

//****************************************
// クラス
//****************************************
class CScore : public CObject
{
	// 定義
	static const int SCORE_START_DEX = 6;	// スコアの開始位置

public:

	CScore(int nPriority = PRIO_UI);
	~CScore();

	// *****  関数 *****

	// -- メイン ---------------------------------
	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);
	/* 生成		*/static CScore *Create(void);

	// -- 設定 ---------------------------------
	/* スコア	*/static void SetScore(int nScore = 0);

	// -- 取得 ---------------------------------
	/* スコア	*/int GetScore(void) { return m_nScore; }

private:
	// ***** 変数 *****
	static int m_nScore;
	static bool m_bUpdateText;	// スコアの更新フラグ
	int m_Digit;				// 桁数
	CText *m_Score;				// テキスト表示
};
#endif 