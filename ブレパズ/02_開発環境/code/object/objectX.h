//========================================
// 
// オブジェクト【 Xファイル 】処理
// 
//========================================
// *** objectX.h ***
//========================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "../main.h"
#include "object.h"

//****************************************
// 前方宣言
//****************************************
class CModel;

//****************************************
// クラス
//****************************************
class CObjectX : public CObject
{
public:

	// ***** 定義 *****
	const DWORD  FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット【3D】

	// ***** 関数 *****
	CObjectX(int nPriority = PRIO_OBJECT);
	virtual~CObjectX();

	/* メイン */
	HRESULT Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static CObjectX *Create(void);	// 生成

	/* 設定 */
	virtual void SetPos(D3DXVECTOR3 pos) { m_pos = m_posOld = pos; }	// 位置
	virtual void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }	// 位置(過去)
	virtual void SetRot(D3DXVECTOR3 rot) { m_rot = m_rotOld = rot; }	// 向き
	virtual void SetRotOld(D3DXVECTOR3 rotOld) { m_rotOld = rotOld; }	// 向き(過去)
	virtual void SetSize(const D3DXVECTOR3 &size) { m_size = size; }	// 当たり判定
	virtual void SetScale(D3DXVECTOR3 scale) { m_scale = scale; };		// スケール
	virtual void SetModel(int  nModelID) { m_nModelID = nModelID; }		// モデルID
	virtual void SetColor(D3DXCOLOR color) { m_color = color; }			// 色

	D3DXVECTOR3 GetPos() { return m_pos; }								// 位置
	D3DXVECTOR3 GetPosOld() { return m_posOld; }						// 位置(過去)
	D3DXVECTOR3 GetRot() { return m_rot; }								// 向き
	D3DXVECTOR3 GetRotOld() { return m_rotOld; }						// 向き(過去)
	D3DXVECTOR3 GetScale() { return m_scale; }							// スケール
	D3DXVECTOR3 GetSize() { return m_size; }							// 当たり判定
	D3DXCOLOR GetColor() { return m_color; }							// 色

	float GetWidth(void) { return m_Width; }	// 幅
	float GetHeight(void) { return m_Height; }	// 高さ
	float GetDepth(void) { return m_Depth; }	// 奥行き

private:
	// ***** 変数 *****
	D3DXMATRIX  m_mtxWorld;	//ワールドマトリックス

	CModel		*m_pModel;	// モデル
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 位置(過去)
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_rotOld;	// 向き(過去)
	D3DXVECTOR3	m_size;		// 当たり判定
	D3DXVECTOR3	m_scale;	// スケール
	D3DXCOLOR m_color;		// 色
	bool m_bColor;			// 色変更フラグ
	int m_nModelID;			// モデルID
	float m_Width;
	float m_Height;
	float m_Depth;
	char m_aModelPath[TXT_MAX];

};
#endif