//========================================
// 
// オブジェクト【 2D 】処理
// 
//========================================
// *** object2D.cpp ***
//========================================
#include "../manager.h"
#include "object2D.h"
#include "../system/renderer.h"
#include "../system/texture.h"

//========================================
// コンストラクタ
//========================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pos = INIT_D3DXVECTOR3;		// 位置
	m_rot = INIT_D3DXVECTOR3;		// 向き
	m_color = INIT_D3DXCOLOR;		// 色
	m_fWidth = 0.0f;				// 幅
	m_fHeight = 0.0f;				// 高さ
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
CObject2D::~CObject2D()
{

}

//========================================
// 生成処理
//========================================
CObject2D *CObject2D::Create(void)
{
	CObject2D *pObject2D = NULL;

	if (pObject2D != NULL)
	{
		return pObject2D;
	}

	// オブジェクト2Dの生成
	pObject2D = new CObject2D;

	// 初期化処理
	pObject2D->Init();

	return pObject2D;
}

//========================================
// 初期化処理
//========================================
HRESULT CObject2D::Init(void)
{
	m_pos = INIT_D3DXVECTOR3;	// 位置
	m_rot = INIT_D3DXVECTOR3;	// 向き
	m_color = INIT_D3DXCOLOR;	// 色
	m_fWidth = 0.0f;			// 幅
	m_fHeight = 0.0f;			// 高さ
	m_nPtn = 0;					// パターン
	m_nPtnWidth = 1;			// パターン幅
	m_nPtnHeight = 1;			// パターン高さ
	m_nPtnMax = 1;				// パターンの最大
	m_bPtnAnim = false;			// パターンアニメフラグ
	m_nAnimCounter = 0;			// アニメカウンター
	m_nAnimTime = 0;			// アニメにかかる時間

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 設定用ポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標の設定
	SetVtxPos(pVtx);	

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTexPos(pVtx);

	// 頂点バッファの破棄
	m_pVtxBuff->Unlock();

	return S_OK;
}

//========================================
// 終了処理
//========================================
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

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
void CObject2D::Draw(void)
{
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


//========================================
// 頂点座標の設定処理
//========================================
void CObject2D::SetVtxPos(VERTEX_2D *pVtx)
{
	// 対角線の長さ
	float fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight)) * 0.5f;
	// 対角線の角度
	float fAngle = atan2f(m_fWidth, m_fHeight);

	// 頂点座標を設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.y - (D3DX_PI - fAngle)) * fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.y - (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.y + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.y + (D3DX_PI - fAngle)) * fLength;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.y - fAngle) * fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.y - fAngle) * fLength;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.y + fAngle) * fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.y + fAngle) * fLength;
}

//========================================
// テクスチャの割り当て
//========================================
void CObject2D::BindTexture(int nIdx)
{
	CTexture *pTexture = CManager::GetTexture();

	m_pTexture = pTexture->GetAddress(nIdx);
}


//========================================
// アニメーション処理
//========================================
void CObject2D::Animation(void)
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
// パターン情報設定
//========================================
void CObject2D::SetPtnInfo(int nPtnWidth, int nPtnHeight, int nPtnMax, bool bPtnAnim, int nAnimTime)
{
	m_nPtnWidth = nPtnWidth;	// パターン幅
	m_nPtnHeight = nPtnHeight;	// パターン高さ
	m_nPtnMax = nPtnMax;		// パターンの最大
	m_bPtnAnim = bPtnAnim;	 	// パターンアニメフラグ
	m_nAnimCounter = 0;			// アニメカウンター(初期化)
	m_nAnimTime = nAnimTime;	// アニメにかかる時間
}

//========================================
// テクスチャ座標の設定処理
//========================================
void CObject2D::SetTexPos(VERTEX_2D *pVtx)
{
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth), ((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight));
	pVtx[1].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth) + (1.0f / m_nPtnWidth), ((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight));
	pVtx[2].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth), (((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight)) + (1.0f / m_nPtnHeight));
	pVtx[3].tex = D3DXVECTOR2((m_nPtn % m_nPtnWidth) * (1.0f / m_nPtnWidth) + (1.0f / m_nPtnWidth), (((m_nPtn / m_nPtnWidth) % m_nPtnHeight) * (1.0f / m_nPtnHeight)) + (1.0f / m_nPtnHeight));
}

//========================================
// テクスチャ設定
//========================================
void CObject2D::SetTex(PositionVec4 tex)
{
	VERTEX_2D *pVtx; //頂点へのポインタ

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

//========================================
// 色の設定処理
//========================================
void CObject2D::SetColar(D3DXCOLOR color)
{
	m_color = color;

	VERTEX_2D *pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[1].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[2].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[3].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}