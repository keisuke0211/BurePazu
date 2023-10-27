//========================================
// 
// バレット処理
// 
//========================================
// *** bullet.cpp ***
//========================================
#include "bullet.h"
#include "bullet_cube.h"
#include "enemy.h"
#include "block.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../system/input.h"
#include "../UI/score.h"
#include "../effect/effectX.h"
#include "../../scene/game.h"
#include "../../scene/pause.h"

// 静的変数
int CBullet::m_nNumAll = 0;

//========================================
// マクロ定義
//========================================
#define BULLET_SPEED		(7.5f)	// 弾の速度
#define BULLET_DAMAGE		(1)		// ダメージ量
#define COLLSION_DIAMETER	(1.00f)	// 当たり判定の倍率
#define BULLET_DIAMETER		(0.2f)	// サイズの倍率
#define BULLET_LIFE			(60)	// 寿命

#define CUBE_LIFE1			(60)	// 壁や床に配置した場合の寿命
#define CUBE_LIFE2			(60)	// 敵に当てた場合の寿命

//========================================
// コンストラクタ
//========================================
CBullet::CBullet(int nPriority) : CObjectX(nPriority)
{
	// 値をクリア
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.posOld = INIT_D3DXVECTOR3;
	m_Info.rot = INIT_D3DXVECTOR3;
	m_Info.rotOld = INIT_D3DXVECTOR3;
	m_Info.move = INIT_D3DXVECTOR3;
	m_Info.size = INIT_D3DXVECTOR3;
	m_Info.nType = 0;
	m_Info.nLife = 0;
	m_Info.fHeight = 0.0f;
	m_Info.fWidth = 0.0f;
	m_Info.bHit = false;
	m_Info.nID = m_nNumAll;

	m_nNumAll++;	// 総数加算
}
//========================================
// デストラクタ
//========================================
CBullet::~CBullet()
{
	m_nNumAll--;	// 総数減算
}

//========================================
// 生成
//========================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBullet *pBullet = NULL;

	if (pBullet != NULL)
	{
		return pBullet;
	}

	// バレットの生成
	pBullet = new CBullet;

	pBullet->m_Info.pos = pos;
	pBullet->m_Info.rot = rot;
	pBullet->m_Info.nLife = BULLET_LIFE;
	pBullet->m_Info.size = D3DXVECTOR3(BULLET_DIAMETER, BULLET_DIAMETER, BULLET_DIAMETER);
	pBullet->SetModel(MODEL_BULLET);

	// 初期化処理
	pBullet->Init();

	return pBullet;
}
//========================================
// 初期化
//========================================
HRESULT CBullet::Init(void)
{
	CObjectX::Init();

	// 種類の設定
	SetType(TYPE_BULLET);

	m_Info.move.x = sinf(m_Info.rot.y) * BULLET_SPEED;
	m_Info.move.z = cosf(m_Info.rot.y) * BULLET_SPEED;
	m_Info.col = D3DXCOLOR(0.1f, 0.9f, 0.9f, 1.0f);

	SetPos(m_Info.pos);
	SetScale(m_Info.size);
	SetColor(m_Info.col);
	
	return S_OK;
}

//========================================
// 終了
//========================================
void CBullet::Uninit(void)
{
	CObjectX::Uninit();
}

//========================================
// 更新
//========================================
void CBullet::Update(void)
{
	// --- 取得 ---------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CMouse *pInputMouse = CManager::GetInputMouse();			// マウス
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット

	bool bPause = CPause::IsPause();
	bool bEnd = CGame::IsEnd();

	if (!bPause && !bEnd)
	{
		// 過去の位置・向きの更新
		m_Info.posOld = m_Info.pos;
		m_Info.rotOld = m_Info.rot;

		// 寿命の減衰
		if (--m_Info.nLife <= 0)
		{// 寿命が尽きた時

			// 弾の破棄
			Uninit();

			return;
		}

		{
			// 移動量を代入
			m_Info.pos.x += m_Info.move.x;

			// X方向の当たり判定
			if (Collsion(VECTOR_X, m_Info.pos))
			{
				return;
			}
		}

		{
			// 移動量を代入
			m_Info.pos.z += m_Info.move.z;

			// Z方向の当たり判定
			if (Collsion(VECTOR_Z, m_Info.pos))
			{
				return;
			}
		}

		{
			// 移動量を代入
			m_Info.pos.y += m_Info.move.y;

			// Y方向の当たり判定
			if (Collsion(VECTOR_Y, m_Info.pos))
			{
				return;
			}
		}

		SetPos(m_Info.pos);
		SetScale(m_Info.size);

		CObjectX::Update();

		CEffectX *pObj = CEffectX::Create();
		pObj->Eff_SetPos(GetPosOld());
		pObj->Eff_SetRot(GetRot());
		pObj->Eff_SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pObj->Eff_SetType(0);
		pObj->Eff_SetLife(10);
		pObj->Eff_SetCol(D3DXCOLOR(0.1f, 0.8f, 0.8f, 1.0f));
		pObj->Eff_SetRadius(0.35f);
	}
}

//========================================
// 描画
//========================================
void CBullet::Draw(void)
{
	CObjectX::Draw();
}

//========================================
// 当たり判定
//========================================
bool CBullet::Collsion(VECTOR vector, D3DXVECTOR3 pos)
{
	for (int nCntPrio = 0; nCntPrio < PRIO_MAX; nCntPrio++)
	{
		// 先頭オブジェクトを取得
		CObject *pObj = CObject::GetTop(nCntPrio);

		while (pObj != NULL)
		{// 使用されている時、

			// 次のオブジェクト
			CObject *pObjNext = pObj->GetNext();

			TYPE type;

			// 種類を取得
			type = pObj->GetType();

			if (type == TYPE_BLOCK || type == TYPE_CUBE)
			{// 種類がブロックかキューブの場合

				// バレットの取得
				D3DXVECTOR3 PosOld = GetPosOld();	// 位置(過去)
				float fWidth = GetWidth();			// 幅
				float fHeight = GetHeight();		// 高さ
				float fDepth = GetDepth();			// 奥行き

				// サイズ調整
				fWidth *= BULLET_DIAMETER;	// 幅
				fHeight *= BULLET_DIAMETER;	// 高さ
				fDepth *= BULLET_DIAMETER;	// 奥行き


				// ブロックの取得
				D3DXVECTOR3 PairPos = pObj->GetPos();	// 位置
				float fPairWidth = pObj->GetWidth();	// 幅
				float fPairHeight = pObj->GetHeight();	// 高さ
				float fPairDepth = pObj->GetDepth();	// 奥行き

				// 判定の修正
				fPairWidth *= COLLSION_DIAMETER;	// 幅
				fPairHeight *= COLLSION_DIAMETER;	// 高さ
				fPairDepth *= COLLSION_DIAMETER;	// 奥行き

				switch (vector)
				{
				case VECTOR_X: {	/* X方向 */

					if ((pos.z + fDepth) > (PairPos.z - fPairDepth) &&
						(pos.z - fDepth) < (PairPos.z + fPairDepth) &&
						(pos.y + fHeight) > (PairPos.y - fPairHeight) &&
						(pos.y - fHeight) < (PairPos.y + fPairHeight))
					{// 奥辺と手前辺がブロックの幅の内側の時、

						if ((pos.x + fWidth) >= (PairPos.x - fPairWidth) &&
							(PosOld.x + fWidth) <= (PairPos.x - fPairWidth))
						{// 左からめり込んでいる時

							// ダイナミックキャストする
							if (type == TYPE_BLOCK)
							{
								CBlock *pBlock = dynamic_cast<CBlock*>(pObj);
								CBlock::STATE aState = pBlock->GetBlockState();
								if (aState == CBlock::STATE_ALLIE)
								{// 状態が貫通なら
									return FALSE;
								}
							}
							m_Info.bHit = true;
							pos.x = (PairPos.x - (fPairWidth / COLLSION_DIAMETER)) - (fWidth / BULLET_DIAMETER);
						}
						else if ((pos.x - fWidth) <= (PairPos.x + fPairWidth) &&
							(PosOld.x - fWidth) >= (PairPos.x + fPairWidth))
						{// 右からめり込んでいる時

							// ダイナミックキャストする
							if (type == TYPE_BLOCK)
							{
								CBlock *pBlock = dynamic_cast<CBlock*>(pObj);
								CBlock::STATE aState = pBlock->GetBlockState();
								if (aState == CBlock::STATE_ALLIE)
								{// 状態が貫通なら
									return FALSE;
								}
							}
							m_Info.bHit = true;
							pos.x = (PairPos.x + (fPairWidth / COLLSION_DIAMETER)) + (fWidth / BULLET_DIAMETER);
						}
					}
				}
							   break;
				case VECTOR_Y: {	/* Y方向 */

					if ((pos.x + fWidth) > (PairPos.x - fPairWidth) &&
						(pos.x - fWidth) < (PairPos.x + fPairWidth) &&
						(pos.z + fDepth) > (PairPos.z - fPairDepth) &&
						(pos.z - fDepth) < (PairPos.z + fPairDepth))
					{// 左辺と右辺が相手の幅の内側の時、

						if ((pos.y + fHeight) > (PairPos.y - fPairHeight) &&
							(PosOld.y + fHeight) <= (PairPos.y - fPairHeight))
						{// 下からめり込んでいる時

							m_Info.bHit = true;
							pos.y = (PairPos.y - (fPairHeight / COLLSION_DIAMETER)) - (fHeight / BULLET_DIAMETER);
						}
						else if ((pos.y - fHeight) < (PairPos.y + fPairHeight) &&
							(PosOld.y - fHeight) >= (PairPos.y + fPairHeight))
						{// 上からめり込んでいる時

							m_Info.bHit = true;
							pos.y = (PairPos.y + (fPairHeight / COLLSION_DIAMETER)) + (fHeight / BULLET_DIAMETER);
						}
					}
				}
							   break;
				case VECTOR_Z: {	/* Z方向 */

					if ((pos.x + fWidth) > (PairPos.x - fPairWidth) &&
						(pos.x - fWidth) < (PairPos.x + fPairWidth) &&
						(pos.y + fHeight) > (PairPos.y - fPairHeight) &&
						(pos.y - fHeight) < (PairPos.y + fPairHeight))
					{// 左辺と右辺が相手の幅の内側の時、

						if ((pos.z + fDepth) >= (PairPos.z - fPairDepth) &&
							(PosOld.z + fDepth) <= (PairPos.z - fPairDepth))
						{// 前からめり込んでいる時

							// ダイナミックキャストする
							if (type == TYPE_BLOCK)
							{
								CBlock *pBlock = dynamic_cast<CBlock*>(pObj);
								CBlock::STATE aState = pBlock->GetBlockState();
								if (aState == CBlock::STATE_ALLIE)
								{// 状態が貫通なら
									return FALSE;
								}
							}
							m_Info.bHit = true;
							pos.z = (PairPos.z - (fPairDepth / COLLSION_DIAMETER)) - (fDepth / BULLET_DIAMETER);
						}
						else if ((pos.z - fDepth) <= (PairPos.z + fPairDepth) &&
							(PosOld.z - fDepth) >= (PairPos.z + fPairDepth))
						{// 奥からめり込んでいる時

							 // ダイナミックキャストする
							if (type == TYPE_BLOCK)
							{
								CBlock *pBlock = dynamic_cast<CBlock*>(pObj);
								CBlock::STATE aState = pBlock->GetBlockState();
								if (aState == CBlock::STATE_ALLIE)
								{// 状態が貫通なら
									return FALSE;
								}
							}
							m_Info.bHit = true;
							pos.z = (PairPos.z + (fPairDepth / COLLSION_DIAMETER)) + (fDepth / BULLET_DIAMETER);
						}
					}
				}
				   break;
				}

				if (m_Info.bHit)
				{
					// キューブの生成
					CCube::Create(m_Info.nType, D3DXVECTOR3(pos.x, pos.y, pos.z), CUBE_LIFE1);

					// 弾の破棄
					Uninit();

					return TRUE;
				}
			}
			else if (type == TYPE_ENEMY)
			{// 種類が敵だった時

				// バレットの取得
				D3DXVECTOR3 PosOld = GetPosOld();	// 位置(過去)
				float fWidth = GetWidth();			// 幅
				float fHeight = GetHeight();		// 高さ
				float fDepth = GetDepth();			// 奥行き


				// ブロックの取得
				D3DXVECTOR3 PairPos = pObj->GetPos();		// 位置
				D3DXVECTOR3 PairPosOld = pObj->GetPosOld();	// 位置(過去)
				float fPairWidth = pObj->GetWidth();		// 幅
				float fPairHeight = pObj->GetHeight();		// 高さ
				float fPairDepth = pObj->GetDepth();		// 奥行き

				fPairWidth = round(fWidth);
				fPairHeight = round(fHeight);
				fPairDepth = round(fDepth);

				switch (vector)
				{
				case VECTOR_X: {	/* X方向 */

					if ((pos.z + fDepth) > (PairPos.z - fPairDepth) &&
						(pos.z - fDepth) < (PairPos.z + fPairDepth) &&
						(pos.y + fHeight) > (PairPos.y - fPairHeight) &&
						(pos.y - fHeight) < (PairPos.y + fPairHeight))
					{// 奥辺と手前辺がブロックの幅の内側の時、

						if ((pos.x + fWidth) >= (PairPos.x - fPairWidth) &&
							(PosOld.x + fWidth) <= (PairPosOld.x - fPairWidth))
						{// 左からめり込んでいる時

							m_Info.bHit = true;
						}
						else if ((pos.x - fWidth) <= (PairPos.x + fPairWidth) &&
							(PosOld.x - fWidth) >= (PairPosOld.x + fPairWidth))
						{// 右からめり込んでいる時

							m_Info.bHit = true;
						}
					}
				}
							   break;
				case VECTOR_Y: {	/* Y方向 */

					if ((pos.x + fWidth) > (PairPos.x - fPairWidth) &&
						(pos.x - fWidth) < (PairPos.x + fPairWidth) &&
						(pos.z + fDepth) > (PairPos.z - fPairDepth) &&
						(pos.z - fDepth) < (PairPos.z + fPairDepth))
					{// 左辺と右辺が相手の幅の内側の時、

						if ((pos.y + fHeight) > (PairPos.y - fPairHeight) &&
							(PosOld.y + fHeight) <= (PairPosOld.y - fPairHeight))
						{// 下からめり込んでいる時

							m_Info.bHit = true;
						}
						else if ((pos.y - fHeight) < (PairPos.y + fPairHeight) &&
							(PosOld.y - fHeight) >= (PairPosOld.y + fPairHeight))
						{// 上からめり込んでいる時

							m_Info.bHit = true;
						}
					}
				}
				   break;
				case VECTOR_Z: {	/* Z方向 */

					if ((pos.x + fWidth) > (PairPos.x - fPairWidth) &&
						(pos.x - fWidth) < (PairPos.x + fPairWidth) &&
						(pos.y + fHeight) > (PairPos.y - fPairHeight) &&
						(pos.y - fHeight) < (PairPos.y + fPairHeight))
					{// 左辺と右辺が相手の幅の内側の時、

						if ((pos.z + fDepth) >= (PairPos.z - fPairDepth) &&
							(PosOld.z + fDepth) <= (PairPosOld.z - fPairDepth))
						{// 前からめり込んでいる時

							m_Info.bHit = true;
						}
						else if ((pos.z - fDepth) <= (PairPos.z + fPairDepth) &&
							(PosOld.z - fDepth) >= (PairPosOld.z + fPairDepth))
						{// 奥からめり込んでいる時

							m_Info.bHit = true;
						}
					}
				}
				   break;
				}

				if (m_Info.bHit)
				{
					// ダイナミックキャストする
					CEnemy *pEnemy = dynamic_cast<CEnemy*>(pObj);
					pEnemy->SetState(CEnemy::STATE_STOP);

					// キューブの生成
					CCube::Create(m_Info.nType, D3DXVECTOR3(pos.x, pos.y, pos.z), CUBE_LIFE2);

					// 弾の破棄
					Uninit();

					return TRUE;
				}
			}

			pObj = pObjNext;	// 次のオブジェクトを代入
		}
	}
	return FALSE;
}

//========================================
// リセット
//========================================
void CBullet::Reset(void)
{
	m_nNumAll = 0;
}