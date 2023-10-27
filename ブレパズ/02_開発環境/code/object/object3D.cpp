//========================================
// 
// オブジェクト【 3D 】処理
// 
//========================================
// *** object3D.cpp ***
//========================================
#include "../manager.h"
#include "object3D.h"
#include "../system/renderer.h"
#include "../system/texture.h"

//========================================
// コンストラクタ
//========================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pos = INIT_D3DXVECTOR3;		// 位置
	m_posOld = INIT_D3DXVECTOR3;	// 位置(過去)
	m_rot = INIT_D3DXVECTOR3;		// 向き
	m_color = INIT_D3DXCOLOR;		// 色
	m_fHeight = INIT_FLOAT;			// 高さ
	m_fWidth = INIT_FLOAT;			// 幅
	m_nPtn = 0;						// パターン
	m_nPtnWidth = 0;				// パターン幅
	m_nPtnHeight = 0;				// パターン高さ
	m_nPtnMax = 0;					// パターンの最大
	m_bPtnAnim = false;				// パターンアニメフラグ
	m_nAnimCounter = 0;				// アニメカウンター
	m_nAnimTime = 0;				// アニメにかかる時間
}

//========================================
// デストラクタ
//========================================
CObject3D::~CObject3D()
{

}

//========================================
// 生成処理
//========================================
CObject3D *CObject3D::Create(void)
{
	CObject3D *pObj3D = NULL;

	if (pObj3D != NULL)
	{
		return pObj3D;
	}

	// オブジェクト3Dの生成
	pObj3D = new CObject3D;

	// 初期化処理
	pObj3D->Init();

	return pObj3D;
}

//========================================
// 初期化処理
//========================================
HRESULT CObject3D::Init(void)
{
	m_nPtn = 0;					// パターン
	m_nPtnWidth = 1;			// パターン幅
	m_nPtnHeight = 1;			// パターン高さ
	m_nPtnMax = 1;				// パターンの最大
	m_bPtnAnim = false;			// パターンアニメフラグ
	m_nAnimCounter = 0;			// アニメカウンター
	m_nAnimTime = 0;			// アニメにかかる時間

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 設定用ポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標の設定
	SetVtxPos(pVtx);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);

	// 頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファの破棄
	m_pVtxBuff->Unlock();

	return S_OK;
}

//========================================
// 終了処理
//========================================
void CObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの解放
	Release();
}

//========================================
// 更新処理
//========================================
void CObject3D::Update(void)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	SetTexPos(pVtx);

	// 頂点座標の設定
	SetVtxPos(pVtx);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//========================================
// 描画処理
//========================================
void CObject3D::Draw(void)
{
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;										//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// 頂点座標の設定処理
//========================================
void CObject3D::SetVtxPos(VERTEX_3D *pVtx)
{
	// 対角線の長さ
	float fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight)) * 0.5f;
	// 対角線の角度
	float fAngle = atan2f(m_fWidth, m_fHeight);

	// 頂点座標を設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.y - (D3DX_PI - fAngle)) * fLength;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = m_pos.z + cosf(m_rot.y - (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.y + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = m_pos.z + cosf(m_rot.y - (D3DX_PI - fAngle)) * fLength;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.y - fAngle) * fLength;
	pVtx[2].pos.y = m_pos.y ;
	pVtx[2].pos.z = m_pos.z + cosf(m_rot.y - fAngle) * fLength;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.y + fAngle) * fLength;
	pVtx[3].pos.y = m_pos.y;
	pVtx[3].pos.z = m_pos.z + cosf(m_rot.y - fAngle) * fLength;
}

//========================================
// 色設定
//========================================
void CObject3D::SetColor(const D3DXCOLOR& color)
{
	// サイズの代入
	m_color = color;

	// 設定用ポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[1].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[2].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[3].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);

	// 頂点バッファの破棄
	m_pVtxBuff->Unlock();
}

//========================================
// パターン情報設定
//========================================
void CObject3D::SetPtnInfo(int nPtnWidth, int nPtnHeight, int nPtnMax, bool bPtnAnim, int nAnimTime)
{
	m_nPtnWidth = nPtnWidth;	// パターン幅
	m_nPtnHeight = nPtnHeight;	// パターン高さ
	m_nPtnMax = nPtnMax;		// パターンの最大
	m_bPtnAnim = bPtnAnim;	 	// パターンアニメフラグ
	m_nAnimCounter = 0;			// アニメカウンター(初期化)
	m_nAnimTime = nAnimTime;	// アニメにかかる時間
}

//========================================
// アニメーション処理
//========================================
void CObject3D::Animation(void)
{
	// パターンアニメフラグが真の時、
	if (m_bPtnAnim)
	{
		if (++m_nAnimCounter % m_nAnimTime == 0)
		{// アニメカウンターを加算してかかる時間で割り切れたら、
			m_nAnimCounter = 0;
			m_nPtn = (m_nPtn + 1) % m_nPtnMax;	// パターンを加算し、最大で剰余算
		}
	}
}

//========================================
// テクスチャ座標の設定処理
//========================================
void CObject3D::SetTexPos(VERTEX_3D *pVtx)
{
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth), ((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight));
	pVtx[1].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth) + (1.0f / m_nPtnWidth), ((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight));
	pVtx[2].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth), (((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight)) + (1.0f / m_nPtnHeight));
	pVtx[3].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth) + (1.0f / m_nPtnWidth), (((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight)) + (1.0f / m_nPtnHeight));
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CObject3D::BindTexture(int nIdx)
{
	CTexture *pTexture = CManager::GetTexture();

	m_pTexture = pTexture->GetAddress(nIdx);
}

//========================================
// テクスチャ設定
//========================================
void CObject3D::SetTex(PositionVec4 tex)
{
	VERTEX_3D *pVtx; //頂点へのポインタ

	 //頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(tex.P0, tex.P2);
	pVtx[1].tex = D3DXVECTOR2(tex.P1, tex.P2);
	pVtx[2].tex = D3DXVECTOR2(tex.P0, tex.P3);
	pVtx[3].tex = D3DXVECTOR2(tex.P1, tex.P3);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}