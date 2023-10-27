//========================================
// 
// 背景（側面）【 3D_MULTI 】処理
// 
//========================================
// *** bg_side.h ***
//========================================
#include "bg_side.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../system/texture.h"
#include "../../scene/pause.h"

//****************************************
// マクロ定義
//****************************************

// 頂点フォーマット【3D_MULTI】
#define FVF_VERTEX_3D_MULTI (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX3)

//****************************************
// コンストラクタ
//****************************************
CBgSide::CBgSide(int nPriority) : CObject(nPriority)
{
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.rot = INIT_D3DXVECTOR3;
	m_Info.color = INIT_D3DXCOLOR;
	m_Info.fHeight = INIT_FLOAT;
	m_Info.fRadius = INIT_FLOAT;
	m_Info.nNumTex = 0;
	m_Info.nDivisionX = 1;
	m_Info.nDivisionY = 1;

	for (int nTex = 0; nTex < m_Info.nNumTex; nTex++)
	{
		m_Info.pMulti[nTex].nType = 0;
		m_Info.pMulti[nTex].fTexV = 0;
		m_Info.pMulti[nTex].fTexVSpeed = 0;
	}
}
//****************************************
// デストラクタ
//****************************************
CBgSide::~CBgSide()
{

}

//========================================
// 生成処理
//========================================
CBgSide *CBgSide::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color, float fHeight, float fRadius, int *nType, int nNumTex, int nDivisionX, int nDivisionY, float *fTexV)
{
	CBgSide *pObj = new CBgSide;

	if (pObj != NULL)
	{
		pObj->m_Info.pos = pos;
		pObj->m_Info.rot = rot;
		pObj->m_Info.color = color;
		pObj->m_Info.fHeight = fHeight;
		pObj->m_Info.fRadius = fRadius;
		pObj->m_Info.nNumTex = nNumTex;

		// メモリの解放
		pObj->m_Info.pMulti = new Multi[nNumTex];

		for (int nTex = 0; nTex < nNumTex; nTex++)
		{
			pObj->m_Info.pMulti[nTex].nType = nType[nTex];
			pObj->m_Info.pMulti[nTex].fTexV = 0.0f;
			pObj->m_Info.pMulti[nTex].fTexVSpeed = fTexV[nTex];
		}

		pObj->m_Info.nDivisionX = nDivisionX;
		pObj->m_Info.nDivisionY = nDivisionY;

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//========================================
// 初期化処理
//========================================
HRESULT CBgSide::Init(void)
{
	SetType(TYPE_BG);

	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * ((m_Info.nDivisionX + 1) * (m_Info.nDivisionY + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D_MULTI *pVtx;	// 設定用ポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標/法線ベクトル/テクスチャ座標の設定
	for (int nCntHeight = 0; nCntHeight < m_Info.nDivisionY + 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < m_Info.nDivisionX + 1; nCntWidth++)
		{
			// 位置
			pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / m_Info.nDivisionX))) * m_Info.fRadius,
					m_Info.fHeight - (nCntHeight * (m_Info.fHeight / m_Info.nDivisionY)),
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / m_Info.nDivisionX))) * m_Info.fRadius);

			// 法線ベクトル
			pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].nor
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / m_Info.nDivisionX))),
					0.0f,
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / m_Info.nDivisionX))));;


			if (m_Info.nNumTex >= 1)
			{
				// テクスチャ１
				pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].tex1
					= D3DXVECTOR2(
						nCntWidth * (8.0f / (float)m_Info.nDivisionX),
						nCntHeight * (1.0f / (float)m_Info.nDivisionY));
			}
			if (m_Info.nNumTex >= 2)
			{
				// テクスチャ2
				pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].texM
					= D3DXVECTOR2(
						nCntWidth * (8.0f / (float)m_Info.nDivisionX),
						nCntHeight * (1.0f / (float)m_Info.nDivisionY));
			}
			if (m_Info.nNumTex >= 3)
			{
				// テクスチャ3
				pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].texM2
					= D3DXVECTOR2(
						nCntWidth * (8.0f / (float)m_Info.nDivisionX),
						nCntHeight * (1.0f / (float)m_Info.nDivisionY));
			}
		}
	}

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_Info.nDivisionY * 2 * (m_Info.nDivisionX + 1)) + ((m_Info.nDivisionY - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuf,
		NULL);

	WORD *pIdx;	// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pIdxBuf->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点番号データの設定
	int nCntIdx = 0;	// インデックスのカウント

	for (int nCntDepth = 0; nCntDepth < m_Info.nDivisionY; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_Info.nDivisionX + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((m_Info.nDivisionX + 1) * (nCntDepth + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((m_Info.nDivisionX + 1) * nCntDepth);
			nCntIdx += 2;
		}

		if (nCntDepth < m_Info.nDivisionY - 1)
		{
			pIdx[nCntIdx] = ((m_Info.nDivisionX + 1) * (nCntDepth + 1)) - 1;
			pIdx[nCntIdx + 1] = (m_Info.nDivisionX + 1) * (nCntDepth + 2);
			nCntIdx += 2;
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuf->Unlock();

	for (int nCntVtx = 0; nCntVtx < ((m_Info.nDivisionX + 1) * (m_Info.nDivisionY + 1)); nCntVtx++)
	{
		// 頂点カラーの設定
		pVtx[nCntVtx].col = m_Info.color;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// テクスチャ設定
	CTexture *pTexture = CManager::GetTexture();

	if (m_Info.nNumTex >= 1)
	{
		m_pTextureMeshfield1 = pTexture->GetAddress(m_Info.pMulti[0].nType);
	}
	if (m_Info.nNumTex >= 2)
	{
		m_pTextureMeshfield2 = pTexture->GetAddress(m_Info.pMulti[1].nType);
	}
	if (m_Info.nNumTex >= 3)
	{
		m_pTextureMeshfield3 = pTexture->GetAddress(m_Info.pMulti[2].nType);
	}

	return S_OK;
}

//========================================
// 終了処理
//========================================
void CBgSide::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuf != NULL)
	{
		m_pIdxBuf->Release();
		m_pIdxBuf = NULL;
	}

	delete[] m_Info.pMulti;
	m_Info.pMulti = NULL;
}

//========================================
// 更新処理
//========================================
void CBgSide::Update(void)
{
	bool bPause = CPause::IsPause();

	if (!bPause)
	{
		VERTEX_3D_MULTI *pVtx;	// 設定用ポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の開始位置(X軸)の更新
		for (int nTex = 0; nTex < m_Info.nNumTex; nTex++)
		{
			m_Info.pMulti[nTex].fTexV += m_Info.pMulti[nTex].fTexVSpeed;
		}

		// テクスチャ座標の更新
		for (int nCntHeight = 0; nCntHeight < m_Info.nDivisionY + 1; nCntHeight++)
		{
			for (int nCntWidth = 0; nCntWidth < m_Info.nDivisionX + 1; nCntWidth++)
			{
				if (m_Info.nNumTex >= 1)
				{
					// テクスチャ1
					pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].tex1
						= D3DXVECTOR2(
							m_Info.pMulti[0].fTexV + (nCntWidth * (8.0f / (float)m_Info.nDivisionX)),
							nCntHeight * (1.0f / (float)m_Info.nDivisionY));
				}
				if (m_Info.nNumTex >= 2)
				{
					// テクスチャ2
					pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].texM
						= D3DXVECTOR2(
							m_Info.pMulti[1].fTexV + (nCntWidth * (8.0f / (float)m_Info.nDivisionX)),
							nCntHeight * (1.0f / (float)m_Info.nDivisionY));
				}
				if (m_Info.nNumTex >= 3)
				{
					// テクスチャ3
					pVtx[(nCntHeight * (m_Info.nDivisionX + 1)) + nCntWidth].texM2
						= D3DXVECTOR2(
							m_Info.pMulti[2].fTexV + (nCntWidth * (8.0f / (float)m_Info.nDivisionX)),
							nCntHeight * (1.0f / (float)m_Info.nDivisionY));
				}
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//========================================
// 描画処理
//========================================
void CBgSide::Draw(void)
{
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX	mtxRot, mtxTrans;	// 計算用マトリックス

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャステージステートの設定
	if (m_Info.nNumTex >= 1)
	{
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	}
	if (m_Info.nNumTex >= 2)
	{
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}
	if (m_Info.nNumTex >= 3)
	{
		pDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
		pDevice->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
		pDevice->SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D_MULTI));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuf);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

	// テクスチャの設定
	if (m_Info.nNumTex >= 1)
	{
		pDevice->SetTexture(0, m_pTextureMeshfield1);
	}
	if (m_Info.nNumTex >= 2)
	{
		pDevice->SetTexture(1, m_pTextureMeshfield2);
	}
	if (m_Info.nNumTex >= 3)
	{
		pDevice->SetTexture(2, m_pTextureMeshfield3);
	}

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		((m_Info.nDivisionX + 1) * (m_Info.nDivisionY + 1)),
		0,
		((m_Info.nDivisionX * m_Info.nDivisionY) * 2) + (4 * (m_Info.nDivisionY - 1)));

	// テクスチャステージステートを戻す
	if (m_Info.nNumTex >= 1)
	{
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}
	if (m_Info.nNumTex >= 2)
	{
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}
	if (m_Info.nNumTex >= 3)
	{
		pDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}