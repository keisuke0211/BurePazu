//========================================
//
// エフェクト【 モデル 】
//
//========================================
// *** effectX.h ***
//========================================
#ifndef _EFFECT_X_H_
#define _EFFECT_X_H_

#include "../../main.h"
#include "../objectX.h"

//****************************************
// クラス
//****************************************
class CEffectX : public CObjectX
{
public:

	// ***** 構造体 *****

	// エフェクト情報
	struct Info
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 位置(過去)
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3	size;	// サイズ
		D3DXVECTOR3 move;	// 移動量
		D3DXCOLOR col;		// 頂点カラー
		int nType;			// 種類
		int nLife;			// 寿命
		int nLifeMax;		// 寿命最大値
		float fRadius;		// 半径
	};

	// ***** 関数 *****
	CEffectX(int nPriority = PRIO_EFFECT);
	virtual~CEffectX();

	/* メイン */

	// 生成
	static CEffectX *Create(void);

	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	/* 設定 */
	void Eff_SetPos(D3DXVECTOR3 pos) { m_Info.pos = pos; }						// 位置
	void Eff_SetRot(D3DXVECTOR3 rot) { m_Info.rot = rot; }						// 向き
	void Eff_SetMove(D3DXVECTOR3 move) { m_Info.move = move; }					// 移動量
	void Eff_SetType(int nType) { m_Info.nType = nType; }						// 種類
	void Eff_SetLife(int nLife) { m_Info.nLife = m_Info.nLifeMax = nLife; }		// 寿命
	void Eff_SetCol(D3DXCOLOR col) { m_Info.col = col; }						// 色
	void Eff_SetRadius(float fRadius) { m_Info.fRadius = fRadius; }				// 半径

	/* 取得 */
	Info GetInfo() { return m_Info; }					// エネミー情報
	D3DXVECTOR3 GetPos() { return m_Info.pos; }			// 位置
	D3DXVECTOR3 GetPosOld() { return m_Info.posOld; }	// 位置(過去)

private:

	// ***** 変数 *****
	Info m_Info;			// 情報

};
#endif