//========================================
// 
// モーション処理
// 
//========================================
// *** motion.cpp ***
//========================================
#include "motion.h"
#include "model.h"
#include "parts.h"
#include "../../system/texture.h"
#include "../../manager.h"
#include "../../system/renderer.h"


//========================================
// コンストラクタ
//========================================
CMotion::CMotion(const char *pFileName)
{
	// 値をクリアする
	memset(&m_partsFile, 0, sizeof(m_partsFile));	// パーツ名
	m_motion = NULL;								// モーションを
	m_ppParts = NULL;								// パーツ
	m_nMaxParts = 0;								// パーツ数
	LoodMotion(pFileName);							// モーション読み込み
	m_nNumMotion = 0;								// 扱うモーション
	m_bMotion = false;								// モーションを行うか
	m_bMotionBlend = false;							// モーションブレンド
}

//========================================
// デストラクタ
//========================================
CMotion::~CMotion()
{

}

//========================================
// 初期化
//========================================
void CMotion::Init(void)
{
	// カウントのリセット
	for (int nCntMotion = 0; nCntMotion < MYMAX_MOTION; nCntMotion++)
	{
		CntReset(nCntMotion);
	}
}

//========================================
// 終了
//========================================
void CMotion::Uninit(void)
{
	for (int nCntMotion = 0; nCntMotion < MYMAX_MOTION; nCntMotion++)
	{
		if (&m_motion[nCntMotion] != NULL)
		{// メモリの解放
			if (m_motion[nCntMotion].pKeySet != NULL)
			{
				for (int nCntKey = 0; nCntKey < m_motion[nCntMotion].nNumKey; nCntKey++)
				{
					if (m_motion[nCntMotion].pKeySet[nCntKey].pKey != NULL)
					{
						delete[] m_motion[nCntMotion].pKeySet[nCntKey].pKey;
						m_motion[nCntMotion].pKeySet[nCntKey].pKey = NULL;
					}
				}
				delete[] m_motion[nCntMotion].pKeySet;
				m_motion[nCntMotion].pKeySet = NULL;
			}
		}
	}

	if (m_motion != NULL)
	{// メモリの解放
		delete[] m_motion;
		m_motion = NULL;
	}

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		if (m_ppParts[nCntParts] != NULL)
		{
			delete m_ppParts[nCntParts];
			m_ppParts[nCntParts] = nullptr;
		}
	}

	if (m_ppParts != nullptr)
	{
		delete[] m_ppParts;
		m_ppParts = nullptr;
	}
}

//========================================
// 更新
//========================================
void CMotion::Update(void)
{
	if (m_bMotionBlend)
	{
		MotionBlend();
	}
	else if (m_bMotion
		&& !m_bMotionBlend)
	{
		PlayMotion();
	}
}

//========================================
//----------------------------------------
// パーツ
//----------------------------------------
//========================================

//========================================
// パーツ設定
//========================================
void CMotion::SetParts(D3DXMATRIX mtxWorld, const bool IsColor, const D3DXCOLOR color)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{// モデルの描画
		if (IsColor)
		{
			m_ppParts[nCntParts]->SetColor(color);
		}

		//　親モデルが指定されているか
		if (m_ppParts[nCntParts]->GetParent() != NULL)
		{
			m_ppParts[nCntParts]->Draw(IsColor);
		}
		else
		{
			m_ppParts[nCntParts]->Draw(mtxWorld, IsColor);
		}
	}

	// 新規深度値とZバッファの深度値が同じ値ならテスト成功にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//========================================
// 元の場所に配置
//========================================
void CMotion::SetPartsOrigin(void)
{
	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 位置の設定
		m_ppParts[nCntParts]->SetPos(m_ppParts[nCntParts]->GetPosOrigin());

		//	向きの設定
		m_ppParts[nCntParts]->SetRot(m_ppParts[nCntParts]->GetRotOrigin());
	}
}

//========================================
//----------------------------------------
// モーション
//----------------------------------------
//========================================

//========================================
// 初期設定
//========================================
void CMotion::SetMotion(const int nCntMotionSet)
{
	CMotion::Motion *motion = (m_motion + nCntMotionSet);

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 変数宣言
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
		D3DXVECTOR3 posOrigin = m_ppParts[nCntParts]->GetPosOrigin();		// 元の位置
		D3DXVECTOR3 rotOrigin = m_ppParts[nCntParts]->GetRotOrigin();		// 元の向き

		// 位置の設定
		pos = (posOrigin + motion->pKeySet[motion->nCntKeySet].pKey[nCntParts].pos);

		//	向きの設定
		rot = (rotOrigin + motion->pKeySet[motion->nCntKeySet].pKey[nCntParts].rot);

		// 角度の正規化
		NormalizeAngle(&rot.x);
		NormalizeAngle(&rot.y);
		NormalizeAngle(&rot.z);

		// 情報の更新
		m_ppParts[nCntParts]->SetPos(pos);
		m_ppParts[nCntParts]->SetRot(rot);
	}
}

//========================================
// モーション番号の設定
//========================================
void CMotion::SetNumMotion(const int nNumMotion, bool bStop)
{
	if (!m_bMotion || bStop)
	{
		// モーションカウントのリセット
		CntReset(m_nNumMotion);

		// モーション番号の設定
		m_nNumMotion = nNumMotion;

		// モーションブレンドを行う
		m_bMotionBlend = true;
		m_bMotion = true;
	}
}

//========================================
// モーションの再生
//========================================
void CMotion::PlayMotion(void)
{
	CMotion::Motion *motion = (m_motion + m_nNumMotion);

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 変数宣言
		D3DXVECTOR3 pos = m_ppParts[nCntParts]->GetPos();			// 位置
		D3DXVECTOR3 rot = m_ppParts[nCntParts]->GetRot();			// 向き
		D3DXVECTOR3 posDest = m_ppParts[nCntParts]->GetPosDest();	// 目的の位置
		D3DXVECTOR3 rotDest = m_ppParts[nCntParts]->GetRotDest();	// 目的の向き

		if (motion->nCntFrame)
		{// フレームカウントが0の時
		 // 変数宣言
			D3DXVECTOR3 posOrigin = m_ppParts[nCntParts]->GetPosOrigin();		// 元の位置
			D3DXVECTOR3 rotOrigin = m_ppParts[nCntParts]->GetRotOrigin();		// 元の向き

			// 目的の位置と向きの算出
			posDest = (posOrigin + motion->pKeySet[motion->nCntKeySet].pKey[nCntParts].pos) - pos;
			rotDest = (rotOrigin + motion->pKeySet[motion->nCntKeySet].pKey[nCntParts].rot) - rot;

			// 角度の正規化
			NormalizeAngle(&rotDest.x);
			NormalizeAngle(&rotDest.y);
			NormalizeAngle(&rotDest.z);

			// 情報の更新
			m_ppParts[nCntParts]->SetPosDest(posDest);
			m_ppParts[nCntParts]->SetRotDest(rotDest);
		}

		// 変数宣言
		D3DXVECTOR3 addPos = D3DXVECTOR3(posDest / (float)(motion->pKeySet[motion->nCntKeySet].nFrame));
		D3DXVECTOR3 addRot = D3DXVECTOR3(rotDest / (float)(motion->pKeySet[motion->nCntKeySet].nFrame));

		// 位置の加算
		pos += addPos;

		//	向きの加算
		rot += addRot;

		// 角度の正規化
		NormalizeAngle(&rot.x);
		NormalizeAngle(&rot.y);
		NormalizeAngle(&rot.z);

		// 情報の更新
		m_ppParts[nCntParts]->SetPos(pos);
		m_ppParts[nCntParts]->SetRot(rot);
	}

	// フレームカウントの加算
	motion->nCntFrame++;

	if (motion->nCntFrame >= motion->pKeySet[motion->nCntKeySet].nFrame)
	{// フレームカウントが指定のフレーム数を超えた場合
	 // フレーム数の初期化
		motion->nCntFrame = 0;

		// 再生中のキー番号数の加算
		motion->nCntKeySet++;

		if (motion->nCntKeySet >= motion->nNumKey && motion->bLoop)
		{// 再生中のキー数カウントがキー数の最大値を超えたとき、そのモーションがループを使用している
		 // 再生中のキー数カウントを初期化
			motion->nCntKeySet = 0;

		}
		else if (motion->nCntKeySet >= motion->nNumKey)
		{
			motion->nCntKeySet = 0;
			m_bMotion = false;
		}
	}
}

//========================================
// モーションブレンド
//========================================
void CMotion::MotionBlend(void)
{
	CMotion::Motion* motion = (m_motion + m_nNumMotion);

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 変数宣言
		D3DXVECTOR3 pos = m_ppParts[nCntParts]->GetPos();			// 位置
		D3DXVECTOR3 rot = m_ppParts[nCntParts]->GetRot();			// 向き
		D3DXVECTOR3 posDest = m_ppParts[nCntParts]->GetPosDest();	// 目的の位置
		D3DXVECTOR3 rotDest = m_ppParts[nCntParts]->GetRotDest();	// 目的の向き

		if (motion->nCntFrame == 0)
		{// フレームカウントが0の時
		 // 変数宣言
			D3DXVECTOR3 posOrigin = m_ppParts[nCntParts]->GetPosOrigin();		// 元の位置
			D3DXVECTOR3 rotOrigin = m_ppParts[nCntParts]->GetRotOrigin();		// 元の向き

			// 目的の位置と向きの算出
			CMotion::Key myKey = motion->pKeySet[motion->nCntKeySet].pKey[nCntParts];
			posDest = posOrigin + myKey.pos - pos;
			rotDest = rotOrigin + myKey.rot - rot;

			// 角度の正規化
			NormalizeAngle(&rotDest.x);
			NormalizeAngle(&rotDest.y);
			NormalizeAngle(&rotDest.z);
			
			// 情報の更新
			m_ppParts[nCntParts]->SetPosDest(posDest);
			m_ppParts[nCntParts]->SetRotDest(rotDest);
		}

		// 変数宣言
		D3DXVECTOR3 addPos = D3DXVECTOR3(posDest / (float)(MOTION_BLEND_FRAM));
		D3DXVECTOR3 addRot = D3DXVECTOR3(rotDest / (float)(MOTION_BLEND_FRAM));

		// 位置の加算
		pos += addPos;

		//	向きの加算
		rot += addRot;

		// 角度の正規化
		NormalizeAngle(&rot.x);
		NormalizeAngle(&rot.y);
		NormalizeAngle(&rot.z);

		// 情報の更新
		m_ppParts[nCntParts]->SetPos(pos);
		m_ppParts[nCntParts]->SetRot(rot);
	}

	// フレームカウントの加算
	motion->nCntFrame++;

	if (motion->nCntFrame >= MOTION_BLEND_FRAM)
	{// フレームカウントが指定のフレーム数を超えた場合

		motion->nCntFrame = 0;	// フレーム数の初期化
		motion->nCntKeySet++;	// 再生中のキー番号数の加算

		m_bMotionBlend = false;
	}
}

//========================================
// パーツとモーションの読み込み
//========================================
void CMotion::LoodMotion(const char *pFileName)
{
	// 変数宣言
	char aDataSearch[128] = {};		// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数
	int	nCntKeySet = 0;				// KeySetカウント
	int	nCntKey = 0;				// Keyカウント
	int nCntFileName = 0;			// ファイル名
	int nCntParts = 0;				// パーツ数のカウント
	int nCntMotion = 0;				// モーション数のカウント

		// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(pFileName, "r");


	// ファイルが開けたら
	if (pFile != NULL)
	{//ファイルが開いた場合

		// END_SCRIPTが見つかるまで読み込みを繰り返す
		while (1)
		{
			fscanf(pFile, "%s", aDataSearch);	// 検索

			if (!strcmp(aDataSearch, "END_SCRIPT"))
			{// 読み込みを終了
				fclose(pFile);
				break;
			}
			else if (aDataSearch[0] == '#')
			{// 折り返す
				continue;
			}
			else if (!strcmp(aDataSearch, "MODEL_FILENAME"))
			{// ファイル名の読み込み
				fscanf(pFile, "%s", &g_aEqual[0]);
				fscanf(pFile, "%s", &m_partsFile[nCntFileName][0]);
				nCntFileName++;
			}
			else if (!strcmp(aDataSearch, "CHARACTERSET"))
			{// キャラクターの読み込み
				while (1)
				{
					fscanf(pFile, "%s", aDataSearch);	// 検索

					if (!strcmp(aDataSearch, "END_CHARACTERSET"))
					{// 読み込みを終了
						break;
					}
					else if (!strcmp(aDataSearch, "NUM_PARTS"))
					{// 読み込むパーツ数
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &m_nMaxParts);

						// メモリの解放
						m_ppParts = new CParts*[m_nMaxParts];
						m_motion = new Motion[MYMAX_MOTION];
						for (int i = 0; i < MYMAX_MOTION; i++)
						{
							m_motion[i].pKeySet = NULL;
						}
						assert(m_ppParts != NULL && m_motion != NULL);

						for (int i = 0; i < m_nMaxParts; i++)
						{// パーツの生成
							m_ppParts[i] = CParts::Create();
						}
					}
					else if (!strcmp(aDataSearch, "PARTSSET"))
					{// パーツの読み込み
						while (1)
						{
							fscanf(pFile, "%s", aDataSearch);	// 検索

							if (!strcmp(aDataSearch, "END_PARTSSET"))
							{// 読み込みを終了
								nCntParts++;	// パーツ情報の加算
								break;
							}
							else if (!strcmp(aDataSearch, "PARENT"))
							{// 親の読み込み
								int nIdxParent = -1;
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nIdxParent);

								if (nIdxParent != -1)
								{// 親のモデルの設定
									m_ppParts[nCntParts]->SetParent(m_ppParts[nIdxParent]);
								}
							}
							else if (!strcmp(aDataSearch, "INDEX"))
							{// タイプの読み込み
								int nType = -1;
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nType);

								// マテリアル情報の取得
								CModel::MODEL_MATERIAL *pMaterial = CModel::GetMaterial();

								for (int nCntModel = 0; nCntModel < CModel::GetMaxModel(); nCntModel++)
								{
									if (strcmp(&m_partsFile[nType][0], &pMaterial[nCntModel].aFileName[0]) == 0)
									{// モデルのIDの設定
										m_ppParts[nCntParts]->SetModelID(nCntModel);
										break;
									}
								}
							}
							else if (!strcmp(aDataSearch, "POS"))
							{// 位置の読み込み
								D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
								m_ppParts[nCntParts]->SetPos(pos);
								m_ppParts[nCntParts]->SetPosOrigin(pos);
							}
							else if (!strcmp(aDataSearch, "ROT"))
							{// 向きの読み込み
								D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
								m_ppParts[nCntParts]->SetRot(rot);
								m_ppParts[nCntParts]->SetRotOrigin(rot);
							}
						}
					}
				}
			}
			else if (!strcmp(aDataSearch, "MOTIONSET"))
			{// モーションの読み込み
				while (1)
				{
					fscanf(pFile, "%s", aDataSearch);	// 検索

					if (!strcmp(aDataSearch, "END_MOTIONSET"))
					{// 読み込みを終了
						nCntKeySet = 0;	// キー設定の初期化
						nCntMotion++;	// モーション情報の加算
						break;
					}
					else if (!strcmp(aDataSearch, "LOOP"))
					{// ループ有無の読み込み
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", (int*)(&m_motion[nCntMotion].bLoop));
					}
					else if (!strcmp(aDataSearch, "NUM_KEY"))
					{// キー数の読み込み
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &m_motion[nCntMotion].nNumKey);

						// メモリの確保
						m_motion[nCntMotion].pKeySet = new KeySet[m_motion[nCntMotion].nNumKey];
						assert(m_motion[nCntMotion].pKeySet != NULL);

						for (int nCntNumKeySet = 0; nCntNumKeySet < m_motion[nCntMotion].nNumKey; nCntNumKeySet++)
						{
							m_motion[nCntMotion].pKeySet[nCntNumKeySet].pKey = new Key[m_nMaxParts];
							assert(m_motion[nCntMotion].pKeySet[nCntNumKeySet].pKey != NULL);
						}

					}
					else if (!strcmp(aDataSearch, "KEYSET"))
					{// キーセットの読み込み
						while (1)
						{
							fscanf(pFile, "%s", aDataSearch);	// 検索

							if (!strcmp(aDataSearch, "END_KEYSET"))
							{// 読み込みを終了

								nCntKey = 0;	// キーカウントの初期化
								nCntKeySet++;	// キーセットの加算
								break;
							}
							else if (!strcmp(aDataSearch, "FRAME"))
							{// フレーム数の読み込み
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &m_motion[nCntMotion].pKeySet[nCntKeySet].nFrame);
							}
							else if (!strcmp(aDataSearch, "KEY"))
							{// キーの読み込み
								while (1)
								{
									fscanf(pFile, "%s", aDataSearch);	// 検索

									if (!strcmp(aDataSearch, "END_KEY"))
									{// 読み込みを終了
										nCntKey++;	// キーカウントの加算
										break;
									}
									else if (!strcmp(aDataSearch, "POS"))
									{// 位置の読み込み
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &m_motion[nCntMotion].pKeySet[nCntKeySet].pKey[nCntKey].pos.x);
										fscanf(pFile, "%f", &m_motion[nCntMotion].pKeySet[nCntKeySet].pKey[nCntKey].pos.y);
										fscanf(pFile, "%f", &m_motion[nCntMotion].pKeySet[nCntKeySet].pKey[nCntKey].pos.z);
									}
									else if (!strcmp(aDataSearch, "ROT"))
									{
										// 向きの読み込み
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &m_motion[nCntMotion].pKeySet[nCntKeySet].pKey[nCntKey].rot.x);
										fscanf(pFile, "%f", &m_motion[nCntMotion].pKeySet[nCntKeySet].pKey[nCntKey].rot.y);
										fscanf(pFile, "%f", &m_motion[nCntMotion].pKeySet[nCntKeySet].pKey[nCntKey].rot.z);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{//ファイルが開けない場合
		assert(false);

		// 処理を終了する
		return;
	}
	// 初期化
	Init();
}

//========================================
// カウントリセット
//========================================
void CMotion::CntReset(const int nNumMotionOld)
{
	m_motion[nNumMotionOld].nCntFrame = 0;
	m_motion[nNumMotionOld].nCntKeySet = 0;
}

//=============================================================================
// モーション正規化
//=============================================================================
float CMotion::NormalizeAngle(float *pAngle)
{
	if (*pAngle >= D3DX_PI)
	{// 3.14より大きい
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle <= -D3DX_PI)
	{// -3.14より小さい
		*pAngle += D3DX_PI * 2.0f;
	}
	return *pAngle;
}
