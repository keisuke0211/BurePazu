//========================================
// 
// オブジェクト処理
// 
//========================================
// *** objext.h ***
//========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "../main.h"
#include "../system/physics.h"

//****************************************
// 前方宣言
//****************************************
class CScore;	// スコア
class CTime;	// タイム

//****************************************
// クラス
//****************************************
class CObject : public CPhysics
{
public:

	// ***** 列挙型 *****

	// 優先順位種類
	enum PRIO
	{
		PRIO_NONE = 0,	// 無し
		PRIO_OBJECT,	// オブジェクト
		PRIO_BLOCK,		// ブロック
		PRIO_CUBE,		// キューブ
		PRIO_EFFECT,	// エフェクト
		PRIO_UI,		// UI
		PRIO_TEXT,		// テキスト
		PRIO_FADE,		// フェード
		PRIO_MAX
	};

	// オブジェクト種類
	enum TYPE
	{
		TYPE_NONE = 0,	// 無し
		TYPE_BG,		// 背景
		TYPE_BG_MULTI,	// マルチ背景
		TYPE_BLOCK,		// ブロック
		TYPE_CUBE,		// キューブ
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,		// エネミー
		TYPE_BULLET,	// バレット
		TYPE_EFFECT,	// エフェクト
		TYPE_PARTICLE,	// パーティクル
		TYPE_TIME,		// タイム
		TYPE_SCORE,		// スコア
		TYPE_FONT,		// フォント
		TYPE_TEXT,		// テキスト
		TYPE_MAX
	};

	// ***** 静的関数 *****
	/* 全ての破棄			*/static void ReleaseAll(void);
	/* 全ての破棄(種類毎)	*/static void ReleaseAll(TYPE type);	// [オーバーロード]
	/* 全ての更新			*/static void UpdateAll(void);
	/* 全ての描画			*/static void DrawAll(void);

	// ***** 関数 *****
	CObject(int nPriority = 0);
	virtual~CObject();

	/* 初期化	*/virtual HRESULT Init(void) = 0;
	/* 終了		*/virtual void Uninit(void) = 0;
	/* 更新		*/virtual void Update(void) = 0;
	/* 描画		*/virtual void Draw(void) = 0;

	// -- 設定 ------------------------------------------
	/* 種類設定*/void SetType(TYPE type);

	// -- 取得 ------------------------------------------
	/* 先頭の情報				*/static CObject *GetTop(int nPriority) { return m_apTop[nPriority]; }
	/* 最後尾の情報				*/static CObject *GetCur(int nPriority) { return m_apCir[nPriority]; }
	/* 前のオブジェクト情報		*/CObject *GetPrev(void) { return m_pPrev; }
	/* 前のオブジェクト情報		*/CObject *GetNext(void) { return m_pNext; }
	/* 幅						*/virtual float GetWidth(void) { return 0.0f; }
	/* 高さ						*/virtual float GetHeight(void) { return 0.0f; }
	/* 奥行き					*/virtual float GetDepth(void) { return 0.0f; }
	/* サイズ					*/virtual D3DXVECTOR3 GetSize(void) { return INIT_D3DXVECTOR3; }
	/* 位置						*/virtual D3DXVECTOR3 GetPos(void) { return INIT_D3DXVECTOR3; }
	/* 位置(過去)				*/virtual D3DXVECTOR3 GetPosOld(void) { return INIT_D3DXVECTOR3; }
	/* 向き						*/virtual D3DXVECTOR3 GetRot(void) { return INIT_D3DXVECTOR3; }
	/* 向き(過去)				*/virtual D3DXVECTOR3 GetRotOld(void) { return INIT_D3DXVECTOR3; }
	/* 種類						*/TYPE GetType(void) { return m_type; }
	/* スコア情報				*/static CScore *GetScore(int nPriority,int nIdx);
	/* タイム情報				*/static CTime *GetTime(int nPriority, int nIdx);

protected:
	
	// ***** 関数 ****
	/* 解放処理 */void Release(void);

	// ***** 変数 *****
	CObject *m_pPrev;					// 前のオブジェクトへのポインタ
	CObject *m_pNext;					// 次のオブジェクトへのポインタ

private:
	// ***** 関数 *****
	/* 解放処理	*/void ReleaseList(void);

	// ***** 変数 *****
	static CObject *m_apTop[PRIO_MAX];	// 先頭のオブジェクトへのポインタ
	static CObject *m_apCir[PRIO_MAX];	// 最後尾のオブジェクトへのポインタ
	int m_nPriority;					// 優先順位の位置
	TYPE m_type;						// 自分自身の種類
	bool m_bDeath;						// 死亡フラグ

};
#endif