//========================================
// 
// 背景(天井)【 3D 】処理
// 
//========================================
// *** bg_ceiling.h ***
//========================================
#ifndef _BG_CEILING_H_
#define _BG_CEILING_H_

#include "../../main.h"
#include "../object.h"

//****************************************
// クラス
//****************************************
class CBgCeiling : public CObject
{
public:
	// ***** 構造体 *****

	//頂点情報【 3D 】
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;	// 頂点座標
		D3DXVECTOR3 nor;	// 法線ベクトル
		D3DCOLOR col;		// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
	};

	// ***** 関数 *****
	CBgCeiling(int nPriority = PRIO_OBJECT);
	~CBgCeiling();

	/* メイン */
	HRESULT Init(void);						// 初期化
	void Uninit(void);						// 終了
	void Update(void);						// 更新
	void Draw(void);						// 描画
	static CBgCeiling *Create(int nType);	// 生成

	/* 設定 */
	/* 位置			*/void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	/* 向き			*/void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	/* 色			*/void SetColor(const D3DXCOLOR& color) { m_color = color; }

private:
	// ***** 変数 *****
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	D3DXMATRIX mtxWorld;				// ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点情報を格納
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuf;		// インデックス情報を格納
	LPDIRECT3DINDEXBUFFER9 m_pIdxBufLid;	// 天井のインデックス情報を格納

	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_color;					// 色
	int m_nStage;						// ステージ
};
#endif