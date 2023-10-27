//========================================
// 
// 文字設定
// 
//========================================
// *** words.cpp ***
//========================================
#include "words.h"
#include "font.h"
#include "../../main.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../object/object.h"

//========================================
// コンストラクタ
//========================================
CWords::CWords(int nPriority) : CObject(nPriority)
{
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.rot = INIT_D3DXVECTOR3;
	m_Info.size = INIT_D3DXVECTOR3;
	m_Info.move = INIT_D3DXVECTOR3;
	m_Info.moveRot = INIT_D3DXVECTOR3;
	m_Info.col = INIT_D3DXCOLOR;
	m_Info.bDisp = false;
}

//========================================
// デストラクタ
//========================================
CWords::~CWords()
{

}

//========================================
// 初期化
//========================================
HRESULT CWords::Init(void)
{
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetType(TYPE_FONT);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_2D*pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//-------------------
	// 頂点情報の設定
	//-------------------
	pVtx[0].pos = D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//========================================
// 終了
//========================================
void CWords::Uninit(void)
{
	// 破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//========================================
// 更新
//========================================
void CWords::Update(void)
{
	m_Info.rot.z += m_Info.moveRot.z;

	// 頂点へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 addPos[4];
	D3DXMATRIX mtx;	// 計算用マトリックス

	// マトリックス作成
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationYawPitchRoll(&mtx, 0.0f, 0.0f, m_Info.rot.z);

	// 頂点座標
	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&addPos[i], &m_Vtx[i], &mtx);

		pVtx[i].pos.x = m_Info.pos.x + (addPos[i].x * m_Info.size.x);// <-サイズ変更
		pVtx[i].pos.y = m_Info.pos.y + (addPos[i].y * m_Info.size.y);// <-サイズ変更
		pVtx[i].pos.z = 0.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();


	// 位置更新
	m_Info.pos += m_Info.move;
}

//========================================
// 描画
//========================================
void CWords::Draw(void)
{
	if (m_Info.bDisp)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTex);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
		pDevice->SetTexture(0, NULL);
	}
}

//========================================
// 生成
//========================================
CWords *CWords::Create(const char*text, D3DXVECTOR3 pos, D3DXVECTOR3 size, CFont::FONT type, D3DXCOLOR col)
{
	CWords * pWords = NULL;
	pWords = new CWords;

	if (pWords != NULL)
	{
		pWords->SetPos(pos);
		pWords->SetSize(size);
		pWords->Init();
		pWords->SetWords(text, type);
		pWords->SetColar(col);
	}
	return pWords;
}

//================================================================================
//--------------------------------------------------------------------------------
//*********************************** 設定処理 ***********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// 文字
//========================================
void CWords::SetWords(const char*text, CFont::FONT type)
{
	string Txt = text;
	if (Txt != "")
	{
		m_Info.bDisp = true;
		m_pTex = CManager::GetFont()->GetFont(text, type);
	}
	else
	{
		m_pTex = NULL;
		SetColar(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

//========================================
// 位置
//========================================
void CWords::SetPos(const D3DXVECTOR3 &pos)
{
	m_Info.pos = pos;
}

//========================================
// 移動量
//========================================
void CWords::SetMove(const D3DXVECTOR3 &move)
{
	m_Info.move = move;
}

//========================================
// 色
//========================================
void CWords::SetColar(D3DXCOLOR col)
{
	if (m_Info.bDisp)
	{
		m_Info.col = col;

		VERTEX_2D *pVtx; //頂点へのポインタ

		 //頂点バッファをロックし頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
		pVtx[1].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
		pVtx[2].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
		pVtx[3].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}