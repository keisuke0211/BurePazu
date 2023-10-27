//========================================
//
// パーティクル【 モデル 】
//
//========================================
// *** particleX.h ***
//========================================
#ifndef _PARTICLE_X_H_
#define _PARTICLE_X_H_

#include "../../main.h"
#include "../objectX.h"

//****************************************
// クラス
//****************************************
class CParticleX : public CObjectX
{
public:

	// ***** 構造体 *****

	// カラー
	struct Color
	{
		D3DXCOLOR col;				// カラー
		D3DXCOLOR colRandamMax;		// ランダムな色の範囲(最大)
		D3DXCOLOR colRandamMin;		// ランダムな色の範囲(最小)
		bool bColRandom;			// ランダムで色を変更するか
	};

	// パーティクル
	struct Info
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 move;	// 移動量
		D3DXVECTOR3 speed;	// 移動量
		Color aColor;		// カラー
		int nForm;			// 生成数
		int nType;			// 種類
		int nLife;			// 寿命
		float fRadius;		// 半径
	};

	// コンストラクタ・デストラクタ
	CParticleX(int nPriority = PRIO_EFFECT);
	virtual~CParticleX();

	// *****  関数 *****

	/* メイン */
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新

	// 生成
	static CParticleX *Create(void);

	/* 設定 */
	void Par_SetPos(D3DXVECTOR3 pos) { m_Info.pos = pos; }						// 位置
	void Par_SetRot(D3DXVECTOR3 rot) { m_Info.rot = rot; }						// 向き
	void Par_SetMove(D3DXVECTOR3 move) { m_Info.speed = move; }					// 移動量
	void Par_SetType(int nType) { m_Info.nType = nType; }						// 種類
	void Par_SetForm(int nForm) { m_Info.nForm = nForm;	}						// 生成数
	void Par_SetLife(int nLife) { m_Info.nLife  = nLife; }						// 寿命
	void Par_SetCol(D3DXCOLOR col) { m_Info.aColor.col = col; }					// 色
	void Par_SetRadius(float fRadius) { m_Info.fRadius = fRadius; }				// 半径

private:
	// ***** 変数 *****
	Info m_Info;		// 情報

};
#endif 