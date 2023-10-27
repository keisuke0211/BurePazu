//========================================
// 
// タイトル
// 
//========================================
// *** title.cpp ***
//========================================
#include "title.h"
#include "game.h"
#include "ranking.h"
#include "../system/sound.h"
#include "../object/UI/UI_title.h"
#include "../object\model\model.h"
#include "../system/input.h"
#include "fade.h"
#include "../object/model/block.h"

#include "../system/words/text.h"
#include "../system/words/font.h"
#include "../system/words/words.h"

// 定義
const char* CTitle::TEXT_FILE_PATH = "data\\GAMEDATA\\TEXT\\TITLE_DATA.txt";

// 静的変数
bool CTitle::m_bStart = false;
bool CTitle::m_bClear = false;

//========================================
// コンストラクタ
//========================================
CTitle::CTitle()
{
	m_bStart = false;
	m_bClear = false;

	m_nTextTime = 0;
	m_nStandTime = 0;
	m_nSelectMenu = 0;

	Title = TITLE_OUTSET;

	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
	{
		m_Words[nCnt] = NULL;
		m_WordsShadow[nCnt] = NULL;
		m_bMove[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_Menu[nCnt] = NULL;
	}

}

//========================================
// デストラクタ
//========================================
CTitle::~CTitle()
{
	/*CBlock::Reset();*/
}

//========================================
// 初期化
//========================================
HRESULT CTitle::Init(void)
{
	m_bStart = false;
	m_bClear = false;

	CUiTitle::Create();		// タイトル背景

	// テキスト読み込み
	TextLoad();

	{
		FormFont pFont = {INIT_D3DXCOLOR,20.0f,50,1,-1};

		m_WordsShadow[0] = CWords::Create("ブ", D3DXVECTOR3(443.0f, -57.0f, 0.0f), D3DXVECTOR3(52.0f, 52.0f, 0.0f), CFont::FONT_BESTTEN, D3DXCOLOR(0, 0, 0, 1));
		m_WordsShadow[1] = CWords::Create("レ", D3DXVECTOR3(563.0f, -57.0f, 0.0f), D3DXVECTOR3(52.0f, 52.0f, 0.0f), CFont::FONT_BESTTEN, D3DXCOLOR(0, 0, 0, 1));
		m_WordsShadow[2] = CWords::Create("パ", D3DXVECTOR3(683.0f, -57.0f, 0.0f), D3DXVECTOR3(52.0f, 52.0f, 0.0f), CFont::FONT_BESTTEN, D3DXCOLOR(0, 0, 0, 1));
		m_WordsShadow[3] = CWords::Create("ズ", D3DXVECTOR3(803.0f, -57.0f, 0.0f), D3DXVECTOR3(52.0f, 52.0f, 0.0f), CFont::FONT_BESTTEN, D3DXCOLOR(0, 0, 0, 1));

		m_Words[0] = CWords::Create("ブ", D3DXVECTOR3(440.0f, -60.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), CFont::FONT_BESTTEN, INIT_D3DXCOLOR);
		m_Words[1] = CWords::Create("レ", D3DXVECTOR3(560.0f, -60.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), CFont::FONT_BESTTEN, INIT_D3DXCOLOR);
		m_Words[2] = CWords::Create("パ", D3DXVECTOR3(680.0f, -60.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), CFont::FONT_BESTTEN, INIT_D3DXCOLOR);
		m_Words[3] = CWords::Create("ズ", D3DXVECTOR3(800.0f, -60.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), CFont::FONT_BESTTEN, INIT_D3DXCOLOR);
	}
	m_bMove[0] = true;

	FormFont pFont = {
		INIT_D3DXCOLOR,
		20.0f,
		1,
		1,
		-1
	};
	m_Menu[0] = CText::Create(CText::BOX_NORMAL_RECT,
		D3DXVECTOR3(640.0f, 600.0f, 0.0f),
		D3DXVECTOR2(1080.0f, 100.0f),
		" ",
		CFont::FONT_BESTTEN,
		&pFont);

	pFont = {
		INIT_D3DXCOLOR,
		20.0f,
		5,
		10,
		-1
	};

	m_Menu[1] = CText::Create(CText::BOX_NORMAL_RECT,
		D3DXVECTOR3(640.0f, 600.0f, 0.0f),
		D3DXVECTOR2(1080.0f, 100.0f),
		"ENTERかAボタンを押して始めてね!",
		CFont::FONT_BESTTEN,
		&pFont,false);

	m_nTextTime = TEXT_TIME;
	m_nStandTime = STAND_MAX;

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::TYPE_TITLE);

	return S_OK;
}

//========================================
// 終了
//========================================
void CTitle::Uninit(void)
{
	CSound *pSound = CManager::GetSound();
	pSound->StopSound();

	CObject::ReleaseAll(CObject::TYPE_BG);
	CObject::ReleaseAll(CObject::TYPE_BLOCK);
	CObject::ReleaseAll(CObject::TYPE_FONT);
}

//========================================
// 更新
//========================================
void CTitle::Update(void)
{
	// タイトルアニメーション
	if (Title == TITLE_OUTSET)
	{
		TitleAnime();
	}
	else if (Title == TITLE_MENU)
	{
		Menu();
	}

	// --- 取得 ---------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CMouse *pInputMouse = CManager::GetInputMouse();			// マウス
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット
	CSound *pSound = CManager::GetSound();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_A))
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			switch (Title)
			{
			case TITLE_OUTSET:
			{
				TextClear(WORDS_MAX, 2, TITLE_MENU);

				// メニュー生成
				MenuCreate();
			}
			break;
			case TITLE_MENU:
			{
				pSound->PlaySound(CSound::TYPE_CYBER);

				switch (m_nSelectMenu)
				{
				case MENU_GAME:
					TextClear(1, MENU_MAX, TITLE_NEXT);
					CGame::Reset();
					CGame::LoodStage();
					CManager::GetFade()->SetFade(MODE_GAME);
					break;
				case MENU_TUTORIAL:
					TextClear(1, MENU_MAX, TITLE_NEXT);
					CManager::GetFade()->SetFade(MODE_TUTORIAL);
					break;
				case MENU_RANKING:
					TextClear(1, MENU_MAX, TITLE_NEXT);
					CManager::GetFade()->SetFade(MODE_RANKING);
					CRanking::SetScore11(0);
					CRanking::SetAllStage(true);
					break;
				case MENU_END:
					//ゲームの終了
					PostQuitMessage(0);
					break;
				}
			}
			break;
			}
		}
	}
}

//========================================
// 描画
//========================================
void CTitle::Draw(void)
{

}

//========================================
// 生成
//========================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = new CTitle;

	pTitle->Init();

	return pTitle;
}

//========================================
// タイトルアニメーション
//========================================
void CTitle::TitleAnime(void)
{
	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
	{
		if (m_Words[nCnt] != NULL)
		{
			D3DXVECTOR3 pos = m_Words[nCnt]->GetPos();

			if (pos.y <= 110.0f && m_bMove[nCnt])
			{
				D3DXVECTOR3 move;

				move.y = 3.0f;

				m_Words[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
				m_WordsShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));

				if (pos.y >= 100.0f)
				{
					move.y = 0.0f;

					m_Words[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
					m_WordsShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
				}
				else if (pos.y >= 20 && nCnt != 3 && !m_bMove[nCnt + 1])
				{
					m_bMove[nCnt + 1] = true;
				}
			}
		}
	}

	// 入力催促テキスト
	if (--m_nTextTime <= 0)
	{
		if (m_nStandTime == STAND_MAX)
		{
			if (m_Menu[1] != NULL)
			{
				m_Menu[1]->Uninit();

				m_Menu[1] = NULL;
			}
		}
		m_nStandTime--;
		if (m_nStandTime <= 0)
		{
			FormFont pFont = {
				INIT_D3DXCOLOR,
				20.0f,
				5,
				10,
				-1
			};

			m_Menu[1] = CText::Create(CText::BOX_NORMAL_RECT,
				D3DXVECTOR3(640.0f, 600.0f, 0.0f),
				D3DXVECTOR2(1080.0f, 100.0f),
				"ENTERかAボタンを押して始めてね!",
				CFont::FONT_BESTTEN,
				&pFont, false);

			m_nTextTime = TEXT_TIME;
			m_nStandTime = STAND_MAX;
		}
	}
}

//========================================
// メニュー生成
//========================================
void CTitle::MenuCreate(void)
{
	FormFont pFont = {
		INIT_D3DXCOLOR,
		20.0f,
		5,
		10,
		-1
	};

	{
		m_Menu[0] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(640.0f, 150.0f, 0.0f),
			D3DXVECTOR2(360.0f, 100.0f),
			"ゲーム",
			CFont::FONT_BESTTEN,
			&pFont);
	}

	{
		m_Menu[1] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(640.0f, 300.0f, 0.0f),
			D3DXVECTOR2(360.0f, 100.0f),
			"チュートリアル",
			CFont::FONT_BESTTEN,
			&pFont);
	}
	{
		m_Menu[2] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(640.0f, 450.0f, 0.0f),
			D3DXVECTOR2(360.0f, 100.0f),
			"ランキング",
			CFont::FONT_BESTTEN,
			&pFont);
	}
	{
		m_Menu[3] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(640.0f, 600.0f, 0.0f),
			D3DXVECTOR2(360.0f, 100.0f),
			"ゲームをやめる",
			CFont::FONT_BESTTEN,
			&pFont);
	}
}

//========================================
// メニュー
//========================================
void CTitle::Menu(void)
{
	// 色
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_Menu[nCnt] != NULL)
		{
			if (nCnt == m_nSelectMenu)
			{
				m_Menu[nCnt]->SetBoxColor(D3DXCOLOR(0.0f,1.0f,0.0f,1.0f));
			}
			else
			{
				m_Menu[nCnt]->SetBoxColor(INIT_D3DXCOLOR);
			}
		}
	}


	// --- 取得 ---------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット
	CSound *pSound = CManager::GetSound();

	// -- メニュー選択 ---------------------------
	if (pInputKeyboard->GetTrigger(DIK_W) || pInputKeyboard->GetTrigger(DIK_UP) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_UP) || pInputJoypad->GetStick(0).aAngleTrigger[CJoypad::STICK_TYPE_LEFT][CJoypad::STICK_ANGLE_UP])
	{
		pSound->PlaySound(CSound::TYPE_SELECT);
		m_nSelectMenu--;
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) || pInputKeyboard->GetTrigger(DIK_DOWN) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_DOWN) || pInputJoypad->GetStick(0).aAngleTrigger[CJoypad::STICK_TYPE_LEFT][CJoypad::STICK_ANGLE_DOWN])
	{
		pSound->PlaySound(CSound::TYPE_SELECT);
		m_nSelectMenu++;
	}

	// ループ制御
	IntLoopControl(&m_nSelectMenu, MENU_MAX, 0);
}

//========================================
// テキスト削除
//========================================
void CTitle::TextClear(int nWords, int nText, TITLE aTitle)
{
	for (int nCnt = 0; nCnt < nWords; nCnt++)
	{
		if (m_WordsShadow[nCnt] != NULL)
		{
			m_WordsShadow[nCnt]->Uninit();
			m_WordsShadow[nCnt] = NULL;
		}
		if (m_Words[nCnt] != NULL)
		{
			m_Words[nCnt]->Uninit();
			m_Words[nCnt] = NULL;
		}
	}
	for (int nCnt = 0; nCnt < nText; nCnt++)
	{
		if (m_Menu[nCnt] != NULL)
		{
			m_Menu[nCnt]->Uninit();
			m_Menu[nCnt] = NULL;
		}
	}

	Title = aTitle;
}

//========================================
// テキスト読み込み
//========================================
void CTitle::TextLoad(void)
{

}