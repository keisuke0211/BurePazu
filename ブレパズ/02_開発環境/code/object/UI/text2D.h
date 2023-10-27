//========================================
// 
// テキスト(2D)
// 
//========================================
// *** text2D.h ***
//========================================
#ifndef _TEXT2D_H_	// このマクロ定義がされていなかった時
#define _TEXT2D_H_	// 二重インクルード防止のマクロを定義する

#include "../object.h"
#include "../../main.h"

//****************************************
// 前方宣言
//****************************************
class CObject2D;	// オブジェクト2Dクラス

//****************************************
// クラス
//****************************************
class CText2D :public CObject 
{
	static const int START_CHAR = (int)'!';				// 開始文字番号
	static const int PTN_WIDTH = 8;						// パターン幅
	static const int PTN_HEIGHT = 8;					// パターン高さ
	static const int PTN_MAX = PTN_WIDTH * PTN_HEIGHT;	// パターン最大値

public:
	// ***** 列挙型 *****

	// 表示形式
	enum DISPLAY
	{
		DISPLAY_CENTER,	// 中央揃え
		DISPLAY_LEFT,	// 左揃え
		DISPLAY_RIGHT,	// 右揃え
		DISPLAY_MAX,
	};

	// ***** 関数 *****
	CText2D(int nPriority = PRIO_UI);
	virtual~CText2D();
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	/* 設定 */
	void SetString(char *pString);					// 文字列
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置
	void SetDisp(DISPLAY disp) { m_disp = disp; }	// 表示形式

	// サイズ
	void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }

	// *** 静的関数 ***
	static CText2D *Create(void);		// 生成処理

private:
	// ***** 変数 *****
	CObject2D *m_apObject2D[TXT_MAX];	// オブジェクト2Dのポインタ配列
	char m_aString[TXT_MAX];			// 文字列のポインタ
	DISPLAY m_disp;		// 表示形式
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
};

#endif