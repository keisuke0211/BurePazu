//========================================
// 
// ランキング
// 
//========================================
// *** ranking.h ***
//========================================
#include "ranking.h"
#include "game.h"
#include "fade.h"
#include "../system/sound.h"
#include "../object/UI/UI_title.h"
#include "../system/csv_file.h"
#include "../system/input.h"
#include "../system/words/text.h"
#include "../system/words/font.h"

// 定義
const char* CRanking::STAGE_FILE = "data\\SAVEDATA\\RANKING_DATA.csv";
const char* CRanking::TEXT_FILE_PATH = "data\\GAMEDATA\\TEXT\\WORDS_DATA.txt";
int CRanking::m_nGameScore = 0;
bool CRanking::m_bSetScore = false;
bool CRanking::m_bRankingAll = false;

//========================================
// コンストラクタ
//========================================
CRanking::CRanking()
{
	// リセット
	CGame::Reset();

	for (int nRank = 0; nRank < RANK_NUM; nRank++)
	{
		m_Ranking[nRank].nScore = 100 + (100 * nRank);
		*m_Ranking[nRank].aName = NULL;
	}

	m_Info.nCounterState = 0;
	m_Info.bNameEntry = false;
	m_Info.nCntName = 0;
	m_Info.nUpdateRank = -1;
	m_Info.nCntBlink = 0;
	m_Info.bNameInput = true;
	m_Info.nCntChar = 0;

	m_Info.nCntString = 0;
	m_Info.nCntLetter = 0;
	m_pString = NULL;
	m_Info.nCntBlink = 0;
	nStringMax = 0;

	strinit(m_aNameData, TXT_MAX);

	for (int nCnt = 0; nCnt < EXPLAIN_MAX; nCnt++)
	{
		m_Explain[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < TEXT_MAX; nCnt++)
	{
		m_Words[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < NAME_NUM; nCnt++)
	{
		m_Info.nNameChar[nCnt] = 0;
	}
}

//========================================
// デストラクタ
//========================================
CRanking::~CRanking()
{

}

//========================================
// 初期化
//========================================
HRESULT CRanking::Init(void)
{
	int nUpdateRank = -1;	// スコアを更新した順位

	CUiTitle::Create();		// タイトル背景

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::TYPE_RANKING);

	// 読み込み
	Load();			// プレイしたステージだけ

	WordsLoad();	// テキスト

	// ソート
	SortDesc(&nUpdateRank);

	FormFont pFont = { INIT_D3DXCOLOR,20.0f,1,1,-1 };
	FormShadow pShadow = { INIT_D3DXCOLOR, true, D3DXVECTOR3(1.0f,1.0f,0.0f), D3DXVECTOR2(1.0f,1.0f) };

	// テキスト生成
	for (int nRank = 0; nRank < RANK_NUM; nRank++)
	{
		switch (nRank)
		{
		case 0:
			pFont = { D3DXCOLOR(1.0f,0.96f,0,1)	, 20.0f, 1, 1, -1 };
			break;
		case 1:
			pFont = { D3DXCOLOR(0.6f,0.6f,0.6f,1), 20.0f, 1, 1, -1 };
			break;
		case 2:
			pFont = { D3DXCOLOR(0.78f,0.54f,0,1), 20.0f, 1, 1, -1 };
			break;
		default:
			pFont = { INIT_D3DXCOLOR, 20.0f, 1, 1, -1 };
			break;
		}

		char aString[TXT_MAX];
		sprintf(aString, " %s %-5s %6d", GetRankText(nRank), m_Ranking[nRank].aName, m_Ranking[nRank].nScore);

		m_Text[nRank] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(10.0f, 155 + ( 40 * nRank), 0.0f),
			D3DXVECTOR2(0.0f, 100.0f),
			aString,
			CFont::FONT_BESTTEN,
			&pFont,false,&pShadow);
		m_Text[nRank]->SetSpace(true);
	}

	// 枠(ランキングフレーム)
	CText::Create(CText::BOX_NORMAL_SQR, D3DXVECTOR3(400.0f, 290, 0.0f), D3DXVECTOR2(740.0f, 400.0f),
		"", CFont::FONT_BESTTEN, &pFont, true);

	int nRank = SetScore(m_nGameScore);

	if (nRank < RANK_NUM && nRank > -1)
	{
		m_bSetScore = true;
	}
	else
	{
		m_bSetScore = false;
	}

	// スコア更新したら
	if (m_bSetScore)
	{
		// 操作テキストの説明
		{
			pFont = {INIT_D3DXCOLOR,15.0f,1,1,-1};
			FormShadow pShadow = {D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(2.0f,2.0f,0.0f), D3DXVECTOR2(1.0f,1.0f)};

			m_Explain[0] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(50.0f, 550, 0.0f),D3DXVECTOR2(0.0f, 0.0f),
				"↑・→：行の変更",CFont::FONT_BESTTEN,&pFont, false,&pShadow);

			m_Explain[1] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(50.0f, 590, 0.0f),D3DXVECTOR2(0.0f, 0.0f),
				"←・→：文字の変更",CFont::FONT_BESTTEN,&pFont, false, &pShadow);

			m_Explain[2] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(50.0f, 630, 0.0f),D3DXVECTOR2(0.0f, 0.0f),
				"X・Y：変換",CFont::FONT_BESTTEN,&pFont, false, &pShadow);

			m_Explain[3] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(640.0f, 550, 0.0f),D3DXVECTOR2(0.0f, 100.0f),
				"A：決定",CFont::FONT_BESTTEN,&pFont, false, &pShadow);

			m_Explain[4] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(640.0f, 590, 0.0f),D3DXVECTOR2(0.0f, 100.0f),
				"B：削除",CFont::FONT_BESTTEN,&pFont, false, &pShadow);

			m_Explain[5] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(640.0f, 630, 0.0f),D3DXVECTOR2(0.0f, 100.0f),
				"途中で入力をやめる場合は※を選択!",CFont::FONT_BESTTEN,&pFont, false, &pShadow);
		}

		SetNameEntry(nRank);
	}

	// タイトルから来たら
	if (m_bRankingAll)
	{
		/*pFont = { INIT_D3DXCOLOR,15.0f,1,1,-1 };
		FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(2.0f,2.0f,0.0f), D3DXVECTOR2(1.0f,1.0f) };

		m_Explain[0] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(50.0f, 590, 0.0f),
			D3DXVECTOR2(0.0f, 0.0f),
			"←・→：ステージの変更",
			CFont::FONT_BESTTEN,
			&pFont, false,&pShadow);

		m_Explain[1] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(640.0f, 590, 0.0f),
			D3DXVECTOR2(0.0f, 0.0f),
			"現在のステージ：STAGE1",
			CFont::FONT_BESTTEN,
			&pFont, false,&pShadow);*/
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CRanking::Uninit(void)
{
	CSound *pSound = CManager::GetSound();
	pSound->StopSound();

	// 書き出し処理
	if (!m_bRankingAll)
	{
		Save();
	}

	for (int nStr = 0; nStr < nStringMax; nStr++)
	{
		for (int nCpnv = 0; nCpnv < m_pString[nStr].nConvMax; nCpnv++)
		{
			if (m_pString[nStr].pConv[nCpnv].pLetter != NULL)
			{
				delete[] m_pString[nStr].pConv[nCpnv].pLetter;
				m_pString[nStr].pConv[nCpnv].pLetter = NULL;
			}
		}
		if (m_pString[nStr].pConv != NULL)
		{
			delete[] m_pString[nStr].pConv;
			m_pString[nStr].pConv = NULL;
		}
	}

	if (m_pString != NULL)
	{
		delete[] m_pString;
		m_pString = NULL;
	}

	CObject::ReleaseAll(CObject::TYPE_BG);
	CObject::ReleaseAll(CObject::TYPE_FONT);
}

//========================================
// 更新
//========================================
void CRanking::Update(void)
{
	// -- 取得 -------------------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット

	// 状態
	State();

	// 名前入力
	NameEntry();

	// ランキング切替
	if (m_bRankingAll)
	{
		RankingSwitch();
	}

	if ((pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_A)) && m_Info.bNameEntry == false)
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(MODE_TITLE);
		}
	}
}

//========================================
// 描画
//========================================
void CRanking::Draw(void)
{

}

//========================================
// 生成
//========================================
CRanking *CRanking::Create(void)
{
	CRanking *pRanking = new CRanking;

	pRanking->Init();

	return pRanking;
}

//========================================
// プレイしたステージのみ読み込み
//========================================
void CRanking::Load(void)
{
	CSVFILE *pFile = new CSVFILE;

	pFile->FileLood(STAGE_FILE, true, true, ',');

	// 行数の取得
	int nRowMax = pFile->GetRowSize();

	for (int nRow = 0; nRow < nRowMax; nRow++)
	{

		// 列数の取得
		int nLineMax = pFile->GetLineSize(nRow);

		for (int nLine = 0; nLine < nLineMax; nLine++)
		{
			string sData = pFile->GetData(nRow, nLine);

			switch (nLine)
			{
			case 0:
				char *pNama;
				pFile->ToValue(pNama, sData);

				strcat(m_Ranking[nRow].aName,pNama);
				break;	// 名前
			case 1:	pFile->ToValue(m_Ranking[nRow].nScore, sData);		break;	// スコア
			}
		}

		// 最大数に達したら返す
		if (nRow == nRowMax - 1)	// (列数 - 列の最大数 - ヘッダーの列数)
		{
			return;
		}
	}

	delete pFile;
	pFile = NULL;
}

//========================================
// 書き出し
//========================================
void CRanking::Save(void)
{
	CSVFILE *pFile = new CSVFILE;

	// ヘッダーの設定
	pFile->SetHeader("名前,スコア,", ',');

	// データの設定
	for (int nRow = 0; nRow < RANK_NUM; nRow++)
	{
		// インデックスの設定
		pFile->SetIndex(GetRankText(nRow), ',');

		for (int nLine = 0; nLine < 2; nLine++)
		{
			string sData;	// 文字列の格納する変数

			// 文字列に変換
			switch (nLine)
			{
			case 0:	sData = m_Ranking[nRow].aName; break;	// 名前
			case 1:	sData = pFile->CastCell(m_Ranking[nRow].nScore); break;	// スコア
			}

			// データの追加
			pFile->SetCell(sData, nRow, nLine);
		}
	}

	// 書き出し
	pFile->FileSave(STAGE_FILE, ',');
}

//========================================
// 状態処理
//========================================
void CRanking::State(void)
{
	for (int nCntRanking = 0; nCntRanking < RANK_NUM; nCntRanking++)
	{
		if (m_Info.nUpdateRank == nCntRanking)
		{
			m_Text[nCntRanking]->SetTextColor(RANKING_COLOR);
		}
	}
}

//========================================
// スコア設定
//========================================
int CRanking::SetScore(int nScore)
{
	int nUpdateRank = -1;	// スコアを更新した順位

	// スコアを降順にソートする
	SortDesc(&nUpdateRank);

	if (nScore > m_Ranking[RANK_NUM - 1].nScore)
	{// 最も小さい値より挿入する値が大きかった時
	 // 最も小さい値に挿入する値を代入する
		m_Ranking[RANK_NUM - 1].nScore = nScore;

		// スコアを更新した順位を代入する
		nUpdateRank = RANK_NUM - 1;
	}

	// スコアを降順にソートする
	SortDesc(&nUpdateRank);

	// スコアを更新した順位を返す
	return nUpdateRank;
}

//========================================
// スコア設定
//========================================
void CRanking::SetScore11(int nScore)
{
	m_nGameScore = nScore;
}

//========================================
// 名前入力設定
//========================================
void CRanking::SetNameEntry(int nUpdateRank)
{
	m_Info.nUpdateRank = nUpdateRank;

	if (m_Info.nUpdateRank != -1)
	{// 更新順位が-1(更新無し)でない時、
		m_Info.bNameEntry = true;
		m_bSetScore = true;
		m_Info.nCntName = 0;
		m_Info.nCntBlink = 0;

		// 更新した順位の名前を初期化
		strinit(m_Ranking[nUpdateRank].aName, TXT_MAX);

		strcat(m_aNameData, "_____");

		char aString[TXT_MAX];
		sprintf(aString, " %s %-5s %6d", GetRankText(m_Info.nUpdateRank), m_aNameData, m_Ranking[m_Info.nUpdateRank].nScore);

		m_Text[m_Info.nUpdateRank]->Uninit();

		FormFont pFont = {
			INIT_D3DXCOLOR,
			20.0f,
			1,
			1,
			-1
		};

		m_Text[m_Info.nUpdateRank] = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(10.0f, 155 + (40 * m_Info.nUpdateRank), 0.0f),
			D3DXVECTOR2(0.0f, 100.0f),
			aString,
			CFont::FONT_BESTTEN,
			&pFont, false);
	}
	else
	{// 更新処理が-1(更新無し)の時、
		m_Info.bNameEntry = false;
	}

	SetWords();
}

//========================================
// 名前入力
//========================================
void CRanking::NameEntry(void)
{
	if (!m_Info.bNameEntry)
	{
		return;
	}
	if (!m_Info.bNameInput)
	{
		++m_Info.nCounterBlink %= (BLINK_TIME * 2);	// 点滅カウンターを加算制御

		D3DXCOLOR col =
			!(m_Info.nCounterBlink / BLINK_TIME) ?
			RANKING_COLOR : BLINK_COLOR;

		int nRank = m_Info.nUpdateRank;

		if (m_Text[nRank]->ChgWords("_", NAME_START_DEX + m_Info.nCntName, col))
		{
			UpdateWords();
		}

		// 操作
		NameInput();
	}
	
	if (m_Info.bNameInput)
	{
		int nString = m_Info.nCntString;
		int nLetter = m_Info.nCntLetter;
		int nConv = m_Info.nCntConv;
		int nRank = m_Info.nUpdateRank;

		UpdateWords();
		m_Info.bNameInput = false;
	}


}

//========================================
// 名前入力(操作)
//========================================
void CRanking::NameInput(void)
{
	// -- 取得 -------------------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CJoypad *pInputJoypad = CManager::GetInputJoypad();			// ジョイパット
	CSound *pSound = CManager::GetSound();

	// -- 文字列切替 ---------------------------------------------
	if (pInputKeyboard->GetRepeat(DIK_A) || pInputKeyboard->GetRepeat(DIK_LEFT) ||
		pInputJoypad->GetRepeat(CJoypad::JOYKEY_LEFT) || pInputJoypad->GetStick().aAngleRepeat[CJoypad::STICK_TYPE_LEFT][CJoypad::STICK_ANGLE_LEFT])
	{
		m_Info.nCntLetter--;
		m_Info.bNameInput = true;
		pSound->PlaySound(CSound::TYPE_SELECT);
	}
	else if (
		pInputKeyboard->GetRepeat(DIK_D) || pInputKeyboard->GetRepeat(DIK_RIGHT) ||
		pInputJoypad->GetRepeat(CJoypad::JOYKEY_RIGHT) || pInputJoypad->GetStick().aAngleRepeat[CJoypad::STICK_TYPE_LEFT][CJoypad::STICK_ANGLE_RIGHT])
	{
		m_Info.nCntLetter++;
		m_Info.bNameInput = true;
		pSound->PlaySound(CSound::TYPE_SELECT);
	}

	// -- 文字切替 ---------------------------------------------
	if (pInputKeyboard->GetRepeat(DIK_S) || pInputKeyboard->GetRepeat(DIK_DOWN) ||
		pInputJoypad->GetRepeat(CJoypad::JOYKEY_DOWN) || pInputJoypad->GetStick().aAngleRepeat[CJoypad::STICK_TYPE_LEFT][CJoypad::STICK_ANGLE_DOWN])
	{
		m_Info.nCntLetter = 0;
		m_Info.nCntConv = 0;
		m_Info.nCntString++;
		m_Info.bNameInput = true;
		pSound->PlaySound(CSound::TYPE_SELECT);
	}
	else if (
		pInputKeyboard->GetRepeat(DIK_W) || pInputKeyboard->GetRepeat(DIK_UP) ||
		pInputJoypad->GetRepeat(CJoypad::JOYKEY_UP) || pInputJoypad->GetStick().aAngleRepeat[CJoypad::STICK_TYPE_LEFT][CJoypad::STICK_ANGLE_UP])
	{
		m_Info.nCntLetter = 0;
		m_Info.nCntConv = 0;
		m_Info.nCntString--;
		m_Info.bNameInput = true;
		pSound->PlaySound(CSound::TYPE_SELECT);
	}

	// -- 変換 ---------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_Q) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_X))
	{
		m_Info.nCntConv--;
		m_Info.bNameInput = true;
	}
	else if (pInputKeyboard->GetTrigger(DIK_E) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_Y))
	{
		m_Info.nCntConv++;
		m_Info.bNameInput = true;
	}

	// カウント制御
	{
		int nString = m_Info.nCntString;
		int nLetterMax = m_pString[nString].nLettreMax;
		int nConvMax = m_pString[nString].nConvMax;

		IntLoopControl(&m_Info.nCntString, nStringMax, 0);
		IntLoopControl(&m_Info.nCntLetter, nLetterMax, 0);
		IntLoopControl(&m_Info.nCntConv, nConvMax, 0);
	}

	// -- 決定・削除 ---------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_B))
	{// 削除
		if (m_Info.nCntName > 0)
		{
			int nRank = m_Info.nUpdateRank;
			char aWords[] = "_";

			int nChar = m_Info.nNameChar[m_Info.nCntName - 1];

			// 現在のカウントの文字を反映する
			for (int nCnt = 0; nCnt < nChar; nCnt++)// 日本語用
			{
				m_Ranking[nRank].aName[(m_Info.nCntChar - 1) - nCnt] = '\0';
			}
			m_Info.nCntChar -= nChar;

			if (m_Text[nRank]->ChgWords(aWords, NAME_START_DEX + m_Info.nCntName,INIT_D3DXCOLOR))
			{
				m_Info.bNameInput = false;
			}
			m_Info.nCntName--;

		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CJoypad::JOYKEY_A))
	{// 決定

		int nString = m_Info.nCntString;
		int nLetter = m_Info.nCntLetter;
		int nLetterMax = m_pString[nString].nLettreMax - 1;
		int nConv = m_Info.nCntConv;
		int nRank = m_Info.nUpdateRank;

		if (nLetter != nLetterMax)
		{
			// 現在のカウントの文字を加える
			strcat(m_Ranking[nRank].aName, m_pString[nString].pConv[nConv].pLetter[nLetter].aConv);

			// 現在のカウントの文字を反映する
			strinit(m_aNameData, TXT_MAX);
			strcat(m_aNameData, m_Ranking[nRank].aName);

			int nStrlen = strlen(m_pString[nString].pConv[nConv].pLetter[nLetter].aConv);

			m_Info.nCntChar += nStrlen;
			m_Info.nNameChar[m_Info.nCntName] = nStrlen;

			if (m_Text[nRank]->ChgWords(m_pString[nString].pConv[nConv].pLetter[nLetter].aConv, NAME_START_DEX + m_Info.nCntName, RANKING_COLOR))
			{
				UpdateWords();
			}

			if (++m_Info.nCntName >= NAME_NUM)
			{// 名前入力のカウントを加算した結果、ランキング名の文字数に達した時、
				m_Info.bNameEntry = false;
				return;
			}
		}
		else
		{
			// 現在のカウントの文字が終端文字だったら
			if (m_Info.nCntName != 0)
			{
				for (int nCnt = 0; nCnt < NAME_NUM - m_Info.nCntName; nCnt++)
				{
					// 現在のカウントの文字を加える
					strcat(m_Ranking[nRank].aName, " ");

					// 現在のカウントの文字を反映する
					strinit(m_aNameData, TXT_MAX);
					strcat(m_aNameData, m_Ranking[nRank].aName);

					if (m_Text[nRank]->ChgWords(" ", NAME_START_DEX + (m_Info.nCntName + nCnt), RANKING_COLOR))
					{
						m_Info.bNameInput = false;
					}
				}
				m_Info.bNameEntry = false;
			}
			else
			{
				int nCntName = 0;
				for (int nCnt = 0; nCnt < NAME_NUM - m_Info.nCntName; nCnt++)
				{
					char aData[6][TXT_MAX] = { "名","無","し"," "," "};

					// 現在のカウントの文字を加える
					strcat(m_Ranking[nRank].aName,aData[nCntName]);

					// 現在のカウントの文字を反映する
					strinit(m_aNameData, TXT_MAX);
					strcat(m_aNameData, m_Ranking[nRank].aName);

					if (m_Text[nRank]->ChgWords(aData[nCntName], NAME_START_DEX + (m_Info.nCntName + nCnt), RANKING_COLOR))
					{
						m_Info.bNameInput = false;
					}
					nCntName++;
				}
				m_Info.bNameEntry = false;
			}
		}

		pSound->PlaySound(CSound::TYPE_CYBER);

	}
}

//========================================
// ランキング切替
//========================================
void CRanking::RankingSwitch(void)
{
	
}

//========================================
// 文字表示(設定)
//========================================
void CRanking::SetWords(void)
{
	int nString = m_Info.nCntString;
	int nLetter = m_Info.nCntLetter;
	int nLetterMax = m_pString[nString].nLettreMax - 1;
	int nConv = m_Info.nCntConv;
	int nConvMax = m_pString[nString].nConvMax - 1;
	int nRank = m_Info.nUpdateRank;

	int nNextLetter = m_Info.nCntLetter + 1;				// 次の文字
	int nPrevLetter = m_pString[nString].nLettreMax - 1;	// 前の文字
	int nNextString = m_Info.nCntString + 1;				// 次の行
	int nPrevString = nStringMax - 1;						// 前の行

	FormFont pFont = {INIT_D3DXCOLOR,30.0f,1,1,-1};

	// 中央
	m_Words[0] = CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(950.0f, 250, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		m_pString[nString].pConv[nConv].pLetter[nLetter].aConv, CFont::FONT_BESTTEN, &pFont, false);

	// 上
	m_Words[1] = CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(950.0f, 160, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		m_pString[nPrevString].pConv[nConv].pLetter[0].aConv, CFont::FONT_BESTTEN, &pFont, false);

	// 下
	m_Words[2] = CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(950.0f, 340, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		m_pString[nNextString].pConv[nConv].pLetter[0].aConv, CFont::FONT_BESTTEN, &pFont, false);

	// 左
	m_Words[3] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(850.0f, 250, 0.0f),D3DXVECTOR2(0.0f, 0.0f),
		m_pString[nString].pConv[nConv].pLetter[nPrevLetter].aConv,CFont::FONT_BESTTEN,&pFont, false);

	// 右
	m_Words[4] = CText::Create(CText::BOX_NORMAL_RECT,D3DXVECTOR3(1050.0f, 250, 0.0f),D3DXVECTOR2(0.0f, 0.0f),
		m_pString[nString].pConv[nConv].pLetter[nNextLetter].aConv,CFont::FONT_BESTTEN,&pFont, false);

	pFont = {INIT_D3DXCOLOR,15.0f,1,1,-1};

	// 上
	m_Words[5] = CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(975.0f, 210, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		"↑", CFont::FONT_BESTTEN, &pFont, false);

	// 下
	m_Words[6] = CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(975.0f, 300, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		"↓", CFont::FONT_BESTTEN, &pFont, false);

	// 左
	m_Words[7] = CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(930.0f, 250, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		"←", CFont::FONT_BESTTEN, &pFont, false);

	// 右
	m_Words[8] = CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(1030.0f, 250, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		"→", CFont::FONT_BESTTEN, &pFont, false);

	// 枠(文字)
	m_Words[9] = CText::Create(CText::BOX_NORMAL_SQR, D3DXVECTOR3(1020.0f, 260, 0.0f), D3DXVECTOR2(320.0f, 320.0f),
		"", CFont::FONT_BESTTEN, &pFont, true);
}

//========================================
// 文字表示(更新)
//========================================
void CRanking::UpdateWords(void)
{
	int nString = m_Info.nCntString;
	int nLetter = m_Info.nCntLetter;
	int nLetterMax = m_pString[nString].nLettreMax - 1;
	int nConv = m_Info.nCntConv;
	int nConvMax = m_pString[nString].nConvMax - 1;
	int nRank = m_Info.nUpdateRank;

	int nNextLetter = m_Info.nCntLetter + 1;	// 次の文字
	int nPrevLetter = m_Info.nCntLetter - 1;	// 前の文字
	int nNextString = m_Info.nCntString + 1;	// 次の行
	int nPrevString = m_Info.nCntString - 1;	// 前の行

	IntLoopControl(&nNextString, nStringMax, 0);
	IntLoopControl(&nPrevString, nStringMax, 0);
	IntLoopControl(&nNextLetter, nLetterMax+1, 0);
	IntLoopControl(&nPrevLetter, nLetterMax+1, 0);

	m_Words[0]->ChgWords(m_pString[nString].pConv[nConv].pLetter[nLetter].aConv, 0, INIT_D3DXCOLOR);
	m_Words[1]->ChgWords(m_pString[nPrevString].pConv[0].pLetter[0].aConv, 0, INIT_D3DXCOLOR);
	m_Words[2]->ChgWords(m_pString[nNextString].pConv[0].pLetter[0].aConv, 0, INIT_D3DXCOLOR);
	m_Words[3]->ChgWords(m_pString[nString].pConv[nConv].pLetter[nPrevLetter].aConv, 0, INIT_D3DXCOLOR);
	m_Words[4]->ChgWords(m_pString[nString].pConv[nConv].pLetter[nNextLetter].aConv, 0, INIT_D3DXCOLOR);

}

//========================================
// ソート降順
//========================================
void CRanking::SortDesc(int *nUpdateRank)
{
	int	 nTemp;					// 一時的に値を格納
	char aNameTemp[TXT_MAX];	// 一時的に値を格納
	bool bUpdateRank = false;	// 順位更新フラグ

	for (int nCnt1 = 0; nCnt1 < RANK_NUM - 1; nCnt1++)
	{
		for (int nCnt2 = nCnt1 + 1; nCnt2 < RANK_NUM; nCnt2++)
		{
			if (m_Ranking[nCnt2].nScore > m_Ranking[nCnt1].nScore)
			{// 要素2が要素1の値を上回っていた時、
			 // 値を入れ替える
				nTemp = m_Ranking[nCnt1].nScore;
				m_Ranking[nCnt1].nScore = m_Ranking[nCnt2].nScore;
				m_Ranking[nCnt2].nScore = nTemp;

				// 名前を入れ替える
				sprintf(aNameTemp, "%s", m_Ranking[nCnt1].aName);
				sprintf(m_Ranking[nCnt1].aName, "%s", m_Ranking[nCnt2].aName);
				sprintf(m_Ranking[nCnt2].aName, "%s", aNameTemp);

				if (bUpdateRank == false)
				{// 順位更新フラグが偽の時、
				 // 順位更新フラグを真にする
					bUpdateRank = true;

					// スコアを更新した順位を代入する
					*nUpdateRank = nCnt1;
				}
			}
		}
	}
}

//========================================
// 空白埋め
//========================================
void CRanking::strinit(char *pData, int nNum)
{
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		pData[nCnt] = '\0';
	}
}

//========================================
// 文字読み込み
//========================================
void CRanking::WordsLoad(void)
{
	// 変数宣言
	char aDataSearch[128] = {};		// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数

	// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(TEXT_FILE_PATH, "r");

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
			else if (!strcmp(aDataSearch, "NUM_ROW"))
			{// 行数
				fscanf(pFile, "%s", &g_aEqual[0]);
				fscanf(pFile, "%d", &nStringMax);

				// メモリの解放
				m_pString = new String[nStringMax];
			}
			else if (!strcmp(aDataSearch, "SET_TEXT"))
			{// 文字列の読み込み

				int nCntString = 0;	// 列

				while (1)
				{
					fscanf(pFile, "%s", aDataSearch);	// 検索

					if (!strcmp(aDataSearch, "END_TEXT"))
					{// 読み込みを終了
						break;
					}
					else if (!strcmp(aDataSearch, "SET_ROW"))
					{// 文字の読み込み
						while (1)
						{
							fscanf(pFile, "%s", aDataSearch);	// 検索

							if (!strcmp(aDataSearch, "END_ROW"))
							{// 読み込みを終了
								nCntString++;	// 列情報の加算
								break;
							}
							else if (!strcmp(aDataSearch, "NUM_CONV"))
							{// 変換数の読み込み

								int nConvMin = 1;
								int nConvMax = 0;

								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nConvMax);

								nConvMax = nConvMin + nConvMax;

								// メモリの解放
								m_pString[nCntString].pConv = new Conv[nConvMax];

								m_pString[nCntString].nConvMax = nConvMax;
							}
							else if (!strcmp(aDataSearch, "NUM_WORDS"))
							{// 文字数の読み込み

								int nLettreMax = -1;

								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nLettreMax);

								for (int nConv = 0; nConv < m_pString[nCntString].nConvMax; nConv++)
								{
									m_pString[nCntString].pConv[nConv].pLetter = new Letter[nLettreMax];
								}
								m_pString[nCntString].nLettreMax = nLettreMax;
							}
							else if (!strcmp(aDataSearch, "SET_WORDS"))
							{// タイプの読み込み
								int nCountLetter = 0;	// 文字数
								int nCountConv = 0;

								while (1)
								{
									fscanf(pFile, "%s", &aDataSearch[0]);	// 検索

									if (!strcmp(aDataSearch, "END_WORDS"))
									{// 読み込みを終了
										break;
									}
									else if (!strcmp(aDataSearch, "WOARDS")) 
									{// 文字の読み込み
										char aString[TXT_MAX];	// 文字列格納
										char *ptr;				// 分割文字の格納

										fscanf(pFile, "%s", &aString[0]);

										// カンマを区切りに文字列を分割
										ptr = strtok(aString, ",");

										if (nCountLetter < m_pString[nCntString].nLettreMax)
										{
											sprintf(m_pString[nCntString].pConv[nCountConv].pLetter[nCountLetter].aConv, "%s", ptr);
											nCountLetter++;
										}

										while (ptr != NULL)
										{
											// strtok関数により変更されたNULLのポインタが先頭
											ptr = strtok(NULL, ",");

											// ptrがNULLの場合エラーが発生するので対処
											if (ptr != NULL) 
											{
												if (nCountLetter < m_pString[nCntString].nLettreMax)
												{
													sprintf(m_pString[nCntString].pConv[nCountConv].pLetter[nCountLetter].aConv, "%s", ptr);
													nCountLetter++;
												}
											}
										}
										nCountConv++;
									}
									else if (!strcmp(aDataSearch, "CONV"))
									{// 文字の読み込み
										nCountLetter = 0;	// 文字数
										char aString[TXT_MAX];	// 文字列格納
										char *ptr;				// 分割文字の格納

										fscanf(pFile, "%s", &aString[0]);

										// 最大数超えてたら
										if (nCountConv >= m_pString[nCntString].nConvMax)
										{
											return;
										}

										// カンマを区切りに文字列を分割
										ptr = strtok(aString, ",");

										if (nCountLetter < m_pString[nCntString].nLettreMax)
										{
											// 更新した順位の名前を初期化
											sprintf(m_pString[nCntString].pConv[nCountConv].pLetter[nCountLetter].aConv, "%s", ptr);
											nCountLetter++;
										}

										while (ptr != NULL)
										{
											// strtok関数により変更されたNULLのポインタが先頭
											ptr = strtok(NULL, ",");

											// ptrがNULLの場合エラーが発生するので対処
											if (ptr != NULL)
											{
												if (nCountLetter < m_pString[nCntString].nLettreMax)
												{
													sprintf(m_pString[nCntString].pConv[nCountConv].pLetter[nCountLetter].aConv, "%s", ptr);	
													nCountLetter++;
												}
											}
										}

										nCountConv++;
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
}