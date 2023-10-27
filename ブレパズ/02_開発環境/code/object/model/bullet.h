//========================================
// 
// 弾処理
// 
//========================================
// *** bullet.h ***
//========================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "../../main.h"
#include "../objectX.h"

//****************************************
// クラス
//****************************************
class CBullet : public CObjectX
{
public:

	// ***** 構造体 *****

	// 弾情報
	struct Info
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 位置(過去)
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 rotOld;	// 向き(過去)
		D3DXVECTOR3 move;	// 移動量
		D3DXVECTOR3	size;	// サイズ
		D3DCOLOR col;		// 頂点カラー
		int nType;			// 種類
		int nLife;			// 寿命

		float fWidth;		// 幅
		float fHeight;		// 高さ
		bool bHit;			// ヒットフラグ
		int nID;			// 自身のID
	};

	// ***** 関数 *****
	CBullet(int nPriority = PRIO_OBJECT);
	~CBullet();

	/* メイン */

	// 生成
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);
	/* リセット	*/static void Reset(void);

	/* 取得 */
	/* 弾情報		*/Info GetInfo() { return m_Info; }
	/* 位置			*/D3DXVECTOR3 GetPos() { return m_Info.pos; }
	/* 位置(過去)	*/D3DXVECTOR3 GetPosOld() { return m_Info.posOld; }
	/* 向き			*/D3DXVECTOR3 GetRot() { return m_Info.rot; }
	/* 移動量		*/D3DXVECTOR3 GetMove() { return m_Info.move; }
	/* 種類			*/int GetType(void) { return m_Info.nType; }
	/* ID			*/int GetID(void) { return m_Info.nID; }
	/* 総数			*/static int GetNumAll(void) { return m_nNumAll; }

private:

	// ***** 関数 *****
	bool Collsion(VECTOR vector, D3DXVECTOR3 pos);	// 当たり判定

	// ***** 変数 *****
	static int m_nNumAll;	// バレットの総数
	Info m_Info;			// 情報
};
#endif