//========================================
// 
// ゲーム
// 
//========================================
// *** game.cpp ***
//========================================
#include "title.h"
#include "game.h"
#include "ranking.h"
#include "../system/csv_file.h"
#include "../object\model\block.h"
#include "../object/model/bullet.h"
#include "../object/model/enemy.h"
#include "../object/model/bullet_cube.h"
#include "../object\model\player.h"
#include "../object\model\enemy.h"
#include "../object\BG\bg_side.h"
#include "../object\BG\bg_ceiling.h"
#include "../object/BG/mesh_floor.h"
#include "../object/BG/blackout.h"
#include "../object\UI\score.h"
#include "../object\UI\time.h"
#include "../object/UI/crown.h"
#include "../system/input.h"
#include "../system/words/text.h"
#include "../system/words/font.h"
#include "../system/camera.h"
#include "../system/sound.h"
#include "../scene/pause.h"
#include "../object/model/model.h"
#include "fade.h"

// 静的変数
CPlayer *CGame::m_pPlayer = NULL;
CTime *CGame::m_pTime = NULL;
CScore *CGame::m_pScore = NULL;
bool CGame::m_bTime = false;
bool CGame::m_bEnd = false;
bool CGame::m_bGemeOver = false;
int CGame::m_nStage = STAGE_00;
int CGame::m_nSelectStage = STAGE_00;
int CGame::m_nScore = 0;
CGame::StageInfo CGame::m_aStageInfo = { NULL,NULL };

const char* CGame::STAGE_INFO_FILE = "data\\GAMEDATA\\STAGE_DATA.txt";
const char* CGame::SIDE_STAGE_FILE = "data\\GAMEDATA\\OBJECT\\SIDE_DATA .txt";
const char* CGame::FLOOR_STAGE_FILE = "data\\GAMEDATA\\OBJECT\\FLOOR_DATA.txt";

//========================================
// コンストラクタ
//========================================
CGame::CGame()
{
	m_rot = INIT_D3DXVECTOR3;
	m_nMoveRot = INIT_FLOAT;
	m_nStartTime = 0;
	m_nEndTime = 0;
	m_nRstStgType = 0;
	m_nTextCreate = 0;

	m_nTimeTotal = 0;
	m_nClearTotal = 0;
	m_nTotal = 0;
	m_nAddTime = 0;
	m_nStandTime = -1;
	m_nEveGame = 1;
	m_bAddScore = false;

	m_bSpecial = false;
	m_bTime = false;
	m_bEnd = false;
	m_bGemeOver = false;


	for (int nRst = 0; nRst < RST_ADD_SCORE; nRst++)
	{
		m_RstText[nRst] = NULL;
	}
}

//========================================
// デストラクタ
//========================================
CGame::~CGame()
{

}

//========================================
// 初期化
//========================================
HRESULT CGame::Init(void)
{
	CEnemy::Reset();
	CBullet::Reset();
	CCube::Reset();

	CBlock::Load();			// ブロック
	CModel::InitModel();	// モデル

	m_rot = INIT_D3DXVECTOR3;
	m_nStartTime = 0;
	m_nEndTime = 0;
	m_bEnd = false;
	m_bGemeOver = false;
	m_bTime = false;

	m_nRstStgType = 0;
	m_nTextCreate = 0;
	m_nTimeTotal = 0;
	m_nClearTotal = 0;
	m_nEveGame = 0;
	m_nTotal = 0;
	m_nAddTime = ADDTIME_MAX;

	CTitle::SetStart(false);
	CTitle::SetClear(false);

	// 背景(側面)の生成
	LoodSide();

	// 背景(天井)の生成
	LoodCeiling();

	// 床の生成
	LoodFloor();

	// ブロックの生成
	LoodBlock();

	CCamera *pCamera = CManager::GetCamera();					// カメラ
	pCamera->Init();
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pCamera->SetHeigth(0.4f);
	pCamera->SetDistance(1400.0f);

	CPlayer *pPlayer = CPlayer::Create(m_aStageInfo.PlayerPos[m_nStage], m_aStageInfo.PlayerRot[m_nStage]);
	pPlayer->SetMotion("data\\GAMEDATA\\MODEL\\Player\\PLAYER_DATA.txt");

	// 敵の生成
	LoodEnemy();

	// キューブの使用数
	CCube::SetLimit(m_aStageInfo.nCube[m_nStage]);

	// タイム生成
	m_pTime = CTime::Create(m_aStageInfo.nTime[m_nStage]);

	// スコア生成
	m_pScore = CScore::Create();
	CScore::SetScore(m_nScore);

	FormFont pFont = { INIT_D3DXCOLOR,20.0f,10,60,30 };

	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),true,D3DXVECTOR3(2.0f,2.0f,0.0f),D3DXVECTOR2(1.0f,1.0f) };

	int nCntText = 0;
	char aString[TXT_MAX];
	char aCube[TXT_MAX];

	// 読み込み
	if (m_nStage != STAGE_04)
	{
		sprintf(aString, "STAGE %d", m_nStage + 1);
	}
	else if (m_nStage == STAGE_04)
	{
		sprintf(aString, "LAST STAGE", m_nStage + 1);
	}
	sprintf(aCube, "CUBE  %d", m_aStageInfo.nCube[m_nStage]);

	int nStrlen = strlen(aString);

	CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 325.0f, 0.0f), D3DXVECTOR2(440.0f, 100.0f),
		aString, CFont::FONT_BESTTEN, &pFont, false, &pShadow);

	CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 375.0f, 0.0f), D3DXVECTOR2(440.0f, 100.0f),
		aCube, CFont::FONT_BESTTEN, &pFont, false, &pShadow);

	m_nStartTime = (nStrlen * 10) + 60 + 10;
	m_nMoveRot = ((D3DX_PI * 2) / m_nStartTime);

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::TYPE_GAME01);

	return S_OK;
}

//========================================
// 終了
//========================================
void CGame::Uninit(void)
{
	CBlock::Reset();
	CModel::UninitModel();

	CObject::ReleaseAll(CObject::TYPE_BG);
	CObject::ReleaseAll(CObject::TYPE_BLOCK);
	CObject::ReleaseAll(CObject::TYPE_CUBE);
	CObject::ReleaseAll(CObject::TYPE_PLAYER);
	CObject::ReleaseAll(CObject::TYPE_ENEMY);
	CObject::ReleaseAll(CObject::TYPE_BULLET);
	CObject::ReleaseAll(CObject::TYPE_EFFECT);
	CObject::ReleaseAll(CObject::TYPE_PARTICLE);
	CObject::ReleaseAll(CObject::TYPE_TIME);
	CObject::ReleaseAll(CObject::TYPE_SCORE);
	CObject::ReleaseAll(CObject::TYPE_FONT);

	CSound *pSound = CManager::GetSound();
	pSound->StopSound();
}

//========================================
// 更新
//========================================
void CGame::Update(void)
{
	// -- 取得 -------------------------------------------
	CCamera *pCamera = CManager::GetCamera();					// カメラ
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット

																// ポーズ
	if (!m_bEnd)
	{
		if (pInputKeyboard->GetTrigger(DIK_P) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_START))
		{
			CPause::SetPause(true);
		}
	}

	// 開始フラグ
	{
		bool bStart = CTitle::IsStart();
		if (!bStart)
		{
			// カメラの注視点を設定
			pCamera->SetPosR(D3DXVECTOR3(0.0f, 95.0f, 0.0f));
			if (--m_nStartTime <= 0)
			{
				pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				pCamera->SetHeigth(0.4f);
				pCamera->SetDistance(500.0f);
				CTitle::SetStart(true);
			}
		}
	}

	// エネミーの全滅
	{
		bool bClear = CTitle::IsClear();
		if (!bClear && CEnemy::GetEnemyAll() <= 0)
		{
			CTitle::SetClear(true);
		}
	}

	if (CFade::GetFade() == CFade::FADE_NONE)
	{
		// CLEAR
		{
			bool bClear = CTitle::IsClear();
			int nCubeRest = CCube::GetRest();

			if (bClear)
			{// ゲームクリア
				if (!m_bEnd || (m_bEnd && bClear && m_bGemeOver && m_nEndTime <= 0))
				{
					char aString[TXT_MAX];
					D3DXCOLOR color, ShadowColor;
					if (m_bEnd && bClear && m_bGemeOver && m_pTime->GetTime() <= 0)
					{
						m_bSpecial = true;
						sprintf(aString, "LIMIT CLEAR");
						color = D3DXCOLOR(0.02f, 0.6f, 1.0f, 1);
						ShadowColor = INIT_D3DXCOLOR;
					}
					else
					{
						sprintf(aString, "STAGE CLEAR");
						color = D3DXCOLOR(1.0f, 0.96f, 0, 1);
						ShadowColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					}

					m_bEnd = true;

					FormFont pFont = { color,20.0f,7,60,30 };
					FormShadow pShadow = { ShadowColor,true,D3DXVECTOR3(2.0f,2.0f,0.0f),D3DXVECTOR2(1.0f,1.0f) };

					CText::Create(CText::BOX_NORMAL_RECT,
						D3DXVECTOR3(640.0f, 300.0f, 0.0f),
						D3DXVECTOR2(440.0f, 100.0f),
						aString,
						CFont::FONT_BESTTEN,
						&pFont, false, &pShadow);

					int nLength = strlen(aString);

					m_nEndTime = (nLength * 7) + 60 + 15;
					m_bEnd = true;
					m_bTime = true;
					m_bGemeOver = false;
				}
				else
				{
					if (--m_nEndTime <= 0 && !m_bGemeOver)
					{
						// リザルト演出
						Result();
					}
				}
			}
			else if (!bClear && (m_pTime->GetTime() <= 0 || nCubeRest <= 0))
			{// ゲームオーバー
				if (!m_bEnd && m_nStandTime == -1)
				{
					m_nStandTime = 30;
				}
				else if (!m_bEnd && --m_nStandTime <= 0)
				{
					FormFont pFont = { D3DXCOLOR(0.7f,0.0f,0.0f,1),20.0f,8,60,30 };
					FormShadow pShadow = { D3DXCOLOR(0.1f,0.1f,0.1f,1),true,D3DXVECTOR3(2.0f,2.0f,0.0f),D3DXVECTOR2(1.0f,1.0f) };

					CText::Create(CText::BOX_NORMAL_RECT,
						D3DXVECTOR3(640.0f, 300.0f, 0.0f),
						D3DXVECTOR2(440.0f, 100.0f),
						"GAME OVER",
						CFont::FONT_BESTTEN,
						&pFont, false, &pShadow);

					m_nEndTime = (7 * 8) + 60 + 15;
					m_bEnd = true;
					m_bTime = true;
					m_bGemeOver = true;
				}
				else if (m_bEnd)
				{
					if (--m_nEndTime <= 0)
					{
						CManager::GetFade()->SetFade(MODE_RANKING);
						CRanking::SetScore11(m_nScore);
						CRanking::SetAllStage(false);
					}
				}
			}

		}
	}
}

//========================================
// 描画
//========================================
void CGame::Draw(void)
{

}

//========================================
// 生成
//========================================
CGame *CGame::Create(void)
{
	CGame *pGame = new CGame;

	pGame->Init();

	return pGame;
}

//========================================
// リザルト演出
//========================================
void CGame::Result(void)
{
	FormFont pFont = { INIT_D3DXCOLOR, 18.0f, 2, 5, 0 };
	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(2.0f,2.0f,0.0f), D3DXVECTOR2(2.0f,2.0f) };

	char aString[TXT_MAX];
	int nLength = 0;
	D3DXVECTOR3 pos = INIT_D3DXVECTOR3;

	switch (m_nRstStgType)
	{
	case RST_TEXT:
	{
		CBlackout::Create();
		if (!m_bSpecial)
		{
			sprintf(aString, "STAGE CLEAR RESULT BONUS");
		}
		else
		{
			sprintf(aString, "LIMIT CLEAR RESULT BONUS");
		}
		pos = D3DXVECTOR3(23.0f, 105.0f, 0.0f);

		int nUseCube = CCube::GetRest();
		int nPerfCube = m_aStageInfo.nCube[m_nStage];

		m_nEveGame = EVE_PERFECT;
	}
	break;
	case RST_TIME:
	{
		if (!m_bSpecial)
		{
			sprintf(aString, "TIME BONUS");
			CCrown::Create(m_nEveGame);
		}
		else
		{
			sprintf(aString, "CUBE BONUS");
		}
		pos = D3DXVECTOR3(100.0f, 190.0f, 0.0f);
	}
	break;
	case RST_TIME_CALC:
	{
		int nTime = m_pTime->GetTime();
		if (!m_bSpecial)
		{
			m_nTimeTotal = TIME_SCORE * nTime;
			sprintf(aString, "%d * %d = %d", TIME_SCORE, nTime, m_nTimeTotal);
		}
		else
		{
			nTime = CCube::GetRest();
			m_nTimeTotal = CUBE_SCORE * nTime;
			sprintf(aString, "%d * %d = %d", CUBE_SCORE, nTime, m_nTimeTotal);
		}
		pos = D3DXVECTOR3(100.0f, 260.0f, 0.0f);
	}
	break;
	case RST_CLEAR:
	{
		if (!m_bSpecial)
		{
			sprintf(aString, "STAGE CLEAR BONUS");
		}
		else
		{
			sprintf(aString, "LIMIT CLEAR BONUS");
		}
		pos = D3DXVECTOR3(100.0f, 340.0f, 0.0f);
	}
	break;
	case RST_CLEAR_CALC:
	{
		int nClear = m_aStageInfo.nClearBonus[m_nStage];

		int nPerfCube = m_aStageInfo.nCube[m_nStage];

		if (!m_bSpecial)
		{
			m_nClearTotal = nClear;
		}
		else
		{
			m_nClearTotal = nClear * (m_nStage + 2);
		}

		sprintf(aString, "%d", m_nClearTotal);
		pos = D3DXVECTOR3(100.0f, 410.0f, 0.0f);
	}
	break;
	case RST_BONUS:
	{
		sprintf(aString, "TOTAL BONUS");
		pos = D3DXVECTOR3(100.0f, 500.0f, 0.0f);
	}
	break;
	case RST_BONUS_CALC:
	{

		m_nTotal = m_nTimeTotal + m_nClearTotal;

		sprintf(aString, "%d", m_nTotal);
		pos = D3DXVECTOR3(100.0f, 550.0f, 0.0f);
	}
	break;
	case RST_ADD_SCORE:
	{
		if (m_bAddScore)
		{
			if (m_nTotal <= 0)
			{
				CSound *pSound = CManager::GetSound();
				pSound->StopSoundType(CSound::SOUND_TYPE_SE);

				if (++m_nStage < STAGE_MAX)
				{
					char aString[TXT_MAX];

					if (m_nStage != (STAGE_MAX - 1))
					{
						sprintf(aString, "NEXT⇒STAGE%d", m_nStage + 1);
						pos = D3DXVECTOR3(1000.0f, 650.0f, 0.0f);
					}
					else if (m_nStage == (STAGE_MAX - 1))
					{
						sprintf(aString, "NEXT⇒LAST STAGE", m_nStage + 1);
						pos = D3DXVECTOR3(900.0f, 650.0f, 0.0f);
					}
					int nStrlen = strlen(aString);

					CText::Create(CText::BOX_NORMAL_RECT,
						pos, D3DXVECTOR2(440.0f, 100.0f),
						aString, CFont::FONT_BESTTEN, &pFont, false, &pShadow);

					m_nStandTime = (nStrlen * 2) + 30 + 10;
				}
				else if (m_nStage >= STAGE_MAX)
				{
					char aString[TXT_MAX];
					sprintf(aString, "NEXT⇒RANKING");

					int nStrlen = strlen(aString);

					CText::Create(CText::BOX_NORMAL_RECT,
						D3DXVECTOR3(1000.0f, 650.0f, 0.0f), D3DXVECTOR2(440.0f, 100.0f),
						aString, CFont::FONT_BESTTEN, &pFont, false, &pShadow);

					m_nStandTime = 90;
				}

				m_nRstStgType++;
			}
			else
			{
				int AddScore;
				AddScore = (m_nTotal / m_nAddTime);
				m_nTotal -= AddScore;
				m_nAddTime--;

				if (m_nTotal <= 0)
				{
					m_nTotal = 0;
					AddScore = AddScore - m_nTotal;
				}

				CScore::SetScore(AddScore);

				char aTotal[TXT_MAX];

				sprintf(aTotal, "%d", m_nTotal);
				m_RstText[RST_BONUS_CALC]->ChgText(aTotal, INIT_D3DXCOLOR);
			}
		}
		else
		{
			if (--m_nTextCreate <= 0)
			{
				m_nTextCreate = 0;
				m_bAddScore = true;

				CSound *pSound = CManager::GetSound();
				pSound->PlaySound(CSound::TYPE_ADD_SCORE);
			}
		}
	}
	break;
	case RST_STAND:
	{
		if (m_nStandTime-- <= 0)
		{
			m_nRstStgType++;
		}
	}
	break;
	case RST_END:
	{
		if (m_nStage < STAGE_MAX)
		{
			m_nScore = m_pScore->GetScore();
			CManager::GetFade()->SetFade(MODE_GAME);
		}
		else if (m_nStage >= STAGE_MAX)
		{
			CManager::GetFade()->SetFade(MODE_RANKING);
			m_nScore = m_pScore->GetScore();
			CRanking::SetScore11(m_nScore);
			CRanking::SetAllStage(false);
			CPause::SetPause(false);
		}
	}
	break;
	}

	// テキストの生成
	if (m_nRstStgType < RST_ADD_SCORE)
	{
		if (--m_nTextCreate <= 0)
		{
			if (m_nRstStgType == RST_TEXT || m_nRstStgType == RST_TIME || m_nRstStgType == RST_BONUS || m_nRstStgType == RST_CLEAR)
			{
				if (!m_bSpecial)
				{
					pFont = { D3DXCOLOR(1.0f,0.96f,0,1)	, 20.0f, 2, 5, 0 };
					pShadow = { INIT_D3DXCOLOR, true, D3DXVECTOR3(1.0f,1.0f,0.0f), D3DXVECTOR2(1.0f,1.0f) };
				}
				else
				{
					pFont = { D3DXCOLOR(0.02f,0.6f,1.0f,1)	, 20.0f, 2, 5, 0 };
					pShadow = { INIT_D3DXCOLOR, true, D3DXVECTOR3(1.0f,1.0f,0.0f), D3DXVECTOR2(1.0f,1.0f) };
				}
			}

			m_nTextCreate = 0;
			nLength = strlen(aString);

			m_RstText[m_nRstStgType] = CText::Create(CText::BOX_NORMAL_RECT, pos, D3DXVECTOR2(0.0f, 0.0f),
				aString, CFont::FONT_BESTTEN, &pFont, false, &pShadow);

			if (m_nRstStgType == RST_TEXT)
			{
				m_nTextCreate = (nLength * 1) + 5;
			}
			else
			{
				m_nTextCreate = (nLength * 3) + 5;
			}
			m_nRstStgType++;
		}
	}
}

//========================================
// リセット
//========================================
void CGame::Reset(void)
{
	m_bEnd = false;
	m_nStage = STAGE_00;
	m_nSelectStage = STAGE_00;
	m_bTime = false;
	m_nScore = 0;
	m_aStageInfo = { NULL,NULL };
}

//================================================================================
//--------------------------------------------------------------------------------
// 設定
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// ステージ選択
//========================================
void CGame::SetStage(int nStage)
{
	m_nSelectStage = nStage;
}

//================================================================================
//--------------------------------------------------------------------------------
// 読み込み
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// ステージ情報
//========================================
void CGame::LoodStage(void)
{
	char aDataSearch[TXT_MAX];	// データ検索用

								// ファイルの読み込み
	FILE *pFile = fopen(STAGE_INFO_FILE, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// END_SCRIPTが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", &aDataSearch[0]);

		if (!strcmp(aDataSearch, "END_SCRIPT"))
		{// 読み込みを終了
			fclose(pFile);
			break;
		}
		else if (aDataSearch[0] == '#')
		{// 折り返す
			continue;
		}
		else if (!strcmp(aDataSearch, "SET_INFO"))
		{// 生成開始

			int nCntStage = 0;
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);

				if (!strcmp(aDataSearch, "END_INFO"))
				{// 生成終了
					break;
				}
				else if (!strcmp(aDataSearch, "SET_STAGE"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);

						if (!strcmp(aDataSearch, "END_STAGE"))
						{// 生成終了
							nCntStage++;
							break;
						}
						else if (nCntStage > STAGE_MAX)
						{// 折り返す
							continue;
						}
						else if (!strcmp(aDataSearch, "TIME"))
						{
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%d", &m_aStageInfo.nTime[nCntStage]);
						}
						else if (!strcmp(aDataSearch, "CUBE"))
						{
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%d", &m_aStageInfo.nCube[nCntStage]);
						}
						else if (!strcmp(aDataSearch, "CLEAR"))
						{
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%d", &m_aStageInfo.nClearBonus[nCntStage]);
						}
						else if (!strcmp(aDataSearch, "BLOCK"))
						{
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%s", &m_aStageInfo.aBlockFile[nCntStage][0]); // ファイル名
						}
						else if (!strcmp(aDataSearch, "PLAYER_POS"))
						{
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%f", &m_aStageInfo.PlayerPos[nCntStage].x);
							fscanf(pFile, "%f", &m_aStageInfo.PlayerPos[nCntStage].y);
							fscanf(pFile, "%f", &m_aStageInfo.PlayerPos[nCntStage].z);
						}
						else if (!strcmp(aDataSearch, "PLAYER_ROT"))
						{
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%f", &m_aStageInfo.PlayerRot[nCntStage]);
						}
					}
				}
				else if (!strcmp(aDataSearch, "ENEMY"))
				{
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_aStageInfo.aEnemyFile[0]); // ファイル名
				}
			}
		}
	}
}

//========================================
// 天井
//========================================
void CGame::LoodCeiling(void)
{
	CBgCeiling *pBgCeiling = CBgCeiling::Create(m_nSelectStage);
}

//========================================
// 側面
//========================================
void CGame::LoodSide(void)
{
	// 変数宣言
	char aDataSearch[128] = {};		// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数

									// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(SIDE_STAGE_FILE, "r");

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

								delete[] nType;
								nType = NULL;

								delete[] fTexV;
								fTexV = NULL;

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
void CGame::LoodFloor(void)
{
	// 変数宣言
	char aDataSearch[128] = {};		// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数

									// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(FLOOR_STAGE_FILE, "r");

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
// ブロック
//========================================
void CGame::LoodBlock(void)
{
	CSVFILE *pFile = new CSVFILE;

	// 読み込み
	pFile->FileLood(m_aStageInfo.aBlockFile[m_nStage], true, true, ',');

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

		// 配置
		CBlock *pObj = CBlock::Create(nType, pos, nState);
	}

	// メモリ開放
	delete pFile;
	pFile = NULL;
}

//========================================
// エネミー
//========================================
void CGame::LoodEnemy(void)
{
	CSVFILE *pFile = new CSVFILE;

	// 読み込み
	pFile->FileLood(m_aStageInfo.aEnemyFile, true, true, ',');

	// 行数の取得
	int nRowMax = pFile->GetRowSize();

	for (int nRow = 0; nRow < nRowMax; nRow++)
	{
		int nStage, nType, nMove, nTime;
		float fRotY;
		D3DXVECTOR3 pos;

		// 列数の取得
		int nLineMax = pFile->GetLineSize(nRow);

		for (int nLine = 0; nLine < nLineMax; nLine++)
		{
			string sData = pFile->GetData(nRow, nLine);

			switch (nLine)
			{
			case 0: pFile->ToValue(nStage, sData); break;	// ステージ
			case 1:	pFile->ToValue(nType, sData); break;	// 種類
			case 2:	pFile->ToValue(nMove, sData); break;	// 移動種類
			case 3:	pFile->ToValue(pos.x, sData); break;	// 位置 X
			case 4:	pFile->ToValue(pos.y, sData); break;	// 位置 Y
			case 5:	pFile->ToValue(pos.z, sData); break;	// 位置 Z
			case 6: pFile->ToValue(fRotY, sData); break;	// 向きY
			case 7: pFile->ToValue(nTime, sData); break;	// 行動カウント
			}
		}

		// 最大数に達したら返す
		if (nRow == nRowMax - 1)	// (列数 - 列の最大数 - ヘッダーの列数)
		{
			delete pFile;
			pFile = NULL;
			return;
		}

		if (nStage == m_nStage)
		{
			CEnemy *pObj = CEnemy::Create(nType, nMove, pos, D3DXVECTOR3(0.0f, fRotY, 0.0f), nTime);
		}
	}

	delete pFile;
	pFile = NULL;
}