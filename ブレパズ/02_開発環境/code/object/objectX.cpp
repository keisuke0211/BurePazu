//========================================
// 
// オブジェクト【 Xファイル 】処理
// 
//========================================
// *** objectX.cpp ***
//========================================
#include "objectX.h"
#include "../manager.h"
#include "../system/renderer.h"
#include "../system/texture.h"
#include "model\model.h"

//========================================
// コンストラクタ
//========================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pos = INIT_D3DXVECTOR3;		// 位置
	m_posOld = INIT_D3DXVECTOR3;	// 位置(過去)
	m_rot = INIT_D3DXVECTOR3;		// 向き
	m_scale = INIT_D3DXVECTOR3;		// スケール
	m_color = INIT_D3DXCOLOR;		// 色
	m_nModelID = 0;					// モデルID
	m_bColor = false;
}

//========================================
// デストラクタ
//========================================
CObjectX::~CObjectX()
{

}

//========================================
// 生成
//========================================
CObjectX *CObjectX::Create(void)
{
	CObjectX *pObj3D = NULL;

	if (pObj3D != NULL)
	{
		return pObj3D;
	}

	// オブジェクト3Dの生成
	pObj3D = new CObjectX;

	// 初期化処理
	pObj3D->Init();

	return pObj3D;
}
//========================================
// 初期化
//========================================
HRESULT CObjectX::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// スケール
	SetColor({ 1.0f,1.0f,1.0f,1.0f });			// 色

	m_pModel = CModel::Create();

	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
	m_pModel->SetScale(m_scale);
	m_pModel->SetColor(m_color);
	m_pModel->SetModelID(0);

	// モデルの更新
	m_pModel->SetModelID(m_nModelID);
	m_size = m_pModel->GetSize(m_nModelID);
	m_Width = m_pModel->GetWidth(m_nModelID);
	m_Height = m_pModel->GetHeight(m_nModelID);
	m_Depth = m_pModel->GetDepth(m_nModelID);

	return S_OK;
}

//========================================
// 終了
//========================================
void CObjectX::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;	//メモリ開放
		m_pModel = NULL;	//ポインタをNULLにする
	}

	// オブジェクトの解放
	this->Release();
}

//========================================
// 更新
//========================================
void CObjectX::Update(void)
{
	// 色の更新
	m_pModel->SetColor(m_color);
}

//========================================
// 描画
//========================================
void CObjectX::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxScale, m_scale.y, m_scale.x, m_scale.z);		// スケール拡縮行列
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);			// 行列掛け算関数 

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画設定
	m_pModel->Draw(m_mtxWorld, true);

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
