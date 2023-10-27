//========================================
// 
// モデル処理
// 
//========================================
// *** objectX.cpp ***
//========================================
#include "model.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../system/texture.h"

//========================================
// 静的変数
//========================================
CModel::MODEL_MATERIAL *CModel::m_material = NULL;	// マテリアル情報
int CModel::m_nMaxModel = 0;						// モデル数		
float CModel::m_Width = NULL;
float CModel::m_Height = NULL;
float CModel::m_Depth = NULL;
bool CModel::m_bPlayerShadow = true;

//========================================
//マクロ定義
//========================================
#define SHADOW_MINPOS (D3DXVECTOR3(0.0f,-12.5f,0.0f))	// 影の最低位置

//========================================
// コンストラクタ
//========================================
CModel::CModel()
{
	// 値をクリアする
	m_pTexture = NULL;
	m_pParent = NULL;				// 親モデルの情報
	m_mtxWorld = {};				// ワールドマトリックス
	m_pos = INIT_D3DXVECTOR3;		// 位置
	m_posOld = INIT_D3DXVECTOR3;	// 位置(過去)
	m_rot = INIT_D3DXVECTOR3;		// 向き
	m_size = INIT_D3DXVECTOR3;
	m_scale = INIT_D3DXVECTOR3;		// 大きさ
	m_color = INIT_D3DXCOLOR;		// 色
	m_nModelID = -1;				// モデルID
}

//========================================
// デストラクタ
//========================================
CModel::~CModel()
{

}

//========================================
// 生成
//========================================
CModel *CModel::Create(void)
{
	CModel *pModel = new CModel;

	//ポインタが確保された
	if (pModel != NULL)
	{
		// 初期化処理
		pModel->Init();

		return pModel;
	}

	//ポインタが確保されていなければ、NULLを返す
	return NULL;
}

//========================================
// モデル情報の初期化
//========================================
void CModel::InitModel(void)
{
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ファイル読み込み
	LoadModel("data\\GAMEDATA\\MODEL\\MODEL_DATA.txt");

	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{// Xファイルの読み込み
		D3DXLoadMeshFromX(&m_material[nCntModel].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_material[nCntModel].pBuffer,
			NULL,
			&m_material[nCntModel].nNumMat,
			&m_material[nCntModel].pMesh);

		// マテリアルのテクスチャ情報のメモリ確保
		m_material[nCntModel].pIdxTex = new int[m_material[nCntModel].nNumMat];

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_material[nCntModel].pBuffer->GetBufferPointer();

		CTexture* pTexture = CManager::GetTexture();

		for (int nCntMat = 0; nCntMat < (int)m_material[nCntModel].nNumMat; nCntMat++)
		{
			// マテリアルのテクスチャ情報の初期化
			m_material[nCntModel].pIdxTex[nCntMat] = -1;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{// テクスチャファイルが存在する

				// テクスチャの読み込み
				m_material[nCntModel].pIdxTex[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
			}
		}
		// 頂点座標の最小値・最大値の算出
		CalcSize(nCntModel);
	}
}

//========================================
// モデル情報の終了
//========================================
void CModel::UninitModel(void)
{
	for (int nCnt = 0; nCnt < m_nMaxModel; nCnt++)
	{
		// メッシュの破棄
		if (m_material[nCnt].pMesh != NULL)
		{
			m_material[nCnt].pMesh->Release();
			m_material[nCnt].pMesh = NULL;
		}

		// マテリアルの破棄
		if (m_material[nCnt].pBuffer != NULL)
		{
			m_material[nCnt].pBuffer->Release();
			m_material[nCnt].pBuffer = NULL;
		}

		// メモリの解放
		delete[] m_material[nCnt].pIdxTex;
		m_material[nCnt].pIdxTex = NULL;
	}

	// メモリの解放
	delete[] m_material;
	m_material = NULL;
}

//========================================
// Xファイルの読み込み
//========================================
void CModel::LoadModel(const char *pFileName)
{
	int nCntModel = 0;
	char aDataSearch[TXT_MAX];	// データ検索用

	// ファイルの読み込み
	FILE *pFile = fopen(pFileName, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))
		{// 読み込みを終了
			fclose(pFile);
			break;
		}
		if (aDataSearch[0] == '#')
		{// 折り返す
			continue;
		}

		if (!strcmp(aDataSearch, "NUM_MODEL"))
		{
			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &m_nMaxModel);		// 最大数
			m_material = new CModel::MODEL_MATERIAL[m_nMaxModel];
			assert(m_material != NULL);
		}
		else if (!strcmp(aDataSearch, "MODEL"))
		{
			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%s", &m_material[nCntModel].aFileName[0]);	// ファイル名
			fscanf(pFile, "%d", &m_material[nCntModel].bShadow);		// 影の有無
			nCntModel++;
		}
	}
}

//========================================
// 初期化
//========================================
HRESULT CModel::Init()
{
	// メンバ変数の初期化
	m_pos = INIT_D3DXVECTOR3;					// 位置
	m_rot = INIT_D3DXVECTOR3;					// 向き
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 大きさ
	m_nModelID = -1;							// モデルID

	return S_OK;
}

//========================================
// 終了
//========================================
void CModel::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CModel::Update(void)
{

}

//================================================================================
// 描画 
// 親モデルが指定されてる場合、親のワールドマトリックス元に描画する
//================================================================================
void CModel::Draw(bool Color)
{
	if (m_nModelID >= 0 && m_nModelID < m_nMaxModel)
	{
		// 親のワールドマトリックス
		D3DXMATRIX mtxParent = {};

		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

		// 現在のマテリアル保存用
		D3DMATERIAL9 matDef;

		// マテリアルデータへのポインタ
		D3DXMATERIAL *pMat;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// サイズの反映
		D3DXMatrixScaling(&mtxScaling, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		if (m_pParent != NULL)
		{
			mtxParent = m_pParent->GetMtxWorld();

			// 行列掛け算関数
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		if (m_material[m_nModelID].pBuffer != NULL)
		{// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)m_material[m_nModelID].pBuffer->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)m_material[m_nModelID].nNumMat; nCntMat++)
			{
				if (Color)
				{// マテリアル情報の設定
					D3DMATERIAL9  matD3D = pMat[nCntMat].MatD3D;

					// 引数を色に設定
					matD3D.Diffuse = m_color;

					// マテリアルの設定
					pDevice->SetMaterial(&matD3D);
				}
				else
				{// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

				CTexture *pTexture = CManager::GetTexture();
				m_pTexture = pTexture->GetAddress(m_material[m_nModelID].pIdxTex[nCntMat]);

				// テクスチャの設定
				pDevice->SetTexture(0, m_pTexture);

				// モデルパーツの描画
				m_material[m_nModelID].pMesh->DrawSubset(nCntMat);
			}

			// 影の描画
			SetShadow(pDevice, pMat, m_nModelID);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//========================================
// 描画
//========================================
void CModel::Draw(D3DXMATRIX mtxParent, bool Color)
{
	if (m_nModelID >= 0 && m_nModelID < m_nMaxModel)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

		// 現在のマテリアル保存用
		D3DMATERIAL9 matDef;

		// マテリアルデータへのポインタ
		D3DXMATERIAL *pMat;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// サイズの反映
		D3DXMatrixScaling(&mtxScaling, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// 行列掛け算関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		if (m_material[m_nModelID].pBuffer != NULL)
		{// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)m_material[m_nModelID].pBuffer->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)m_material[m_nModelID].nNumMat; nCntMat++)
			{
				if (Color)
				{// マテリアル情報の設定
					D3DMATERIAL9  matD3D = pMat[nCntMat].MatD3D;

					// 引数を色に設定
					matD3D.Diffuse = m_color;

					// マテリアルの設定
					pDevice->SetMaterial(&matD3D);
				}
				else
				{// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

				CTexture *pTexture = CManager::GetTexture();
				m_pTexture = pTexture->GetAddress(m_material[m_nModelID].pIdxTex[nCntMat]);

				// テクスチャの設定
				pDevice->SetTexture(0, m_pTexture);

				// モデルパーツの描画
				m_material[m_nModelID].pMesh->DrawSubset(nCntMat);
			}
		
			// 影の描画
			SetShadow(pDevice, pMat, m_nModelID);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//========================================
// 影の設定
//========================================
void CModel::SetShadow(LPDIRECT3DDEVICE9 pDevice, D3DXMATERIAL *pMat, int nModelID)
{
	if (m_material[nModelID].bShadow)
	{
		if (m_pos.y >= SHADOW_MINPOS.y)
		{
			D3DLIGHT9	light;			//ライト情報
			D3DXVECTOR4	posLight;		//ライトの位置
			D3DXVECTOR3	pos, normal;	//平面上の任意の点、法線ベクトル
			D3DXPLANE	plane;			//平面情報

			//ライトの位置を設定
			pDevice->GetLight(0, &light);
			posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

			//平面情報を生成
			pos = SHADOW_MINPOS;
			normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXPlaneFromPointNormal(&plane, &pos, &normal);

			//シャドウマトリックス
			{
				D3DXMATRIX	mtxShadow;

				//シャドウマトリックスの初期化
				D3DXMatrixIdentity(&mtxShadow);

				//シャドウマトリックスの作成
				D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

				//シャドウマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);
			}

			for (int nCntMat = 0; nCntMat < (int)m_material[nModelID].nNumMat; nCntMat++)
			{
				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)m_material[nModelID].pBuffer->GetBufferPointer();

				D3DMATERIAL9 MatCopy = InitMtrl(
					D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					0.0f);

				//黒色に設定
				MatCopy.Diffuse.a = 1.0f;

				//マテリアル設定
				pDevice->SetMaterial(&MatCopy);

				//テクスチャ設定
				pDevice->SetTexture(0, NULL);

				// モデルパーツの描画
				m_material[nModelID].pMesh->DrawSubset(nCntMat);
			}
		}
	}

}

//========================================
// 頂点座標の最小値・最大値の算出
//========================================
void CModel::CalcSize(int nCntModel)
{
	int		nNumVtx;	// 頂点数
	DWORD	sizeFVF;	// 頂点フォーマットのサイズ
	BYTE	*pVtxBuff;	// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = m_material[nCntModel].pMesh->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(m_material[nCntModel].pMesh->GetFVF());

	// 頂点バッファのロック
	m_material[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 最も大きな頂点
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < vtxMin.x)
		{// 比較対象が現在の頂点座標(X)の最小値より小さい
			vtxMin.x = vtx.x;
		}
		if (vtx.y < vtxMin.y)
		{// 比較対象が現在の頂点座標(Y)の最小値より小さい
			vtxMin.y = vtx.y;
		}
		if (vtx.z < vtxMin.z)
		{// 比較対象が現在の頂点座標(Z)の最小値より小さい
			vtxMin.z = vtx.z;
		}

		if (vtx.x > vtxMax.x)
		{// 比較対象が現在の頂点座標(X)の最大値より大きい
			vtxMax.x = vtx.x;
		}
		if (vtx.y > vtxMax.y)
		{// 比較対象が現在の頂点座標(Y)の最大値より大きい
			vtxMax.y = vtx.y;
		}
		if (vtx.z > vtxMax.z)
		{// 比較対象が現在の頂点座標(Z)の最大値より大きい
			vtxMax.z = vtx.z;
		}

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	// 頂点バッファのアンロック
	m_material[nCntModel].pMesh->UnlockVertexBuffer();

	// 大きさの設定
	m_material[nCntModel].size = D3DXVECTOR3(vtxMax.x - vtxMin.x, vtxMax.y - vtxMin.y, vtxMax.z - vtxMin.z);

	m_material[nCntModel].fWidth = vtxMax.x;
	m_material[nCntModel].fHeight = vtxMax.y;
	m_material[nCntModel].fDepth = vtxMax.z;
}

//========================================
// 当たり判定の取得
//========================================
D3DXVECTOR3 CModel::GetSize(const int nModelID)
{
	m_size = m_material[nModelID].size;

	return m_size;
}

//========================================
// 幅の取得
//========================================
float CModel::GetWidth(const int nModelID)
{
	m_Width = m_material[nModelID].fWidth;

	return m_Width;
}

//========================================
// 高さの取得
//========================================
float CModel::GetHeight(const int nModelID)
{
	m_Height = m_material[nModelID].fHeight;

	return m_Height;
}

//========================================
// 奥行きの取得
//========================================
float CModel::GetDepth(const int nModelID)
{
	m_Depth = m_material[nModelID].fDepth;

	return m_Depth;
}

//========================================
// マテリアルの初期化
//========================================
D3DMATERIAL9 CModel::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

//========================================
// プレイヤー影の表示・非表示
//========================================
void CModel::SetPlayerShadow(bool bShadow)
{
	if (bShadow)
	{
		if (!m_bPlayerShadow)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				m_material[nCnt + CPhysics::MODEL_PLAYER_00].bShadow = true;
			}
			m_bPlayerShadow = true;
		}
	}
	else
	{
		if (m_bPlayerShadow)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				m_material[nCnt + CPhysics::MODEL_PLAYER_00].bShadow = false;
			}
			m_bPlayerShadow = false;
		}
	}
}