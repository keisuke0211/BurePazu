//========================================
// 
// 背景（天井）【 3D 】処理
// 
//========================================
// *** bg_ceiling.h ***
//========================================
#include "bg_ceiling.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../system/texture.h"
#include "../../scene/pause.h"
#include "../../scene/game.h"

//****************************************
// マクロ定義
//****************************************

// 頂点フォーマット【3D】
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


#define BG_POS				(D3DXVECTOR3(0.0f,580.0f,0.0f))			// 位置
#define BG_HEIGHT			(475.0f)								// 高さ
#define BG_RADIUS			(960.0f)								// 半径
#define BG_DIVISION_WIDTH	(32)									// 分割幅
#define BG_DIVISION_HEIGHT	(8)										// 分割高さ
#define BG_SPIN				(D3DXVECTOR3(0.0f,-0.001f,0.0f))		// 回転量

//****************************************
// コンストラクタ
//****************************************
CBgCeiling::CBgCeiling(int nPriority) : CObject(nPriority)
{
	m_pos = INIT_D3DXVECTOR3;
	m_rot = INIT_D3DXVECTOR3;
	m_color = INIT_D3DXCOLOR;
	m_nStage = 0;
}
//****************************************
// デストラクタ
//****************************************
CBgCeiling::~CBgCeiling()
{

}

//========================================
// 生成処理
//========================================
CBgCeiling *CBgCeiling::Create(int nStage)
{
	CBgCeiling *pObj = new CBgCeiling;

	if (pObj != NULL)
	{
		pObj->m_nStage = nStage;
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//========================================
// 初期化処理
//========================================
HRESULT CBgCeiling::Init(void)
{
	m_pos = BG_POS;

	SetType(TYPE_BG);
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((BG_DIVISION_WIDTH + 1) * (BG_DIVISION_HEIGHT + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 設定用ポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 真上の頂点座標/テクスチャ座標の設定
	pVtx[(BG_DIVISION_WIDTH + 1) * BG_DIVISION_HEIGHT].pos = D3DXVECTOR3(0.0f, BG_HEIGHT, 0.0f);
	pVtx[(BG_DIVISION_WIDTH + 1) * BG_DIVISION_HEIGHT].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点座標/法線ベクトル/テクスチャ座標の設定
	for (int nCntHeight = 0; nCntHeight < BG_DIVISION_HEIGHT; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_DIVISION_WIDTH + 1; nCntWidth++)
		{
			// 頂点座標
			pVtx[(nCntHeight * (BG_DIVISION_WIDTH + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_DIVISION_WIDTH))) * (BG_RADIUS * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_DIVISION_HEIGHT)))),
					BG_HEIGHT - (((nCntHeight + 1) * (BG_HEIGHT / BG_DIVISION_HEIGHT)) * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_DIVISION_HEIGHT)))),
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_DIVISION_WIDTH))) * (BG_RADIUS * sinf(D3DX_PI - ((nCntHeight + 1) * ((D3DX_PI * 0.5f) / (float)BG_DIVISION_HEIGHT)))));

			// 法線ベクトル
			pVtx[(nCntHeight * (BG_DIVISION_WIDTH + 1)) + nCntWidth].nor
				= D3DXVECTOR3(
					sinf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_DIVISION_WIDTH))),
					0.0f,
					cosf(-D3DX_PI + (nCntWidth * ((D3DX_PI * 2) / BG_DIVISION_WIDTH))));;

			// テクスチャ座標
			pVtx[(nCntHeight * (BG_DIVISION_WIDTH + 1)) + nCntWidth].tex
				= D3DXVECTOR2(
					nCntWidth * (8.0f / (float)BG_DIVISION_WIDTH),
					(nCntHeight + 1) * (2.0f / (float)BG_DIVISION_HEIGHT));
		}
	}

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((((BG_DIVISION_HEIGHT - 1) * 2 * (BG_DIVISION_WIDTH + 1)) + ((BG_DIVISION_HEIGHT - 2) * 2)) + 1),
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

	for (int nCntHeight = 0; nCntHeight < BG_DIVISION_HEIGHT - 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < BG_DIVISION_WIDTH + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((BG_DIVISION_WIDTH + 1) * (nCntHeight + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((BG_DIVISION_WIDTH + 1) * nCntHeight);
			nCntIdx += 2;
		}

		if (nCntHeight < BG_DIVISION_HEIGHT - 2)
		{
			pIdx[nCntIdx] = ((BG_DIVISION_WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (BG_DIVISION_WIDTH + 1) * (nCntHeight + 2);
			nCntIdx += 2;
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuf->Unlock();

	{
		// 天井のインデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * (BG_DIVISION_WIDTH + 2),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&m_pIdxBufLid,
			NULL);

		WORD *pIdx;	// 天井のインデックス情報へのポインタ

		// 天井のインデックスバッファをロックし、頂点番号データへのポインタを取得
		m_pIdxBufLid->Lock(0, 0, (void**)&pIdx, 0);

		pIdx[0] = (BG_DIVISION_WIDTH + 1) * BG_DIVISION_HEIGHT;
		for (int nCntIdx = 0; nCntIdx < BG_DIVISION_WIDTH + 1; nCntIdx++)
		{
			pIdx[nCntIdx + 1] = BG_DIVISION_WIDTH - nCntIdx;
		}

		// 天井のインデックスバッファをアンロックする
		m_pIdxBufLid->Unlock();
	}

	for (int nCntVtx = 0; nCntVtx < ((BG_DIVISION_WIDTH + 1) * BG_DIVISION_HEIGHT) + 1; nCntVtx++)
	{
		// 頂点カラーの設定
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// テクスチャ設定
	CTexture *pTexture = CManager::GetTexture();

	m_pTexture = pTexture->GetAddress(CTexture::TEX_NIGHT_SKY00);
	
	return S_OK;
}

//========================================
// 終了処理
//========================================
void CBgCeiling::Uninit(void)
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

	// 天井のインデックスバッファの破棄
	if (m_pIdxBufLid != NULL)
	{
		m_pIdxBufLid->Release();
		m_pIdxBufLid = NULL;
	}
}

//========================================
// 更新処理
//========================================
void CBgCeiling::Update(void)
{
	bool bPause = CPause::IsPause();

	if (!bPause)
	{
		m_rot += BG_SPIN;	// 回転量を加算
		RotControl(&m_rot);	// 向きを制御
	}
}

//========================================
// 描画処理
//========================================
void CBgCeiling::Draw(void)
{
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX	mtxRot, mtxTrans;	// 計算用マトリックス

									// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuf);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		((BG_DIVISION_WIDTH + 1) * (BG_DIVISION_HEIGHT + 1)),
		0,
		((BG_DIVISION_WIDTH * BG_DIVISION_HEIGHT) * 2) + (4 * (BG_DIVISION_HEIGHT - 1)));

	// 天井のインデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBufLid);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,
		0,
		0,
		(BG_DIVISION_WIDTH + 2),
		0,
		BG_DIVISION_WIDTH);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}