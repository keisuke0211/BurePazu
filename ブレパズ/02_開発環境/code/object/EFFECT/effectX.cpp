//========================================
//
// エフェクト【 モデル 】
//
//========================================
// *** effectX.cpp ***
//=======================================
#include "effectX.h"
#include "../../manager.h"
#include "../../scene/pause.h"
#include "../../system/renderer.h"

//========================================
// コンストラクタ
//========================================
CEffectX::CEffectX(int nPriority) : CObjectX(nPriority)
{
	// 値をクリア
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.posOld = INIT_D3DXVECTOR3;
	m_Info.rot = INIT_D3DXVECTOR3;
	m_Info.size = INIT_D3DXVECTOR3;
	m_Info.col = INIT_D3DXCOLOR;
	m_Info.move = INIT_D3DXVECTOR3;
	m_Info.nType = 0;
	m_Info.nLife = 0;
	m_Info.nLifeMax = 0;
	m_Info.fRadius = INIT_FLOAT;
}

//========================================
// デストラクタ
//========================================
CEffectX::~CEffectX()
{

}

//========================================
// 生成
//========================================
CEffectX *CEffectX::Create(void)
{
	CEffectX *pEnemy = new CEffectX;

	pEnemy->SetModel(MODEL_EFFECT_00);

	// 初期化処理
	pEnemy->Init();

	return pEnemy;
}

//========================================
// 初期化
//========================================
HRESULT CEffectX::Init(void)
{
	CObjectX::Init();

	// 種類の設定
	SetType(TYPE_EFFECT);

	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = INIT_D3DXCOLOR;
	m_Info.nLife = 10;
	m_Info.fRadius = 0.0f;
	m_Info.size = D3DXVECTOR3(m_Info.fRadius, m_Info.fRadius, m_Info.fRadius);
	m_Info.nType = 0;

	// 設定
	SetPos(m_Info.pos);
	SetRot(m_Info.rot);
	SetScale(m_Info.size);
	SetColor(m_Info.col);

	return S_OK;
}

//========================================
// 終了
//========================================
void CEffectX::Uninit(void)
{
	CObjectX::Uninit();
}

//========================================
// 更新
//========================================
void CEffectX::Update(void)
{
	bool bPause = CPause::IsPause();

	if (!bPause)
	{
		// 位置の代入
		m_Info.posOld = m_Info.pos;

		// 位置の代入
		m_Info.pos.x += m_Info.move.x;
		m_Info.move.x += (0.0f - m_Info.move.x) * 0.05f;

		m_Info.pos.y += m_Info.move.y;
		m_Info.move.y += (0.0f - m_Info.move.y) * 0.05f;

		m_Info.pos.z += m_Info.move.z;
		m_Info.move.z += (0.0f - m_Info.move.z) * 0.05f;

		// 位置と向き設定
		SetPos(m_Info.pos);
		SetRot(m_Info.rot);

		// 色・サイズの推移
		if (m_Info.nType != 1)
		{
			m_Info.fRadius -= m_Info.fRadius / m_Info.nLife;
			m_Info.col.a *= ((float)m_Info.nLife / m_Info.nLifeMax);
		}

		// サイズの更新
		m_Info.size = D3DXVECTOR3(m_Info.fRadius, m_Info.fRadius, m_Info.fRadius);

		// サイズと色の設定
		SetScale(m_Info.size);
		SetColor(m_Info.col);

		if (--m_Info.nLife <= 0)
		{// 寿命が尽きたら

			Uninit();
			return;
		}

		CObjectX::Update();
	}
}

//========================================
// 描画
//========================================
void CEffectX::Draw(void)
{
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObjectX::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}