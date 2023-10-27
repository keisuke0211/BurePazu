//========================================
// 
// ライト処理
// 
//========================================
// *** light.cpp ***
//========================================
#include "light.h"
#include "../manager.h"
#include "renderer.h"

//========================================
// コンストラクタ
//========================================
CLight::CLight()
{

}

//========================================
// デストラクタ
//========================================
CLight::~CLight()
{

}

//========================================
// 初期化
//========================================
HRESULT CLight::Init(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir;//設定用方向ベクトル

	//ライトの情報をクリアする
	ZeroMemory(&m_lihgt, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_lihgt.Type = D3DLIGHT_DIRECTIONAL;

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの拡散光を設定
		m_lihgt.Diffuse = m_lihgtInformation[nCntLight].col;

		//ライトの方向を設定
		vecDir = m_lihgtInformation[nCntLight].rot;

		//ベクトルを正規化
		D3DXVec3Normalize(&vecDir, &vecDir);

		m_lihgt.Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &m_lihgt);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
	return S_OK;
}

//========================================
// 終了
//========================================
void CLight::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CLight::Update(void)
{

}