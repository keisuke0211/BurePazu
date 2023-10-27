//========================================
// 
// メッシュフィールド
// 
//========================================
// *** mesh_floor.h ***
//========================================
#ifndef _MESH_FLOOR_H_
#define _MESH_FLOOR_H_

#include "../../main.h"
#include "../object.h"

//****************************************
// クラス
//****************************************
class CFloor : public CObject
{
public:
	// ***** 構造体 *****

	// 頂点情報[3D_MULTI]の構造体を定義
	struct VERTEX_3D_MULTI
	{
		D3DXVECTOR3 pos;	// 頂点座標
		D3DXVECTOR3 nor;	// 法線ベクトル
		D3DCOLOR	col;	// 頂点カラー
		D3DXVECTOR2 tex1;	// テクスチャ座標1
		D3DXVECTOR2 texM;	// テクスチャ座標2
		D3DXVECTOR2 texM2;	// テクスチャ座標3
	};

	// ***** 関数 *****
	CFloor(int nPriority = PRIO_OBJECT);
	~CFloor();

	/* メイン */
	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);
	/* 生成		*/static CFloor *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color, float fWidth,float fHeight, int *nType, int nNumTex,bool bDivision, int nDivisionX, int nDivisionY, float *fTexX, float *fTexY);

	/* 設定 */
	/* 位置			*/void SetPos(D3DXVECTOR3 pos) { m_Info.pos = pos; }
	/* 向き			*/void SetRot(D3DXVECTOR3 rot) { m_Info.rot = rot; }
	/* 色			*/void SetColor(const D3DXCOLOR& color) { m_Info.color = color; }

private:
	// ***** 構造体 *****
	struct Multi
	{
		int	nType;			// 画像種類
		float fTexX;		// テクスチャ座標の開始位置(X軸)
		float fTexY;		// テクスチャ座標の開始位置(Y軸)
		float fTexXSpeed;	// テクスチャ座標の移動速度(X軸)
		float fTexYSpeed;	// テクスチャ座標の移動速度(Y軸)
	};

	// 共通情報
	struct Info
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		D3DXCOLOR	color;	// 色
		float fWidth;		// 幅
		float fHeight;		// 高さ
		int nNumTex;		// 画像数
		int nDivisionX;		// 分割幅
		int nDivisionY;		// 分割高さ
		int nNumDivisionX;	// 分割数X
		int nNumDivisionY;	// 分割数Y
		Multi *pMulti;		// マルチ情報
	};

	// ***** 変数 *****
	LPDIRECT3DTEXTURE9 m_pTextureMeshfield1;		// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureMeshfield2;		// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureMeshfield3;		// テクスチャへのポインタ
	D3DXMATRIX mtxWorld;							// ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点情報を格納
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuf;	//インデックス情報を格納

	Info m_Info;	// 共通情報
};
#endif