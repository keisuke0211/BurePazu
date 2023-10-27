//========================================
// 
// モーションモデル処理
// 
//========================================
// *** motion_model.cpp ***
//========================================
#include "motion.h"
#include "motion_model.h"
#include "../../manager.h"
#include "../../system/renderer.h"


//========================================
// コンストラクタ
//========================================
CMotionModel::CMotionModel(int nPriority) : CObject(nPriority)
{
	m_pMotion = NULL;
	m_pos = INIT_D3DXVECTOR3;
	m_posOld = INIT_D3DXVECTOR3;
	m_rot = INIT_D3DXVECTOR3;
	m_size = INIT_D3DXVECTOR3;
}

//========================================
// デストラクタ
//========================================
CMotionModel::~CMotionModel()
{

}

//========================================
// 生成
//========================================
CMotionModel * CMotionModel::Create(void)
{
	CMotionModel *pMotionModel = NULL;

	pMotionModel = new CMotionModel;

	// メモリの確保ができなかった
	assert(pMotionModel != NULL);

	// 数値の初期化
	pMotionModel->Init();

	// インスタンスを返す
	return pMotionModel;
}

//========================================
// 初期化
//========================================
HRESULT CMotionModel::Init(void)
{
	// 変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);				// スケール
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);					// 当たり判定
	m_quat = { 0.0f,0.0f,0.0f,1.0f };						// クオータニオン
	SetIsColor(false);										// 色の変更
	SetColor({ 1.0f,1.0f,1.0f,1.0f });						// 色

	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXQuaternionRotationAxis(&m_quat, &vecY, D3DX_PI);	// クオータニオンによる回転
	return S_OK;
}

//========================================
// 終了
//========================================
void CMotionModel::Uninit(void)
{
	if (m_pMotion != NULL)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// オブジェクトの解放
	Release();
}

//========================================
// 更新
//========================================
void CMotionModel::Update(void)
{
	if (m_pMotion != NULL)
	{// モーション番号の設定
		m_pMotion->Update();
	}
}

//========================================
// 描画
//========================================
void CMotionModel::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxRotQuat, mtxScale;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);									// 行列初期化関数

	// クォータニオンの使用した姿勢の設定
	D3DXMatrixRotationQuaternion(&mtxRotQuat, &m_quat);					// クオータニオンによる行列回転

	D3DXMatrixScaling(&mtxScale, m_scale.y, m_scale.x, m_scale.z);		// スケール拡縮行列
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);			// 行列掛け算関数 

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// 行列回転関数

	D3DXMatrixMultiply(&mtxRot, &mtxRot, &mtxRotQuat);					// 行列掛け算関数 
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);				// 行列掛け算関数 

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);		// 行列移動関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);			// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pMotion != NULL)
	{// パーツの描画設定
		m_pMotion->SetParts(m_mtxWorld, m_bcolor, m_color);
	}
}


//========================================
// モーションの設定
//========================================
void CMotionModel::SetMotion(const char * pName)
{
	if (m_pMotion != NULL)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// モーション情報
	m_pMotion = new CMotion(pName);
	assert(m_pMotion != nullptr);

	// モーションの初期設定
	m_pMotion->SetMotion(0);

	// モーション番号の設定
	m_pMotion->SetNumMotion(0,false);
}

//========================================
// 色設定
//========================================
void CMotionModel::ChangeColar(D3DXCOLOR color)
{
	SetColor(color);
	SetIsColor(true);
}