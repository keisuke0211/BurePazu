//========================================
// 
// オブジェクト【 3D 】処理
// 
//========================================
// *** object3D.h ***
//========================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "../main.h"
#include "object.h"

//****************************************
// クラス
//****************************************
class CObject3D : public CObject
{
public:

	// ***** 定義 *****
	const DWORD  FVF_VERTEX_3D	= (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット【3D】

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
	CObject3D(int nPriority = PRIO_OBJECT);
	virtual~CObject3D();

	/* メイン */
	HRESULT Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static CObject3D *Create(void);	// 生成

	/* 設定 */
	virtual void SetPos(D3DXVECTOR3 pos) { m_pos = m_posOld = pos; }		// 位置
	virtual void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }					// 向き
	virtual void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; };		// サイズ
	virtual void SetColor(const D3DXCOLOR& color);		// 色
	void SetPtn(int nPtn) { m_nPtn = nPtn; }			// パターン
	virtual void SetTex(PositionVec4 tex);				// テクスチャ座標
	void BindTexture(int nIdx);							// テクスチャ指定

	int GetPtn(void) { return m_nPtn; }				// パターン 
	int GetPtnWidth(void) { return m_nPtnWidth; }	// パターン幅
	int GetPtnHeight(void) { return m_nPtnHeight; }	// パターン高さ

	// パターンの情報設定
	void SetPtnInfo(int nPtnWidth = 1, int nPtnHeight = 1, int nPtnMax = 1, bool bPtnAnim = false, int nAnimTime = 0);

protected:

	// ***** 関数 *****
	void SetVtxPos(VERTEX_3D *pVtx);			// 頂点座標の設定
	virtual void SetTexPos(VERTEX_3D *pVtx);	// テクスチャ座標の設定
	void Animation(void);						// アニメーション

	/* 取得 */
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }	// 頂点バッファ

private:
	// ***** 変数 *****
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	D3DXMATRIX mtxWorld;				// ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点情報を格納

	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_posOld;				// 位置(過去)
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_color;					// 色
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	int m_nPtn;							// パターン
	int m_nPtnWidth;					// パターン幅
	int m_nPtnHeight;					// パターン高さ
	int m_nPtnMax;						// パターンの最大
	bool m_bPtnAnim;					// パターンアニメフラグ
	int m_nAnimCounter;					// アニメカウンター
	int m_nAnimTime;					// アニメにかかる時間

};
#endif