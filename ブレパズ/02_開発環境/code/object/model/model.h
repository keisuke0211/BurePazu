//========================================
// 
// モデル処理
// 
//========================================
// *** model.h ***
//========================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "../../main.h"
#include "../../system/physics.h"
#include "../object.h"

//****************************************
// クラス
//****************************************
class CModel
{
public:

	// ***** 定義 *****
	const DWORD  FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット【3D】

	// ***** 構造体 *****

	//頂点情報【 3D 】
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;	//頂点座標
		D3DXVECTOR3 nor;	//法線ベクトル
		D3DCOLOR col;		//頂点カラー
		D3DXVECTOR2 tex;	//テクスチャ座標
	};

	struct MODEL_MATERIAL
	{
		LPD3DXMESH		pMesh;				// メッシュ情報へのポインタ
		LPD3DXBUFFER	pBuffer;			// マテリアル情報へのポインタ
		DWORD			nNumMat;			// マテリアル情報の数
		D3DXVECTOR3		size;				// モデルの大きさ
		int				*pIdxTex;			// テクスチャ番号
		char			aFileName[0xff];	// Xファイルのパス
		bool			bShadow;			// 影使用の有無

		float fWidth;
		float fHeight;
		float fDepth;
	};

	// ***** 静的関数 *****

	/* 生成			*/static CModel *Create(void);
	/* 初期化		*/static void InitModel(void);
	/* 終了			*/static void UninitModel(void);
	/* 読み込み		*/static void LoadModel(const char *pFileName);
	/* サイズの算出 */static void CalcSize(int nCntModel);

	/* 取得 */
	/* マテリアル情報	*/static MODEL_MATERIAL *GetMaterial() { return m_material; }
	/* モデル数			*/static int GetMaxModel(void) { return m_nMaxModel; }

	// ***** 関数 *****
	CModel();
	virtual~CModel();

	// -- メイン ------------------------------------------

	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(bool Color);
	/* 描画		*/void Draw(D3DXMATRIX mtxParent, bool Color);

	// -- 設定 ------------------------------------------

	/* 位置					*/virtual void SetPos(const D3DXVECTOR3 pos) { m_pos = m_posOld = pos; }
	/* 向き					*/void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	/* 色					*/void SetColor(const D3DXCOLOR color) { m_color = color; }
	/* スケール				*/void SetScale(const D3DXVECTOR3 &scale) { m_scale = scale; }
	/* ワールドマトリックス	*/void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }
	/* 親モデル				*/void SetParent(CModel *pParent) { m_pParent = pParent; }
	/* モデルID				*/void SetModelID(const int nModelID) { m_nModelID = nModelID; }
	/* 影の有無				*/static void SetPlayerShadow(bool bShadow);

	// -- 所得 ------------------------------------------

	/* 位置					*/D3DXVECTOR3 GetPos(void) { return m_pos; }
	/* 向き					*/D3DXVECTOR3 GetRot(void) { return m_rot; }
	/* 色					*/D3DXCOLOR GetColor(void) { return m_color; }
	/* サイズ				*/D3DXVECTOR3 GetSize(const int nModelID);
	/* スケール				*/D3DXVECTOR3 GetScale(void) { return m_scale; }
	/* ワールドマトリックス	*/D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	/* 親モデル				*/CModel *GetParent(void) { return m_pParent; }
	/* モデルID				*/int GetModelID(void) { return m_nModelID; }
	/* 幅					*/static float GetWidth(const int nModelID);
	/* 高さ					*/static float GetHeight(const int nModelID);
	/* 奥行き				*/static float GetDepth(const int nModelID);

private:

	// ***** 関数 *****
	/* 影	*/void SetShadow(LPDIRECT3DDEVICE9 pDevice, D3DXMATERIAL *pMat,int nModelID);

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	// ***** 静的変数 *****
	/* マテリアル情報	*/static MODEL_MATERIAL	*m_material;
	/* モデル数			*/static int m_nMaxModel;

	// ***** 変数 *****
	/* テクスチャポインタ	*/LPDIRECT3DTEXTURE9 m_pTexture;
	/* ワールドマトリックス	*/D3DXMATRIX  m_mtxWorld;


	/* 親モデルの情報		*/CModel	*m_pParent;
	/* 位置					*/D3DXVECTOR3 m_pos;
	/* 位置(過去)			*/D3DXVECTOR3 m_posOld;
	/* 向き					*/D3DXVECTOR3 m_rot;
	/* サイズ				*/D3DXVECTOR3	m_size;
	/* スケール				*/D3DXVECTOR3	m_scale;
	/* 色					*/D3DXCOLOR m_color;
	/* モデルID				*/int	m_nModelID;
	/* 幅					*/static float m_Width;
	/* 高さ					*/static float m_Height;
	/* 奥行き				*/static float m_Depth;
	/* プレイヤーの影		*/static bool m_bPlayerShadow;
};
#endif