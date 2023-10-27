//========================================
//
// スコア処理
//
//========================================
// *** score2D.cpp ***
//========================================
#include "score.h"
#include "../../system/words/text.h"
#include "../../system/words/words.h"
#include "../../system/words/font.h"

// 静的メンバ変数定義
int CScore::m_nScore = 0;	// スコア
bool CScore::m_bUpdateText = false;	// 更新フラグ

//========================================
// コンストラクタ
//========================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	m_Score = NULL;
	m_bUpdateText = false;
	m_Digit = 0;
}

//========================================
// デストラクタ
//========================================
CScore::~CScore(void) 
{

}

//========================================
// 生成処理
//========================================
CScore *CScore::Create(void)
{
	CScore *pScore = NULL;

	// スコアの生成
	pScore = new CScore;

	// 初期化処理
	pScore->Init();

	return pScore;
}

//========================================
// 初期化処理
//========================================
HRESULT CScore::Init(void)
{
	m_nScore = 0;

	{// スコアを文字列に設定
		char aString[TXT_MAX];
		sprintf(aString, "SCORE：%06d", m_nScore);

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

		m_Score = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(SCREEN_WIDTH - 290.0f, 63.0f, 0.0f),
			D3DXVECTOR2(0.0f, 0.0f),
			aString,
			CFont::FONT_BESTTEN,
			&pFont, false, &pShadow);
	}

	// 種類設定
	SetType(TYPE_SCORE);

	return S_OK;
}

//========================================
// 終了処理
//========================================
void CScore::Uninit(void)
{
	if (m_Score != NULL)
	{
		m_Score = NULL;
	}
}

//========================================
// 更新処理
//========================================
void CScore::Update(void)
{
	if (m_bUpdateText)
	{
		int nNumSet = 0;
		char aString[TXT_MAX];
		sprintf(aString, "%06d", m_nScore);

		// 長さを取得
		m_Digit = strlen(aString);

		for (int nTime = 0; nTime < m_Digit; nTime++)
		{
			if (m_Score->ChgWords(&aString[nTime], SCORE_START_DEX + nTime, INIT_D3DXCOLOR))
			{
				nNumSet++;
			}
		}

		if (nNumSet == m_Digit)
		{
			m_bUpdateText = false;
		}
	}
}

//========================================
// 描画処理
//========================================
void CScore::Draw(void)
{

}

//========================================
// 設定
//========================================
void CScore::SetScore(int nScore)
{
	m_nScore += nScore;

	if (!m_bUpdateText)
	{
		m_bUpdateText = true;
	}
}