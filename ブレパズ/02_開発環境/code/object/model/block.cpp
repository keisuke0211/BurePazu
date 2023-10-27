//========================================
// 
// ブロック処理
// 
//========================================
// *** block.cpp ***
//========================================
#include "block.h"
#include "bullet_cube.h"
#include "enemy.h"
#include "../../scene/game.h"
#include "../../scene/title.h"
#include "../../scene/tutorial.h"
#include "model.h"
#include "../EFFECT/particleX.h"
#include "../../system/sound.h"
#include "../../system/csv_file.h"
#include "../../system/camera.h"
#include "../../system/words/font.h"
#include "../../system/words/text.h"

// 静的変数
CBlock::TypeInfo *CBlock::m_TypeInfo = NULL;
D3DXVECTOR3 CBlock::m_CameraRot = INIT_D3DXVECTOR3;
float CBlock::m_CameraHeigth = 0.0f;
int CBlock::m_nNumAll = 0;

//========================================
// マクロ定義
//========================================
#define GRAVITY_MAG		(0.08f)		// 重力係数 0.08
#define GRAVITY_POWER	(1.5f)		// 重力加速度
#define TNT_COLLSION	(3.5f)		// TNTの爆発の判定
#define FILE_PATH	("data/GAMEDATA/BLOCK/BLOCK_DATA.txt")	// ファイルパス

//========================================
// コンストラクタ
//========================================
CBlock::CBlock(int nPriority) : CObjectX(nPriority)
{
	// 値をクリア
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.posOld = INIT_D3DXVECTOR3;
	m_Info.move = INIT_D3DXVECTOR3;
	m_Info.rot = INIT_D3DXVECTOR3;
	m_Info.rotOld = INIT_D3DXVECTOR3;
	m_Info.size = INIT_D3DXVECTOR3;
	m_Info.col = INIT_D3DXCOLOR;
	m_Info.nModelID = 0;
	m_Info.state = STATE_NORMAL;
	m_Info.fRadius = 0.0f;
	m_Info.nCntRadius = 0;
	m_Info.fRadiusRate = 0.0f;
	m_Info.bSet = false;
	m_Info.nEraseTime = 0;
	m_Info.bErase = false;
	m_Info.nID = m_nNumAll;
	m_nNumAll++;
}

//========================================
// デストラクタ
//========================================
CBlock::~CBlock()
{

}

//========================================
// 生成
//========================================
CBlock *CBlock::Create(int nType, D3DXVECTOR3 pos, int nState)
{
	CBlock *pBlock = NULL;

	if (pBlock != NULL)
	{
		return pBlock;
	}

	// オブジェクト2Dの生成
	pBlock = new CBlock;

	pBlock->m_Info.state = STATE_MAX;
	pBlock->SetModel(nType);
	pBlock->m_Info.nModelID = nType;

	switch (nState)
	{
	case STATE_NORMAL:
	{
		pBlock->m_Info.state = STATE_NORMAL;
	}
	break;
	case STATE_BREAK:
	{
		pBlock->m_Info.state = STATE_BREAK;
	}
	break;
	case STATE_ALLIE:
	{
		pBlock->m_Info.state = STATE_ALLIE;
	}
	break;
	}

	// 初期化処理
	pBlock->Init();

	pBlock->SetBlockPos(pos);

	return pBlock;
}

//========================================
// 初期化
//========================================
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	// 種類の設定
	SetType(TYPE_BLOCK);

	m_Info.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.size = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_Info.fRadius = 1.0f;

	if(m_Info.state == STATE_MAX)
	{
	switch (m_TypeInfo[m_Info.nModelID].nState)
	{
	case STATE_NORMAL:
		m_Info.state = STATE_NORMAL;
		break;
	case STATE_BREAK:
		m_Info.state = STATE_BREAK;
		break;
	case STATE_ALLIE:
		m_Info.state = STATE_ALLIE;
		break;
	}
	}

	m_Info.col = INIT_D3DXCOLOR;

	// 生成
	SetPos(m_Info.pos);
	SetScale(m_Info.size);
	SetColor(m_Info.col);

	return S_OK;
}

//========================================
// 終了
//========================================
void CBlock::Uninit(void)
{
	CObjectX::Uninit();
}

//========================================
// 更新
//========================================
void CBlock::Update(void)
{
	// -- 取得 -------------------------------------------
	CCamera *pCamera = CManager::GetCamera();		// カメラ

	// 過去の位置・向きの更新
	m_Info.posOld = m_Info.pos;
	m_Info.rotOld = m_Info.rot;

	if (m_Info.nID == 154)
	{
		m_Info.nID = 154;
	}

	// 移動量の代入
	m_Info.pos.y += m_Info.move.y;

	if (m_Info.state != STATE_NORMAL)
	{
		if (m_Info.pos.y > -40.0f)
		{
			//Ｙの移動量に重力を加算
			m_Info.move.y -= (GRAVITY_POWER - m_Info.move.y) * GRAVITY_MAG;
			m_Info.pos = Collision(PRIO_BLOCK, TYPE_BLOCK, VECTOR_Y, m_Info.pos);
		}
		else if (m_Info.pos.y <= -40.0f)
		{
			m_Info.move.y = 0.0f;
			m_Info.pos.y = -40.0f;
		}
	}
	
	SetPos(m_Info.pos);
	SetScale(m_Info.size);

	if (m_Info.bErase)
	{// フラグが真の時、

		if (--m_Info.nEraseTime <= 0)
		{
			switch (m_Info.nModelID)
			{
			case MODEL_BOMB:
			{
				Bomb();
			}
				break;
			case MODEL_WOOD_BOX:
			{
				WoodenBox();
			}
				break;
			case MODEL_IRON_BOX:
			{
				WoodenBox();
			}
			break;
			}

			// 破棄
			Uninit();

			return;
		}
	}

	CObjectX::Update();
}

//========================================
// 描画
//========================================
void CBlock::Draw(void)
{
	CObjectX::Draw();
}

//========================================
// Hit処理
//========================================
void CBlock::HitBlock(void)
{
	if (m_Info.state == STATE_BREAK)
	{
		switch (m_Info.nModelID)
		{
		case MODEL_WOOD_BOX:
		{
			m_Info.nEraseTime = 10;
			m_Info.bErase = true;
		}
		break;
		case MODEL_BOMB:
		{
			m_Info.nEraseTime = 10;
			m_Info.bErase = true;
		}
		break;
		case MODEL_IRON_BOX:
		{
			m_Info.nEraseTime = 10;
			m_Info.bErase = true;
		}
		break;
		}
	}
}

//========================================
// 木箱
//========================================
void CBlock::WoodenBox(void)
{
	bool bGravity = CEnemy::IsmGravity();

	if (!bGravity)
	{
		CEnemy::SetGravity(true);
	}

	// パーティクル生成
	CParticleX *pObj = CParticleX::Create();
	pObj->Par_SetPos(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, m_Info.pos.z));
	pObj->Par_SetRot(INIT_D3DXVECTOR3);
	pObj->Par_SetMove(D3DXVECTOR3(35.0f, 15.0f, 35.0f));
	pObj->Par_SetType(0);
	pObj->Par_SetLife(100);
	pObj->Par_SetCol(D3DXCOLOR(0.6f, 0.337f, 0.086f, 1.0f));
	pObj->Par_SetForm(20);
}

//========================================
// 爆弾
//========================================
void CBlock::Bomb(void)
{
	// キューブとの当たり判定
	BombCollsion(PRIO_BLOCK, TYPE_BLOCK, VECTOR_NONE, m_Info.pos);
	BombCollsion(PRIO_BLOCK, TYPE_BLOCK, VECTOR_X, m_Info.pos);
	BombCollsion(PRIO_BLOCK, TYPE_BLOCK, VECTOR_Y, m_Info.pos);
	BombCollsion(PRIO_BLOCK, TYPE_BLOCK, VECTOR_Z, m_Info.pos);

	BombCollsion(PRIO_OBJECT, TYPE_ENEMY, VECTOR_NONE, m_Info.pos);
	BombCollsion(PRIO_OBJECT, TYPE_ENEMY, VECTOR_X,m_Info.pos);
	BombCollsion(PRIO_OBJECT, TYPE_ENEMY, VECTOR_Y, m_Info.pos);
	BombCollsion(PRIO_OBJECT, TYPE_ENEMY, VECTOR_Z, m_Info.pos);

	// パーティクル生成
	CParticleX *pObj = CParticleX::Create();
	pObj->Par_SetPos(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, m_Info.pos.z));
	pObj->Par_SetRot(INIT_D3DXVECTOR3);
	pObj->Par_SetMove(D3DXVECTOR3(25.0f, 15.0f, 25.0f));
	pObj->Par_SetType(0);
	pObj->Par_SetLife(100);
	pObj->Par_SetCol(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	pObj->Par_SetForm(15);

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::TYPE_HIT);
}

//========================================
// 鉄箱
//========================================
void CBlock::IronBox(void)
{
	bool bGravity = CEnemy::IsmGravity();

	if (!bGravity)
	{
		CEnemy::SetGravity(true);
	}

	// パーティクル生成
	CParticleX *pObj = CParticleX::Create();
	pObj->Par_SetPos(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, m_Info.pos.z));
	pObj->Par_SetRot(INIT_D3DXVECTOR3);
	pObj->Par_SetMove(D3DXVECTOR3(35.0f, 15.0f, 35.0f));
	pObj->Par_SetType(0);
	pObj->Par_SetLife(100);
	pObj->Par_SetCol(D3DXCOLOR(0.6f, 0.337f, 0.086f, 1.0f));
	pObj->Par_SetForm(20);
}

//========================================
// 爆発の当たり判定
//========================================
void CBlock::BombCollsion(PRIO nPrio, TYPE nType, VECTOR vector,D3DXVECTOR3 pos)
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

			 // ブロックの取得
			int nBlockType = GetBlockType();	// 種類
			float fWidth = GetWidth();			// 幅
			float fHeight = GetHeight();		// 高さ
			float fDepth = GetDepth();			// 奥行き

			if (nBlockType == MODEL_BOMB)
			{
				if (vector != VECTOR_NONE)
				{
					fHeight *= 1.5f;
				}

				switch (vector)
				{
				case CPhysics::VECTOR_X:
					fWidth *= TNT_COLLSION;
					break;
				case CPhysics::VECTOR_Y:
					fHeight *= TNT_COLLSION;
					break;
				case CPhysics::VECTOR_Z:
					fDepth *= TNT_COLLSION;
					break;
				case CPhysics::VECTOR_NONE:
					fWidth *= (TNT_COLLSION - 1.5f);
					fHeight *= 0.5f;
					fDepth *= (TNT_COLLSION - 1.5f);
					break;
				default:
					fWidth *= TNT_COLLSION;
					fDepth *= TNT_COLLSION;
					break;
				}
			}

			// 相手の取得
			D3DXVECTOR3 PairPos = pObj->GetPos();		// 位置
			D3DXVECTOR3 PairPosOld = pObj->GetPosOld();	// 位置(過去)
			float fPairWidth = pObj->GetWidth();		// 幅
			float fPairHeight = pObj->GetHeight();		// 高さ
			float fPairDepth = pObj->GetDepth();		// 奥行き

			// 当たり判定
			if (Collsion(pos, PairPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
			{// 当たったら

				switch (nType)
				{
				case TYPE_BLOCK:
				{
					// ダイナミックキャストする
					CBlock *pBlock = dynamic_cast<CBlock*>(pObj);

					// IDを取得
					int nID = pBlock->GetID();

					CBlock::STATE state = pBlock->GetBlockState();

					if (state == STATE_BREAK)
					{
						if (m_Info.nID != nID)
						{
							// Hit処理
							pBlock->HitBlock();
						}
					}
				}
				break;
				case TYPE_CUBE:
				{
					// ダイナミックキャストする
					CCube *pCube = dynamic_cast<CCube*>(pObj);

					// HIT処理
					pCube->SetCubeLife(20);
				}
				break;
				case TYPE_ENEMY:
				{
					// ダイナミックキャストする
					CEnemy *pEnemy = dynamic_cast<CEnemy*>(pObj);

					// HIT処理
					pEnemy->HitLife(1);
				}
				break;
				}
			}
		}

		pObj = pObjNext;	// 次のオブジェクトを代入
	}
}

//========================================
// オブジェクトの当たり判定
//========================================
D3DXVECTOR3 CBlock::Collision(PRIO nPrio, TYPE nType, VECTOR vector, D3DXVECTOR3 pos)
{
	// 先頭オブジェクトを取得
	CObject *pObj = CObject::GetTop(nPrio);

	while (pObj != NULL)
	{// 使用されている時、

		// 次のオブジェクト
		CObject *pObjNext = pObj->GetNext();

		// 種類を取得
		TYPE type = pObj->GetType();

		// ダイナミックキャストする
		CBlock *pBlock = dynamic_cast<CBlock*>(pObj);
		int nID = pBlock->GetID();

		if (m_Info.nID != nID)
		{
			// ブロックの取得
			D3DXVECTOR3 PosOld = GetPosOld();	// 位置(過去)
			D3DXVECTOR3 RotOld = GetRotOld();	// 向き(過去)
			D3DXVECTOR3 Scale = GetSize();
			float fWidth = GetWidth();		// 幅
			float fHeight = GetHeight();	// 高さ
			float fDepth = GetDepth();		// 奥行き

			if (m_Info.nModelID == MODEL_BOMB)
			{
				fHeight = 20.0f;
			}

			fWidth = round(fWidth);
			fHeight = round(fHeight);
			fDepth = round(fDepth);

			if (type == nType)
			{// 種類がブロックの時、

				// ブロックの取得
				D3DXVECTOR3 PairPos = pObj->GetPos();		// 位置
				D3DXVECTOR3 PairPosOld = pObj->GetPosOld();	// 位置(過去)
				D3DXVECTOR3 PairRot = pObj->GetRot();		// 向き
				D3DXVECTOR3 PairRotOld = pObj->GetRotOld();	// 向き(過去)

				float fPairWidth = pObj->GetWidth();	// 幅
				float fPairHeight = pObj->GetHeight();	// 高さ
				float fPairDepth = pObj->GetDepth();	// 奥行き

				int nModelId = pBlock->GetBlockType();
				if (nModelId == MODEL_BOMB)
				{
					fPairHeight = 20.0f;
				}

				fPairWidth = round(fPairWidth);
				fPairHeight = round(fPairHeight);
				fPairDepth = round(fPairDepth);

				// --- 当たり判定 ----------------------------------------------
				switch (vector)
				{
				case VECTOR_X: {	/* X方向 */

					if (CollsionX(pos, PairPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 奥辺と手前辺が相手の幅の内側の時、

						if (CollsionDirection(pos, PairPos, PosOld, PairPosOld, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth), DIRECTION_LEFT))
						{// 左からめり込んでいる時
							pos.x = (PairPos.x - fPairWidth) - fWidth;
						}
						else if (CollsionDirection(pos, PairPos, PosOld, PairPosOld, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth), DIRECTION_RIGHT))
						{// 右からめり込んでいる時
							pos.x = (PairPos.x + fPairWidth) + fWidth;
						}
					}
				}
							   break;
				case VECTOR_Y: {	/* Y方向 */

					if (CollsionY(pos, PairPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 左辺と右辺が相手の幅の内側の時、

						if (CollsionDirection(pos, PairPos, PosOld, PairPosOld, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth), DIRECTION_DOWN))
						{// 下からめり込んでいる時

						}
						else if (CollsionDirection(pos, PairPos, PosOld, PairPosOld, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth), DIRECTION_UP))
						{// 上からめり込んでいる時

							m_Info.move.y = 0.0f;

							pos.y = (PairPos.y + fPairHeight) + fHeight;
						}
					}
				}
							   break;
				case VECTOR_Z: {	/* Z方向 */

					if (CollsionZ(pos, PairPos, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth)))
					{// 奥辺と手前辺が相手の幅の内側の時、

						if (CollsionDirection(pos, PairPos, PosOld, PairPosOld, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth), DIRECTION_BACK))
						{// 後ろからめり込んでいる時
							pos.z = (PairPos.z - fPairDepth) - fDepth;
						}
						else if (CollsionDirection(pos, PairPos, PosOld, PairPosOld, D3DXVECTOR3(fWidth, fHeight, fDepth), D3DXVECTOR3(fPairWidth, fPairHeight, fPairDepth), DIRECTION_FRONT))
						{// 前からめり込んでいる時
							pos.z = (PairPos.z + fPairDepth) + fDepth;
						}
					}
				}
							   break;
				}
			}
		}
		pObj = pObjNext;	// 次のオブジェクトを代入
	}
	return pos;
}

//========================================
// 読み込み
//========================================
void CBlock::Load(void)
{
	int nCntModel = 0;
	char aDataSearch[TXT_MAX];	// データ検索用
	m_TypeInfo = NULL;

	// ファイルの読み込み
	FILE *pFile = fopen(FILE_PATH, "r");

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

		if (!strcmp(aDataSearch, "NUM_BLOCK"))
		{
			int nMaxBlock = 0;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nMaxBlock);			// 最大数
			m_TypeInfo = new CBlock::TypeInfo[nMaxBlock];
			assert(m_TypeInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "BLOCK"))
		{
			fscanf(pFile, "%s", &aDataSearch[0]);
			m_TypeInfo[nCntModel].nModelID = nCntModel;			// モデルID
			fscanf(pFile, "%d", &m_TypeInfo[nCntModel].nState);	// 状態
			nCntModel++;
		}
	}
}

//========================================
// リセット
//========================================
void CBlock::Reset(void)
{
	if (m_TypeInfo != NULL)
	{
		delete[] m_TypeInfo;
		m_TypeInfo = NULL;
	}

	m_CameraRot = INIT_D3DXVECTOR3;
	m_CameraHeigth = 0.0f;
	m_nNumAll = 0;
}