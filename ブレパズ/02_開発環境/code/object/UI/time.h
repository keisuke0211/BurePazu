//========================================
//
// 時間
//
//========================================
// *** time.h ***
//========================================
#ifndef _TIME_H_
#define _TIME_H_

#include "../../main.h"
#include "../object.h"

//****************************************
// 前方宣言
//****************************************
class CText;

//****************************************
// クラス
//****************************************
class CTime : public CObject
{
	// 定義
	static const int TIME_START_DEX = 6;	// 時間の開始位置

public:

	CTime(int nPriority = PRIO_UI);
	~CTime();

	// *****  関数 *****

	/* メイン */
	/* 初期化		*/HRESULT Init(void);
	/* 終了			*/void Uninit(void);
	/* 更新			*/void Update(void);
	/* 描画			*/void Draw(void);
	/* 生成			*/static CTime *Create(int nTime = 0);
	/* タイム設定	*/void SetTime(int nTime = 0);
	/* タイム所得	*/int GetTime(void) { return m_nTime; }
private:
	// ***** 変数 *****
	int m_nTime;		// 時間
	int m_nTimeMax;		// 時間の最大値
	int m_nCounter;		// カウンター
	int m_Digit;		// 桁数
	int m_nNumWarn;		// 生成した警告数
	bool m_bSetTime;	// 時間の設定をしたか
	bool m_bUpdateTime;	// 時間の更新フラグ
	CText *m_Time;		// テキスト表示

};
#endif 