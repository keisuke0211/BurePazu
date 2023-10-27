//========================================
// 
// パーツ処理
// 
//========================================
// *** parts.h ***
//========================================
#ifndef _PARTS_H_			// このマクロ定義がされてなかったら
#define _PARTS_H_			// 二重インクルード防止のマクロ定義

#include "model.h"
#include "../../main.h"

//****************************************
// クラス
//****************************************
class CParts : public CModel
{
public:
	// ***** 静的関数 *****
	static CParts *Create();	// 生成

	CParts();			// コンストラクタ
	virtual~CParts();	// デストラクタ

	// ***** 関数 *****

	/* 設定 */
	void SetPosOrigin(const D3DXVECTOR3 &posOrigin) { m_posOrigin = posOrigin; }		// 元の位置
	void SetRotOrigin(const D3DXVECTOR3 &rotOrigin) { m_rotOrigin = rotOrigin; }		// 元の向き
	void SetPosDest(const D3DXVECTOR3 &posDest) { m_posDest = posDest; }				// 目的位置
	void SetRotDest(const D3DXVECTOR3 &rotDest) { m_rotDest = rotDest; }				// 目的向き

	/* 取得 */
	D3DXVECTOR3 GetPosOrigin() { return m_posOrigin; }									// 元の位置
	D3DXVECTOR3 GetRotOrigin() { return m_rotOrigin; }									// 元の向き
	D3DXVECTOR3 GetPosDest() { return m_posDest; }										// 目的位置
	D3DXVECTOR3 GetRotDest() { return m_rotDest; }										// 目的向き

private:
	// ***** 変数 *****
	D3DXVECTOR3 m_posOrigin;		// 元の位置
	D3DXVECTOR3 m_rotOrigin;		// 元の向き
	D3DXVECTOR3 m_posDest;			// 目的の位置
	D3DXVECTOR3 m_rotDest;			// 目的の向き
};

#endif
