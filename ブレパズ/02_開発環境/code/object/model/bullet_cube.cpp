//========================================
// 
// キューブ処理
// 
//========================================
// *** block.cpp ***
//========================================
#include "model.h"
#include "enemy.h"
#include "block.h"
#include "bullet_cube.h"
#include "../../scene/pause.h"
#include "../EFFECT/particleX.h"
#include "../../system/physics.h"
#include "../../system/sound.h"
#include "../../system/csv_file.h"
#include "../../system/words/text.h"
#include "../../system/words/words.h"
#include "../../system/words/font.h"

// 静的変数
int CCube::m_nNumAll = -1;
int CCube::m_nNumChain = 1;
int CCube::m_nLimitCube = 0;
int CCube::m_nRestCube = 0;
int CCube::m_nUseCube = 0;
bool CCube::bLeadSet = false;
CText *CCube::m_Cube = NULL;

//========================================
// マクロ定義
//========================================
#define RADIUS_TIME			(20)	// 半径・推移時間
#define SIZE_DIAMETER		(2.0f)	// サイズの倍率
#define COLLSION_DIAMETER	(1.0f)	// 判定の倍率
#define BOM_COLLSION		(2.0f)	// 爆発の判定
#define DAMAGE_DIAMETER		(2)		// 連続爆破のダメージの割合		BOM_DAMAGE　+ (連鎖カウント / DAMAGE_DIAMETER)
#define BOM_DAMAGE			(1)		// 爆発ダメージ

//========================================
// コンストラクタ
//========================================
CCube::CCube(int nPriority) : CObjectX(nPriority)
{
	m_nNumAll++;	// 総数を加算

	// 値をクリア
	m_Info.pos = INIT_D3DXVECTOR3;		// 位置
	m_Info.posOld = INIT_D3DXVECTOR3;	// 位置(過去)
	m_Info.movePos = INIT_D3DXVECTOR3;	// 移動位置
	m_Info.targetPos = INIT_D3DXVECTOR3;// 目的位置
	m_Info.rot = INIT_D3DXVECTOR3;		// 向き
	m_Info.rotOld = INIT_D3DXVECTOR3;	// 向き(過去)
	m_Info.size = INIT_D3DXVECTOR3;		// 大きさ
	m_Info.col = INIT_D3DXCOLOR;		// 色
	m_Info.nShape = -1;					// 形状
	m_Info.nStandTime = 0;				// 待機時間
	m_Info.nLife = 0;					// 寿命
	m_Info.nLifeMax = 0;				// 寿命の最大値
	m_Info.fRadius = 0.0f;				// 半径
	m_Info.nCntRadius = 0;				// 半径推移	
	m_Info.fRadiusRate = 0.0f;			// 半径の割合
	m_Info.bSet = false;				// 配置フラグ
	m_Info.nChain = 0;					// 連鎖
	m_Info.bBom = false;				// 爆発
	m_Info.nID = m_nNumAll;				// 自分自身のID
	m_Info.bErase = false;
}

//========================================
// デストラクタ
//========================================
CCube::~CCube()
{
	m_nNumAll--;	// 総数減算
}

//========================================
// 生成
//========================================
CCube *CCube::Create(int nShape, D3DXVECTOR3 pos, int nLife)
{
	CCube *pCube = NULL;

	if (pCube != NULL)
	{
		return pCube;
	}

	// キューブの生成
	pCube = new CCube;

	pCube->m_Info.nCntRadius = RADIUS_TIME;
	pCube->m_Info.fRadiusRate = 0.0f;
	pCube->m_Info.bSet = false;
	pCube->SetModel(MODEL_BULLET + nShape);

	// 初期化処理
	pCube->Init();

	int nCubeLife = nLife;
	
	pCube->m_Info.nShape = nShape;
	pCube->m_Info.nLife = nCubeLife;
	pCube->m_Info.nLifeMax = nCubeLife;
	pCube->SetCubePos(pos);
	pCube->SetPos(pos);

	pCube->m_Info.posOld = pCube->m_Info.pos;

	// 配置場所にプレイヤーがいるか
	pCube->ModelCollsion(PRIO_OBJECT, TYPE_PLAYER, VECTOR_NONE, pCube->m_Info.pos);
	pCube->ModelCollsion(PRIO_BLOCK, TYPE_BLOCK, VECTOR_NONE, pCube->m_Info.pos);

	if (pCube->m_Info.bErase)
	{
		pCube->Uninit();
		return NULL;
	}

	// --- 取得 ---------------------------------
	CSound *pSound = CManager::GetSound();

	pSound->PlaySound(CSound::TYPE_SET);

	// 使用数の加算
	m_nUseCube++;

	// テキストの更新
	CubeText();

	// 位置補正
	/* X軸	*/if (pCube->Correction(VECTOR_X, pCube->m_Info.pos)) { return pCube; }
	/* Y軸	*/if (pCube->Correction(VECTOR_Y, pCube->m_Info.pos)) { return pCube;}
	/* Z軸	*/if (pCube->Correction(VECTOR_Z, pCube->m_Info.pos)) { return pCube;}

	return pCube;
}

//========================================
// 初期化
//========================================
HRESULT CCube::Init(void)
{
	CObjectX::Init();

	if (m_nLimitCube - m_nUseCube <= 0)
	{
		Uninit();
		return S_OK;
	}

	// 種類の設定
	SetType(TYPE_CUBE);

	m_Info.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Info.col = D3DXCOLOR(0.1f, 0.9f, 0.9f, 1.0f);
	m_Info.nShape = 0;
	m_Info.fRadius = 1.0f;

	// 生成
	SetPos(m_Info.pos);
	SetScale(m_Info.size);
	SetColor(m_Info.col);

	// １つ目のキューブなら
	if (m_Info.nID == 0)
	{
		// 先頭フラグを真にする
		bLeadSet = true;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CCube::Uninit(void)
{
	if (m_Info.bSet)
	{
		m_nRestCube--;	// キューブの残り数
	}

	CObjectX::Uninit();
}

//========================================
// 更新
//========================================
void CCube::Update(void)
{
	bool bPause = CPause::IsPause();

	if (!bPause)
	{
		// 過去の位置・向きの更新
		m_Info.posOld = m_Info.pos;
		m_Info.rotOld = m_Info.rot;

		// 半径推移
		if (m_Info.bSet == false)
		{
			m_Info.fRadiusRate = (float)m_Info.nCntRadius / (float)RADIUS_TIME;
			m_Info.fRadius = 1 * (1.0f - m_Info.fRadiusRate);

			if (--m_Info.nCntRadius <= 0)
			{
				m_Info.bSet = true;
			}
		}

		// 寿命処理
		if (m_Info.bSet)
		{
			// 寿命
			if (--m_Info.nLife <= 0)
			{
				// １つ目のキューブなら
				if (m_Info.nID == 0)
				{
					// 先頭フラグを偽にする
					bLeadSet = false;
				}

				// パーティクル生成
				CParticleX *pObj = CParticleX::Create();
				pObj->Par_SetPos(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, m_Info.pos.z));
				pObj->Par_SetRot(INIT_D3DXVECTOR3);
				pObj->Par_SetMove(D3DXVECTOR3(15.0f, 15.0f, 15.0f));
				pObj->Par_SetType(0);
				pObj->Par_SetLife(100);
				pObj->Par_SetCol(D3DXCOLOR(0.3f, 0.8f, 0.8f, 1.0f));
				pObj->Par_SetForm(10);

				CSound *pSound = CManager::GetSound();
				pSound->PlaySound(CSound::TYPE_ERASE);

				// 周囲にキューブがあるか
				Contact(0, VECTOR_X, m_Info.pos);
				Contact(0, VECTOR_Y, m_Info.pos);
				Contact(0, VECTOR_Z, m_Info.pos);

				// 当たり判定
				/* 敵		*/ModelCollsion(PRIO_OBJECT, TYPE_ENEMY, VECTOR_X, m_Info.pos);
				/* 敵		*/ModelCollsion(PRIO_OBJECT, TYPE_ENEMY, VECTOR_Z, m_Info.pos);
				/* ブロック	*/ModelCollsion(PRIO_BLOCK, TYPE_BLOCK, VECTOR_X, m_Info.pos);
				/* ブロック	*/ModelCollsion(PRIO_BLOCK, TYPE_BLOCK, VECTOR_Z, m_Info.pos);

				// オブジェクト破棄
				Uninit();

				m_nNumChain = 1;

				return;
			}
			else if (m_Info.nLife <= (RADIUS_TIME * 2))
			{
				m_Info.fRadius -= m_Info.fRadius / m_Info.nLife;
			}
			else if (m_Info.nLife <= RADIUS_TIME)
			{
				m_Info.fRadius -= m_Info.fRadius / m_Info.nLife;
				m_Info.col.a *= ((float)m_Info.nLife / RADIUS_TIME);
			}
		}

		// サイズの更新
		m_Info.size = D3DXVECTOR3(m_Info.fRadius, m_Info.fRadius, m_Info.fRadius);

		SetPos(m_Info.pos);
		SetScale(m_Info.size);
		SetColor(m_Info.col);

		CObjectX::Update();
	}
}

//========================================
// 描画
//========================================
void CCube::Draw(void)
{
	CObjectX::Draw();
}

//========================================
// 位置補正
//========================================
bool CCube::Correction(VECTOR vector, D3DXVECTOR3 pos)
{
	// 先頭オブジェクトを取得
	CObject *pObj = CObject::GetTop(PRIO_CUBE);

	while (pObj != NULL)
	{// 使用されている時、

		//　判定フラグ
		bool bHit = false;

		// 次のオブジェクト
		CObject *pObjNext = pObj->GetNext();

		TYPE type;

		// 種類を取得
		type = pObj->GetType();

		if (type == TYPE_CUBE)
		{// 種類がキューブの場合

			// ダイナミックキャストする
			CCube *pCube = dynamic_cast<CCube*>(pObj);

			// IDを取得
			int ID = pCube->GetID();

			if (m_Info.nID != ID)
			{// 自分以外のキューブだったら、

				// 自分自身の取得
				float fWidth = GetWidth();		// 幅
				float fHeight = GetHeight();	// 高さ
				float fDepth = GetDepth();		// 奥行き

				// 相手の取得
				D3DXVECTOR3 PairPos = pObj->GetPos();	// 位置
				float fPairWidth = pObj->GetWidth();	// 幅
				float fPairHeight = pObj->GetHeight();	// 高さ
				float fPairDepth = pObj->GetDepth();	// 奥行き

				//　中心点からの距離
				float fCubeWidth = fPairWidth * SIZE_DIAMETER;		// 幅
				float fCubeHeight = fPairHeight * SIZE_DIAMETER;	// 高さ
				float fCubeDepth = fPairDepth * SIZE_DIAMETER;		// 奥行き

				// サイズ調整
				fPairWidth *= COLLSION_DIAMETER;	// 幅
				fPairHeight *= COLLSION_DIAMETER;	// 高さ
				fPairDepth *= COLLSION_DIAMETER;	// 奥行き

				// 各方向の当たり判定
				D3DXVECTOR3 PairUpPos = D3DXVECTOR3(PairPos.x, PairPos.y + fCubeHeight, PairPos.z);		// 上
				D3DXVECTOR3 PairDownPos = D3DXVECTOR3(PairPos.x, PairPos.y - fCubeHeight, PairPos.z);	// 下
				D3DXVECTOR3 PairLeftPos = D3DXVECTOR3(PairPos.x + fCubeWidth, PairPos.y, PairPos.z);	// 左
				D3DXVECTOR3 PairRightPos = D3DXVECTOR3(PairPos.x - fCubeWidth, PairPos.y, PairPos.z);	// 右
				D3DXVECTOR3 PairBackPos = D3DXVECTOR3(PairPos.x, PairPos.y, PairPos.z + fCubeDepth);	// 奥
				D3DXVECTOR3 PairFrontPos = D3DXVECTOR3(PairPos.x, PairPos.y, PairPos.z - fCubeDepth);	// 手前

				switch (vector)
				{
				case VECTOR_X: {	/* X方向 */

					if (Collsion(pos, PairLeftPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 左側
						m_Info.pos = PairLeftPos;
						bHit = true;
						break;
					}

					if (Collsion(pos, PairRightPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 右側
						m_Info.pos = PairRightPos;
						bHit = true;
						break;
					}
				}
							   break;
				case VECTOR_Y: {	/* Y方向 */

					if (Collsion(pos, PairUpPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 上側
						m_Info.pos = PairUpPos;
						bHit = true;
						break;
					}

					if (Collsion(pos, PairDownPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 下側
						m_Info.pos = PairDownPos;
						bHit = true;
						break;
					}
				}
							   break;
				case VECTOR_Z: {	/* Z方向 */

					if (Collsion(pos, PairBackPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 奥側
						m_Info.pos = PairBackPos;
						bHit = true;
						break;
					}

					if (Collsion(pos, PairFrontPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 手前側
						m_Info.pos = PairFrontPos;
						bHit = true;
						break;
					}
				}
						   break;
				}

				// 判定が真なら TRUE を返す
				if (bHit)
				{	
					if (CubeCollsion(m_Info.pos, m_Info.nID))
					{
						m_Info.bErase = true;
					}

					return TRUE;
				}
			}
		}

		pObj = pObjNext;	// 次のオブジェクトを代入
	}
	return FALSE;
}

//========================================
// キューブの当たり判定
//========================================
bool CCube::CubeCollsion(D3DXVECTOR3 pos, int ID)
{
	// 先頭オブジェクトを取得
	CObject *pObj = CObject::GetTop(PRIO_CUBE);

	while (pObj != NULL)
	{// 使用されている時、

		// 次のオブジェクト
		CObject *pObjNext = pObj->GetNext();

		TYPE type;

		// 種類を取得
		type = pObj->GetType();

		if (type == TYPE_CUBE)
		{// 種類がキューブの場合

			// ダイナミックキャストする
			CCube *pCube = dynamic_cast<CCube*>(pObj);

			// IDを取得
			int nID = pCube->GetID();

			if (ID != nID)
			{// 自分以外のキューブだったら、

				// 自分自身の取得
				float fWidth = GetWidth();		// 幅
				float fHeight = GetHeight();	// 高さ
				float fDepth = GetDepth();		// 奥行き

				// 相手の取得
				D3DXVECTOR3 PairPos = pObj->GetPos();	// 位置
				float fPairWidth = pObj->GetWidth();	// 幅
				float fPairHeight = pObj->GetHeight();	// 高さ
				float fPairDepth = pObj->GetDepth();	// 奥行き

				// サイズ調整
				fWidth *= 0.1f;
				fHeight *= 0.1f;
				fDepth *= 0.1f;
				fPairWidth *= 0.5f;
				fPairHeight *= 0.5f;
				fPairDepth *= 0.5f;

				if (Collsion(pos, PairPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
				{
					return TRUE;
				}
			}
		}

		pObj = pObjNext;	// 次のオブジェクトを代入
	}
	return FALSE;
}

//========================================
// 接触判定
//========================================
bool CCube::Contact(int mode, VECTOR vector, D3DXVECTOR3 pos)
{
	// 先頭オブジェクトを取得
	CObject *pObj = CObject::GetTop(PRIO_CUBE);

	while (pObj != NULL)
	{// 使用されている時、

		bool bHit = false;

		// 次のオブジェクト
		CObject *pObjNext = pObj->GetNext();

		int ID;
		TYPE type;

		// 種類を取得
		type = pObj->GetType();

		if (type == TYPE_CUBE)
		{// 種類がキューブの場合

			// ダイナミックキャストする
			CCube *pCube = dynamic_cast<CCube*>(pObj);

			// IDを取得
			ID = pCube->GetID();

			// 配置フラグを取得
			bool bSet = pCube->GetCubeSet();

			if(!pCube->m_Info.bBom)
			{
				if (m_Info.nID != ID && bSet)
				{// 自分以外のキューブだったら、

					// 自分自身の取得
					float fWidth = GetWidth();		// 幅
					float fHeight = GetHeight();	// 高さ
					float fDepth = GetDepth();		// 奥行き

					// 相手の取得
					D3DXVECTOR3 PairPos = pCube->GetPos();	// 位置
					float fPairWidth = pCube->GetWidth();	// 幅
					float fPairHeight = pCube->GetHeight();	// 高さ
					float fPairDepth = pCube->GetDepth();	// 奥行き

					//　中心点からの距離
					float fCubeWidth = fPairWidth * SIZE_DIAMETER;		// 幅
					float fCubeHeight = fPairHeight * SIZE_DIAMETER;	// 高さ
					float fCubeDepth = fPairDepth * SIZE_DIAMETER;		// 奥行き

					// サイズ調整
					fPairWidth *= COLLSION_DIAMETER;	// 幅
					fPairHeight *= COLLSION_DIAMETER;	// 高さ
					fPairDepth *= COLLSION_DIAMETER;	// 奥行き

					// 各方向の当たり判定
					D3DXVECTOR3 PairUpPos = D3DXVECTOR3(PairPos.x, PairPos.y + fCubeHeight, PairPos.z);		// 上
					D3DXVECTOR3 PairDownPos = D3DXVECTOR3(PairPos.x, PairPos.y - fCubeHeight, PairPos.z);	// 下
					D3DXVECTOR3 PairLeftPos = D3DXVECTOR3(PairPos.x + fCubeWidth, PairPos.y, PairPos.z);	// 左
					D3DXVECTOR3 PairRightPos = D3DXVECTOR3(PairPos.x - fCubeWidth, PairPos.y, PairPos.z);	// 右
					D3DXVECTOR3 PairBackPos = D3DXVECTOR3(PairPos.x, PairPos.y, PairPos.z + fCubeDepth);	// 奥
					D3DXVECTOR3 PairFrontPos = D3DXVECTOR3(PairPos.x, PairPos.y, PairPos.z - fCubeDepth);	// 手前

					switch (vector)
					{
					case VECTOR_X: {	/* X方向 */

						if (Collsion(pos, PairLeftPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
						{// 左側

							bHit = true;
							break;
						}

						if (Collsion(pos, PairRightPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
						{// 右側

							bHit = true;
							break;
						}
					}
					   break;
					case VECTOR_Y: {	/* Y方向 */

						if (Collsion(pos, PairUpPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
						{// 上側

							bHit = true;
							break;
						}

						if (Collsion(pos, PairDownPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
						{// 下側

							bHit = true;
							break;
						}
					}
					   break;
					case VECTOR_Z: {	/* Z方向 */

						if (Collsion(pos, PairBackPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
						{// 奥側

							bHit = true;
							break;
						}

						if (Collsion(pos, PairFrontPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
						{// 手前側

							bHit = true;
							break;
						}
					}
					   break;
					}

					// 判定が真なら TRUE を返す
					if (bHit)
					{
						// 連爆だと
						if (mode == 0)
						{
							pCube->Destruction(pCube);
						}
						return TRUE;
					}
				}
			}
		}

		pObj = pObjNext;	// 次のオブジェクトを代入
	}
	return FALSE;
}

//========================================
// オブジェクトの当たり判定
//========================================
void CCube::ModelCollsion(PRIO nPrio, TYPE nType, VECTOR vector,D3DXVECTOR3 pos)
{
	// 先頭オブジェクトを取得
	CObject *pObj = CObject::GetTop(nPrio);

	while (pObj != NULL)
	{// 使用されている時、

		// 次のオブジェクト
		CObject *pObjNext = pObj->GetNext();

		TYPE type;

		// 種類を取得
		type = pObj->GetType();

		if (type == nType)
		{// 選択した種類の時、

			// キューブの取得
			float fWidth = GetWidth();			// 幅
			float fHeight = GetHeight();		// 高さ
			float fDepth = GetDepth();			// 奥行き

			// サイズ調整
			if (m_Info.bSet)
			{
				if (vector != VECTOR_Y)
				{
					fHeight *= 0.05f;
				}

				switch (vector)
				{
				case CPhysics::VECTOR_X:
					fWidth *= BOM_COLLSION;
					break;
				case CPhysics::VECTOR_Y:
					fHeight *= BOM_COLLSION;
					break;
				case CPhysics::VECTOR_Z:
					fDepth *= BOM_COLLSION;
					break;
				case CPhysics::VECTOR_NONE:
					fWidth *= BOM_COLLSION;
					fDepth *= BOM_COLLSION;
					break;
				default:
					fWidth *= BOM_COLLSION;
					fDepth *= BOM_COLLSION;
					break;
				}
			}

			// 相手の取得
			D3DXVECTOR3 PairPos = pObj->GetPos();		// 位置
			D3DXVECTOR3 PairPosOld = pObj->GetPosOld();	// 位置(過去)
			float fPairWidth = pObj->GetWidth();		// 幅
			float fPairHeight = pObj->GetHeight();		// 高さ
			float fPairDepth = pObj->GetDepth();		// 奥行き

			if (nType == TYPE_ENEMY)
			{// 選択した種類の時、
				fPairHeight = 24.0f;
			}

			// 当たり判定
			if (Collsion(pos, PairPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
			{// 当たったら

				switch (nType)
				{
				case TYPE_ENEMY:
				{
					// ダイナミックキャストする
					CEnemy *pEnemy = dynamic_cast<CEnemy*>(pObj);

					// HIT処理
					pEnemy->HitLife(BOM_DAMAGE + (m_Info.nChain / DAMAGE_DIAMETER));
				}
					break;
				case TYPE_PLAYER:
				{
					m_Info.bErase = true;
				}
				break;

				case TYPE_BLOCK:
				{
					// ダイナミックキャストする
					CBlock *pBlock = dynamic_cast<CBlock*>(pObj);

					if (m_Info.bSet)
					{
						CBlock::STATE state = pBlock->GetBlockState();
						int nBlockType = pBlock->GetBlockType();

						if (state == CBlock::STATE_BREAK && nBlockType != MODEL_IRON_BOX)
						{
							// Hit処理
							pBlock->HitBlock();
						}
					}
					else if (!m_Info.bSet)
					{
						CBlock::STATE aState = pBlock->GetBlockState();
						if (aState == CBlock::STATE_ALLIE)
						{// 状態が貫通なら
							m_Info.bErase = true;
						}
					}
				}
				break;
				}
				
			}
		}

		pObj = pObjNext;	// 次のオブジェクトを代入
	}
}

//========================================
// 破壊処理
//========================================
void CCube::Destruction(CCube *pCube)
{
	// オブジェクト破棄
	pCube->m_Info.nLife = 10 + m_nNumChain;
	pCube->m_Info.bBom = true;

	m_nNumChain++;	// 連鎖カウントを加算

	// 連鎖カウントを代入
	pCube->m_Info.nChain = m_nNumChain;

	// 周囲にキューブがあるか
	pCube->Contact(0, VECTOR_X, pCube->m_Info.pos);
	pCube->Contact(0, VECTOR_Y, pCube->m_Info.pos);
	pCube->Contact(0, VECTOR_Z, pCube->m_Info.pos);
}

//========================================
// 使用数の設定
//========================================
void CCube::SetLimit(int nLimit)
{
	m_nRestCube = nLimit;
	m_nUseCube = 0;
	m_nLimitCube = nLimit;

	// 制限数を文字列に設定
	char aString[TXT_MAX];
	sprintf(aString, "CUBE ：%02d", m_nRestCube);

	FormFont pFont = {
		INIT_D3DXCOLOR,
		11.0f,
		1,
		10,
		-1
	};

	FormShadow pShadow = {
		D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),
		true,
		D3DXVECTOR3(2.0f,2.0f,0.0f),
		D3DXVECTOR2(1.0f,1.0f)
	};

	m_Cube = CText::Create(CText::BOX_NORMAL_RECT,
		D3DXVECTOR3(SCREEN_WIDTH - 290.0f, 42.0f, 0.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		aString,
		CFont::FONT_BESTTEN,
		&pFont, false, &pShadow);
}

//========================================
// リセット
//========================================
void CCube::Reset(void)
{
	m_nNumAll = -1;
	m_nRestCube = 0;
	m_nUseCube = 0;
	m_nNumChain = 1;
	m_nLimitCube = 0;
	bLeadSet = false;

	if (m_Cube != NULL)
	{
		m_Cube = NULL;
	}
}

//========================================
// テキスト
//========================================
void CCube::CubeText(void)
{
	int nNumSet = 0;
	D3DXCOLOR col;
	int nRest = m_nLimitCube - m_nUseCube;
	char aString[TXT_MAX];

	if (nRest <= 0)
	{
		nRest = 0;
	}
	sprintf(aString, "%02d", nRest);

	// 長さを取得
	int m_Digit = strlen(aString);

	if (nRest <= 0)
	{
		col = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);
	}
	else if (nRest <= m_nLimitCube / 3)
	{
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (nRest <= m_nLimitCube * 2/3)
	{
		col = D3DXCOLOR(0.8f,0.8f,0.0f,1.0f);
	}
	else if (nRest <= m_nLimitCube)
	{
		col = INIT_D3DXCOLOR;
	}


	for (int nTime = 0; nTime < m_Digit; nTime++)
	{
		m_Cube->ChgWords(&aString[nTime], CUBE_START_DEX + nTime, col);
	}
}