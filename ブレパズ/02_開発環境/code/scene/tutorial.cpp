//========================================
// 
// チュートリアル
// 
//========================================
// *** tutorial.h ***
//========================================
#include "tutorial.h"
#include "title.h"
#include "../manager.h"
#include "../system/csv_file.h"
#include "../object\UI\score.h"
#include "../object\UI\time.h"
#include "../object\model\block.h"
#include "../object/model/bullet.h"
#include "../object/model/bullet_cube.h"
#include "../object\model\player.h"
#include "../object\model\enemy.h"
#include "../object\BG\bg_side.h"
#include "../object\BG\bg_ceiling.h"
#include "../object/BG/mesh_floor.h"
#include "../system/input.h"
#include "../system/words/text.h"
#include "../system/words/font.h"
#include "../system/camera.h"
#include "../scene/pause.h"
#include "../system/sound.h"
#include "../object/model/model.h"
#include "fade.h"

// 静的変数
CTutorial::LoadTxt *CTutorial::m_LoadText = NULL;	// テキスト情報

// サウンドのデータファイルの相対パス
const char* CTutorial::FILE_PATH = "data\\GAMEDATA\\TEXT\\TUTORIAL_DATA.csv";

CPlayer *CTutorial::m_pPlayer = NULL;
CTime *CTutorial::m_pTime = NULL;
CScore *CTutorial::m_pScore = NULL;
int CTutorial::m_nCurAction = 0;

const char* CTutorial::SIDE_FILE = "data\\GAMEDATA\\OBJECT\\SIDE_DATA .txt";
const char* CTutorial::FLOOR_FILE = "data\\GAMEDATA\\OBJECT\\FLOOR_DATA.txt";
const char* CTutorial::BLOCK_FILE1 = "data\\GAMEDATA\\BLOCK\\TUTORIAL_DATA.csv";
const char* CTutorial::ENEMY_FILE1 = "data\\GAMEDATA\\ENEMY\\TUTORIAL_ENEMY.csv";

//========================================
// コンストラクタ
//========================================
CTutorial::CTutorial()
{
	for (int nTxt = 0; nTxt < 4; nTxt++)
	{
		m_Txt[nTxt] = NULL;
	}

	m_nStartTime = 0;

	Action = ACTION_MOVE;
	m_nEndTime = 0;
	m_bEnd = false;

	m_aCreateText.nCreateTime = 0;
	m_aCreateText.nCurAction = 0;
	m_aCreateText.nNumCur = 0;
	m_aCreateText.nNumCurAll = 0;

	for (int nCur = 0; nCur < ACTION_MAX; nCur++)
	{
		m_aCreateText.nCurMax[nCur] = 0;
		m_aCreateText.bCreate[nCur] = false;
	}
}

//========================================
// デストラクタ
//========================================
CTutorial::~CTutorial()
{

}

//========================================
// 初期化
//========================================
HRESULT CTutorial::Init(void)
{
	CEnemy::Reset();
	CBullet::Reset();
	CCube::Reset();

	CBlock::Load();			// ブロック
	CModel::InitModel();	// モデル
	CTitle::SetStart(false);
	CTitle::SetClear(false);

	// キューブの使用数
	CCube::SetLimit(99);

	// 背景(側面)の生成
	LoodSide();

	// 背景(天井)の生成
	LoodCeiling();

	// 床の生成
	LoodFloor();

	// ブロックの生成
	LoodBlock();

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 20.0f, 0.0f),0.0f);
	m_pPlayer->SetMotion("data\\GAMEDATA\\MODEL\\Player\\PLAYER_DATA.txt");


	CCamera *pCamera = CManager::GetCamera();					// カメラ
	pCamera->Init();
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pCamera->SetHeigth(0.4f);
	pCamera->SetDistance(500.0f);

	// 読み込み
	TextLoad();

	// テキスト生成
	TxtCreate(ACTION_MOVE);

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::TYPE_GAME01);

	return S_OK;
}

//========================================
// 終了
//========================================
void CTutorial::Uninit(void)
{
	CSound *pSound = CManager::GetSound();
	pSound->StopSound();

	CBlock::Reset();
	CModel::UninitModel();

	CObject::ReleaseAll(CObject::TYPE_BG);
	CObject::ReleaseAll(CObject::TYPE_BLOCK);
	CObject::ReleaseAll(CObject::TYPE_CUBE);
	CObject::ReleaseAll(CObject::TYPE_PLAYER);
	CObject::ReleaseAll(CObject::TYPE_ENEMY);
	CObject::ReleaseAll(CObject::TYPE_EFFECT);
	CObject::ReleaseAll(CObject::TYPE_PARTICLE);
	CObject::ReleaseAll(CObject::TYPE_TIME);
	CObject::ReleaseAll(CObject::TYPE_SCORE);
	CObject::ReleaseAll(CObject::TYPE_FONT);

	// メモリ開放
	delete m_LoadText;
	m_LoadText = NULL;
}

//========================================
// 更新
//========================================
void CTutorial::Update(void)
{
	// -- 取得 -------------------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット

	// ポーズ
	if (pInputKeyboard->GetTrigger(DIK_P) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_START))
	{
		CPause::SetPause(true);
	}

	bool bPause = CPause::IsPause();
	if (!bPause)
	{
		// 各テキストの処理
		TutorialTex();
	}

	// 開始フラグ
	{
		bool bStart = CTitle::IsStart();
		if (!bStart && --m_nStartTime <= 0)
		{
			CTitle::SetStart(true);
		}
	}

	// エネミーの全滅
	{
		bool bClear = CTitle::IsClear();
		if (!bClear && CEnemy::GetEnemyAll() <= 0 && m_aCreateText.nCurAction >= ACTION_ENEMY)
		{
			CTitle::SetClear(true);
		}
	}
}

//========================================
// 描画
//========================================
void CTutorial::Draw(void)
{

}

//========================================
// 生成
//========================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = new CTutorial;

	pTutorial->Init();

	return pTutorial;
}

//========================================
// 読み込み
//========================================
void CTutorial::TextLoad(void)
{
	CSVFILE *pFile = new CSVFILE;

	pFile->FileLood(FILE_PATH, true, true, ',');

	// 行数の取得
	int nRowMax = pFile->GetRowSize();

	// メモリ確保
	m_LoadText = new CTutorial::LoadTxt[nRowMax];

	for (int nRow = 0; nRow < nRowMax; nRow++)
	{
		// 初期化
		TxtInit(nRow);

		// 列数の取得
		int nLineMax = pFile->GetLineSize(nRow);

		for (int nLine = 0; nLine < nLineMax; nLine++)
		{
			string sData = pFile->GetData(nRow, nLine);

			switch (nLine)
			{
			case 0:	pFile->ToValue(m_LoadText[nRow].nType, sData); break;		// 種類
			case 1:	pFile->ToValue(m_LoadText[nRow].pos.x, sData); break;		// 位置 X
			case 2:	pFile->ToValue(m_LoadText[nRow].pos.y, sData); break;		// 　　 Y
			case 3:	pFile->ToValue(m_LoadText[nRow].size.x, sData); break;		// サイズ X
			case 4:	pFile->ToValue(m_LoadText[nRow].size.y, sData); break;		// 　　　 Y
			case 5:	pFile->ToValue(m_LoadText[nRow].nStartTime, sData); break;	// 表示時間
			case 6:	pFile->ToValue(m_LoadText[nRow].nStandTime, sData); break;	// 待機時間
			case 7:	pFile->ToValue(m_LoadText[nRow].nDisapTime, sData); break;	// 消える時間
			case 8:	pFile->ToValue(m_LoadText[nRow].nCreateTime, sData); break;	// 生成待機時間
			case 9:	pFile->ToValue(m_LoadText[nRow].bTextBok, sData); break;	// Box表示
			case 10:pFile->ToValue(m_LoadText[nRow].nTextSize, sData); break;	// テキスト サイズ
			case 11:pFile->ToValue(*m_LoadText[nRow].ActionTex, sData); break;	// テキスト
			}
		}

		// 最大数に達したら返す
		if (nRow == nRowMax - 1)	// (列数 - 列の最大数 - ヘッダーの列数)
		{
			delete pFile;
			pFile = NULL;
			return;
		}

		{
			int nType = m_LoadText[nRow].nType;
			m_aCreateText.nCurMax[nType]++;
		}
	}

	delete pFile;
	pFile = NULL;
}

//========================================
// テキスト情報の初期化
//========================================
void CTutorial::TxtInit(int nIdx)
{
	m_LoadText[nIdx].pos = INIT_D3DXVECTOR3;
	m_LoadText[nIdx].size = INIT_D3DXVECTOR2;
	m_LoadText[nIdx].nType = 0;
	m_LoadText[nIdx].nStartTime = 0;
	m_LoadText[nIdx].nStandTime = 0;
	m_LoadText[nIdx].nDisapTime = 0;
	*m_LoadText[nIdx].ActionTex = NULL;
	m_LoadText[nIdx].nTextSize = 0;
	m_LoadText[nIdx].bTextBok = false;
}

//========================================
// テキスト生成
//========================================
void CTutorial::TxtCreate(int nType)
{
	m_nCurAction = nType;

	if (m_aCreateText.nCurAction == ACTION_MAX)
	{
		return;
	}

	if (--m_aCreateText.nCreateTime <= 0 && !m_aCreateText.bCreate[nType])
	{
		int nNumCur = m_aCreateText.nNumCur;
		int nNumCurAll = m_aCreateText.nNumCurAll;

		m_aCreateText.nCreateTime = 0;

		if (nNumCur < m_aCreateText.nCurMax[nType])
		{
			FormFont pFont = {
				INIT_D3DXCOLOR,
				m_LoadText[nNumCurAll].nTextSize,
				m_LoadText[nNumCurAll].nStartTime,
				m_LoadText[nNumCurAll].nStandTime,
				m_LoadText[nNumCurAll].nDisapTime
			};

			int nText = m_aCreateText.nNumCur;

			m_Txt[nText] = CText::Create(CText::BOX_NORMAL_RECT,
				m_LoadText[nNumCurAll].pos,
				m_LoadText[nNumCurAll].size,
				*m_LoadText[nNumCurAll].ActionTex,
				CFont::FONT_BESTTEN,
				&pFont,
				m_LoadText[nNumCurAll].bTextBok);

			// 最低値の設定
			if (m_LoadText[nNumCurAll].nDisapTime <= -1)
			{
				m_LoadText[nNumCurAll].nDisapTime = 5;
			}

			int nStrlen = (strlen(*m_LoadText[nNumCurAll].ActionTex) / 2);

			m_aCreateText.nCreateTime = (nStrlen * m_LoadText[nNumCurAll].nStartTime) +
				m_LoadText[nNumCurAll].nStandTime + (m_LoadText[nNumCurAll].nDisapTime - 5);

			m_aCreateText.nNumCur++;
			m_aCreateText.nNumCurAll++;

			if (m_aCreateText.nNumCur == m_aCreateText.nCurMax[nType])
			{
				if (m_LoadText[nNumCurAll].nDisapTime >= 1)
				{
					m_aCreateText.nCreateTime = (nStrlen * m_LoadText[nNumCurAll].nStartTime) +
						m_LoadText[nNumCurAll].nStandTime;
				}
			}
		}
		if (m_aCreateText.nNumCur == m_aCreateText.nCurMax[nType])
		{
			m_aCreateText.bCreate[nType] = true;
		}
	}
	else if (m_aCreateText.nCreateTime <= 0 && m_aCreateText.bCreate[nType])
	{
		for (int nCnt = 0; nCnt < m_aCreateText.nCurMax[nType]; nCnt++)
		{
			if (m_LoadText[m_aCreateText.nNumCurAll - 1].nDisapTime >= 6)
			{
				if (m_Txt[nCnt] != NULL)
				{
					m_Txt[nCnt]->Disap(true, m_LoadText[m_aCreateText.nNumCurAll - 1].nDisapTime);
					m_Txt[nCnt] = NULL;
				}
			}
		}
	}
}

//========================================
// テキスト削除
//========================================
void CTutorial::TxtDelete(int nType, int nNextType)
{
	for (int nCnt = 0; nCnt < m_aCreateText.nCurMax[nType]; nCnt++)
	{
		if (m_Txt[nCnt] != NULL)
		{
			m_Txt[nCnt]->Uninit();
			m_Txt[nCnt] = NULL;
		}
	}

	m_aCreateText.nNumCur = 0;
	m_aCreateText.nCurAction = nNextType;

	int nNumCurAll = m_aCreateText.nNumCurAll - 1;
	if (nNumCurAll >= 1)
	{
		m_aCreateText.nCreateTime = m_LoadText[nNumCurAll].nCreateTime;
	}
}

//========================================
// チュートリアル
//========================================
void CTutorial::TutorialTex(void)
{
	// -- 取得 -------------------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CMouse *pInputMouse = CManager::GetInputMouse();			// マウス
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット

	// テキストの生成
	TxtCreate(m_aCreateText.nCurAction);

	switch (m_aCreateText.nCurAction)
	{
	case ACTION_MOVE:// 移動
	{
		D3DXVECTOR3 move = m_pPlayer->GetPlayerMove();
		
		if (move.x <= -1.0f || move.x >= 1.0f)
		{
			if (m_aCreateText.bCreate[ACTION_MOVE])
			{
				TxtDelete(ACTION_MOVE,ACTION_CAMERA);
			}
		}
	}
		break;
	case ACTION_CAMERA:	// 視点移動
	{
		D3DXVECTOR3 move = CManager::GetCamera()->GetInfo().spin;

		if (move.y <= -0.01f || move.y >= 0.01f)
		{
			if (m_aCreateText.bCreate[ACTION_CAMERA])
			{
				TxtDelete(ACTION_CAMERA, ACTION_SHOT);
			}
		}
	}
		break;
	case ACTION_SHOT:	// 発射
	{
		int nNumAll = CBullet::GetNumAll();

		if (nNumAll > 0 && m_aCreateText.bCreate[ACTION_SHOT])
		{
			TxtDelete(ACTION_SHOT, ACTION_SET);
		}
	}
	break;
	case ACTION_SET1:	// 配置(説明)
	{
		if (m_aCreateText.bCreate[ACTION_SET1] && m_aCreateText.nCreateTime <= 0)
		{
			TxtDelete(ACTION_SET1, ACTION_ENEMY);

			// 敵の生成
			LoodEnemy();

			// スコア生成
			m_pScore = CScore::Create();
			CScore::SetScore();
		}
	}
	break;
	case ACTION_ENEMY:	// 敵
	{
		int nNumEnemy = CEnemy::GetEnemyAll();

		if (nNumEnemy == 0 && m_aCreateText.bCreate[ACTION_ENEMY])
		{
			TxtDelete(ACTION_ENEMY, ACTION_CLEAR);

			// タイム生成
			m_pTime = CTime::Create(MAX_TIME);
		}
	}
	break;
	case ACTION_FREE:// 終了
	{
		if (m_aCreateText.nCreateTime <= 0 && m_aCreateText.bCreate[ACTION_FREE])
		{
			TxtDelete(ACTION_FREE, ACTION_MAX);

			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(MODE_TITLE);
			}
		}
	}
	break;
	default:
	{
		int nCurAction = m_aCreateText.nCurAction;

		if (m_aCreateText.nCreateTime <= 0)
		{
			if (m_aCreateText.bCreate[nCurAction] && nCurAction != ACTION_MAX)
			{
				TxtDelete(nCurAction, nCurAction + 1);
			}
		}
	}
		break;
	}
}

//========================================
// 天井
//========================================
void CTutorial::LoodCeiling(void)
{
	CBgCeiling *pBgCeiling = CBgCeiling::Create(0);
}

//========================================
// 側面
//========================================
void CTutorial::LoodSide(void)
{
	// 変数宣言
	char aDataSearch[128] = {};		// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数

									// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(SIDE_FILE, "r");

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
			else if (!strcmp(aDataSearch, "CREATE_SIDE"))
			{// 生成開始

				while (1)
				{
					fscanf(pFile, "%s", aDataSearch);

					if (!strcmp(aDataSearch, "END_CREATE_SIDE"))
					{// 生成終了
						break;
					}
					else if (!strcmp(aDataSearch, "SET_SIDE"))
					{// 生成情報の読み込み
						D3DXVECTOR3 pos = INIT_D3DXVECTOR3;	// 位置
						D3DXVECTOR3 rot = INIT_D3DXVECTOR3;	// 向き
						D3DXCOLOR	color = INIT_D3DXCOLOR;	// 色
						float fHeight = 0;					// 高さ
						float fRadius = 0;					// 半径
						int *nType = 0;						// 画像種類
						int nNumTex = 0;					// 画像数
						int nDivisionX = 1;					// 分割幅
						int nDivisionY = 1;					// 分割高さ
						float *fTexV = 0;					// テクスチャ座標の開始位置(X軸)

						int nCntTex = 0; // 画像カウント

						while (1)
						{
							fscanf(pFile, "%s", aDataSearch);

							if (!strcmp(aDataSearch, "END_SIDE"))
							{// 読み込みを終了

								CBgSide *pBgSide = CBgSide::Create(
									pos, rot, color, fHeight, fRadius, nType, nNumTex,
									nDivisionX, nDivisionY, fTexV);

								break;
							}
							else if (!strcmp(aDataSearch, "POS"))
							{// 位置
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
							}
							else if (!strcmp(aDataSearch, "ROT"))
							{// 向き
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
							else if (!strcmp(aDataSearch, "COLOR"))
							{// 色
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &color.r);
								fscanf(pFile, "%f", &color.g);
								fscanf(pFile, "%f", &color.b);
								fscanf(pFile, "%f", &color.a);
							}
							else if (!strcmp(aDataSearch, "HEIGHT"))
							{// 高さ
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &fHeight);
							}
							else if (!strcmp(aDataSearch, "RADIUS"))
							{// 半径
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &fRadius);
							}
							else if (!strcmp(aDataSearch, "NUMTEX"))
							{// 画像数
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nNumTex);

								// メモリの解放
								nType = new int[nNumTex];
								fTexV = new float[nNumTex];
							}
							else if (!strcmp(aDataSearch, "SET_TEX"))
							{// 画像関連の設定

								while (1)
								{
									fscanf(pFile, "%s", aDataSearch);

									if (!strcmp(aDataSearch, "END_TEX"))
									{// 読み込みを終了
										nCntTex++;
										break;
									}
									else if (!strcmp(aDataSearch, "TYPE"))
									{// 半径
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%d", &nType[nCntTex]);
									}
									else if (!strcmp(aDataSearch, "TEX_V"))
									{// 画像数
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &fTexV[nCntTex]);
									}
								}
							}
							else if (!strcmp(aDataSearch, "DIVISION_X"))
							{// 分割幅
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nDivisionX);
							}
							else if (!strcmp(aDataSearch, "DIVISION_Y"))
							{// 分割高さ
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nDivisionY);
							}
						}
					}
				}
			}
		}
	}
}

//========================================
// 床
//========================================
void CTutorial::LoodFloor(void)
{
	// 変数宣言
	char aDataSearch[128] = {};		// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数

									// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(FLOOR_FILE, "r");

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
			else if (!strcmp(aDataSearch, "CREATE_FLOOR"))
			{// 生成開始

				while (1)
				{
					fscanf(pFile, "%s", aDataSearch);

					if (!strcmp(aDataSearch, "END_CREATE_FLOOR"))
					{// 生成終了
						break;
					}
					else if (!strcmp(aDataSearch, "SET_FLOOR"))
					{// 生成情報の読み込み
						D3DXVECTOR3 pos = INIT_D3DXVECTOR3;	// 位置
						D3DXVECTOR3 rot = INIT_D3DXVECTOR3;	// 向き
						D3DXCOLOR	color = INIT_D3DXCOLOR;	// 色
						float fWidth = 0;					// 幅
						float fHeight = 0;					// 高さ
						int *nType = 0;						// 画像種類
						int nNumTex = 0;					// 画像数
						int nDivisionX = 1;					// 分割幅
						int nDivisionY = 1;					// 分割高さ
						bool bDivision = false;				// テクスチャの分割するか
						float *fTexX = 0;					// テクスチャ座標の開始位置(X軸)
						float *fTexY = 0;					// テクスチャ座標の開始位置(Y軸)


						int nCntTex = 0; // 画像カウント

						while (1)
						{
							fscanf(pFile, "%s", aDataSearch);

							if (!strcmp(aDataSearch, "END_FLOOR"))
							{// 読み込みを終了

								CFloor *pBgSide = CFloor::Create(
									pos, rot, color, fWidth, fHeight, nType, nNumTex, bDivision,
									nDivisionX, nDivisionY, fTexX, fTexY);

								break;
							}
							else if (!strcmp(aDataSearch, "POS"))
							{// 位置
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
							}
							else if (!strcmp(aDataSearch, "ROT"))
							{// 向き
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
							else if (!strcmp(aDataSearch, "COLOR"))
							{// 色
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &color.r);
								fscanf(pFile, "%f", &color.g);
								fscanf(pFile, "%f", &color.b);
								fscanf(pFile, "%f", &color.a);
							}
							else if (!strcmp(aDataSearch, "WIDTH"))
							{// 幅
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &fWidth);
							}
							else if (!strcmp(aDataSearch, "HEIGHT"))
							{// 高さ
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &fHeight);
							}
							else if (!strcmp(aDataSearch, "NUMTEX"))
							{// 画像数
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nNumTex);

								// メモリの解放
								nType = new int[nNumTex];
								fTexX = new float[nNumTex];
								fTexY = new float[nNumTex];
							}
							else if (!strcmp(aDataSearch, "DIVISION"))
							{// テクスチャの分割するか
								int nData;

								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nData);

								if (nData <= 0)
								{
									bDivision = false;
								}
								else
								{
									bDivision = true;
								}
							}
							else if (!strcmp(aDataSearch, "SET_TEX"))
							{// 画像関連の設定

								while (1)
								{
									fscanf(pFile, "%s", aDataSearch);

									if (!strcmp(aDataSearch, "END_TEX"))
									{// 読み込みを終了
										nCntTex++;
										break;
									}
									else if (!strcmp(aDataSearch, "TYPE"))
									{// 半径
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%d", &nType[nCntTex]);
									}
									else if (!strcmp(aDataSearch, "TEX_X"))
									{// テクスチャの移動量(X軸)
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &fTexX[nCntTex]);
									}
									else if (!strcmp(aDataSearch, "TEX_Y"))
									{// テクスチャの移動量(Y軸)
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &fTexY[nCntTex]);
									}
								}
							}
							else if (!strcmp(aDataSearch, "DIVISION_X"))
							{// 分割幅
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nDivisionX);
							}
							else if (!strcmp(aDataSearch, "DIVISION_Y"))
							{// 分割高さ
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nDivisionY);
							}
						}
					}
				}
			}
		}
	}
}

//========================================
// ブロックの読み込み
//========================================
void CTutorial::LoodBlock(void)
{
	CSVFILE *pFile = new CSVFILE;

	// 読み込み
	pFile->FileLood(BLOCK_FILE1, true, true, ',');

	// 行数の取得
	int nRowMax = pFile->GetRowSize();

	// 各データに代入
	for (int nRow = 0; nRow < nRowMax; nRow++)
	{
		// 配置情報の生成
		int nType;				// 種類
		int nState;				// 状態
		D3DXVECTOR3 pos;		// 位置

								// 列数の取得
		int nLineMax = pFile->GetLineSize(nRow);

		for (int nLine = 0; nLine < nLineMax; nLine++)
		{
			string sData = pFile->GetData(nRow, nLine);

			switch (nLine)
			{
			case 0:	pFile->ToValue(nType, sData); break;	// 種類
			case 1:	pFile->ToValue(pos.x, sData); break;	// 位置 X
			case 2:	pFile->ToValue(pos.y, sData); break;	// 位置 Y
			case 3:	pFile->ToValue(pos.z, sData); break;	// 位置 Z
			case 4: pFile->ToValue(nState, sData); break;	// 状態
			}
		}

		// 最大数に達したら返す
		if (nRow == nRowMax - 1)	// (列数 - 列の最大数 - ヘッダーの列数)
		{
			delete pFile;
			pFile = NULL;
			return;
		}
		if (nRow == 150)
		{
			int n = nRow;
		}

		// 配置
		CBlock *pObj = CBlock::Create(nType, pos, nState);
	}

	// メモリ開放
	delete pFile;
	pFile = NULL;
}

//========================================
// エネミーの読み込み
//========================================
void CTutorial::LoodEnemy(void)
{
	CSVFILE *pFile = new CSVFILE;

	pFile->FileLood(ENEMY_FILE1, true, true, ',');

	// 行数の取得
	int nRowMax = pFile->GetRowSize();

	for (int nRow = 0; nRow < nRowMax; nRow++)
	{
		int nType, nMove, nTime;
		float fRotY;
		D3DXVECTOR3 pos;

		// 列数の取得
		int nLineMax = pFile->GetLineSize(nRow);

		for (int nLine = 0; nLine < nLineMax; nLine++)
		{
			string sData = pFile->GetData(nRow, nLine);

			switch (nLine)
			{
			case 0:	pFile->ToValue(nType, sData); break;	// 種類
			case 1:	pFile->ToValue(nMove, sData); break;	// 移動種類
			case 2:	pFile->ToValue(pos.x, sData); break;	// 位置 X
			case 3:	pFile->ToValue(pos.y, sData); break;	// 位置 Y
			case 4:	pFile->ToValue(pos.z, sData); break;	// 位置 Z
			case 5: pFile->ToValue(fRotY, sData); break;	// 向きY
			case 6: pFile->ToValue(nTime, sData); break;	// 行動カウント
			}
		}

		// 最大数に達したら返す
		if (nRow == nRowMax - 1)	// (列数 - 列の最大数 - ヘッダーの列数)
		{
			delete pFile;
			pFile = NULL;
			return;
		}

		CEnemy *pObj = CEnemy::Create(nType, nMove, pos, D3DXVECTOR3(0.0f, fRotY, 0.0f), nTime);
	}

	delete pFile;
	pFile = NULL;
}