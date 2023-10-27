//========================================
//
// スコア処理
//
//========================================
// *** score2D.cpp ***
//========================================
#include "time.h"
#include "../model/block.h"
#include "../../scene/game.h"
#include "../../scene/title.h"
#include "../../scene/pause.h"
#include "../../system/words/text.h"
#include "../../system/words/words.h"
#include "../../system/words/font.h"

//========================================
// コンストラクタ
//========================================
CTime::CTime(int nPriority) : CObject(nPriority)
{
	m_nTime = 0;			// 時間
	m_nTimeMax  = 0;		// 時間の最大値
	m_nCounter = 0;			// カウンター
	m_Digit = 0;			// 桁数
	m_bSetTime = false;		// 設定フラグ
	m_bUpdateTime = false;	// 更新フラグ
	m_Time = NULL;
}

//========================================
// デストラクタ
//========================================
CTime::~CTime(void)
{

}

//========================================
// 生成処理
//========================================
CTime *CTime::Create(int nTime)
{
	CTime *pTime = new CTime;

	 // 初期化処理
	pTime->Init();
	pTime->SetTime(nTime);

	return pTime;
}

//========================================
// 初期化処理
//========================================
HRESULT CTime::Init(void)
{
	m_nTime = 0;	// タイム
	m_nCounter = 0;	// カウンター
	m_nNumWarn = 0;

	// 種類設定
	SetType(TYPE_TIME);

	return S_OK;
}

//========================================
// 終了処理
//========================================
void CTime::Uninit(void)
{
	if (m_Time != NULL)
	{
		m_Time = NULL;
	}
}

//========================================
// 更新処理
//========================================
void CTime::Update(void)
{
	bool bStart = CTitle::IsStart();
	
	if (bStart)
	{
		bool bPause = CPause::IsPause();
		bool bTime = CGame::IsTime();

		if (!bPause && !bTime)
		{
			if (m_nTime > 0)
			{// タイムが0以上の時、

				// カウンターが0になった時、
				m_nCounter = (m_nCounter + 1) % 60;
				if (m_nCounter == 0)
				{
					m_nTime--;			// タイムを減算
					SetTime(m_nTime);	// タイム設定
				}
			}
		}

		// テキストの更新
		if (m_bUpdateTime)
		{
			// タイムを文字列に設定
			char aTime[TXT_MAX];
			int nNumSet = 0;
			D3DXCOLOR col;

			sprintf(aTime, "%02d", m_nTime);

			// 長さを取得
			m_Digit = strlen(aTime);

			int n = (m_nTimeMax * 2) / 3;

			if (m_nTime <= 0)
			{
				col = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);
			}
			else if (m_nTime <= m_nTimeMax / 3)
			{
				col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (m_nTime <= (m_nTimeMax * 2) / 3)
			{
				col = D3DXCOLOR(0.9f, 0.9f, 0.0f, 1.0f);
			}
			else if (m_nTime < m_nTimeMax)
			{
				col = INIT_D3DXCOLOR;
			}

			for (int nTime = 0; nTime < m_Digit; nTime++)
			{
				if (m_Time->ChgWords(&aTime[nTime], TIME_START_DEX + nTime, col))
				{
					nNumSet++;
				}
			}

			if (nNumSet == m_Digit)
			{
				m_bUpdateTime = false;
			}
		}

		// 警告テキストの表示
		{
			// 残り時間をテキストに設定
			char aString[TXT_MAX];
			sprintf(aString, "残り %02d秒！！", m_nTime);
			D3DXCOLOR col;

			FormFont pFont = { INIT_D3DXCOLOR,20.0f,1,60,60 };
			FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),true,D3DXVECTOR3(2.0f,2.0f,0.0f),D3DXVECTOR2(1.0f,1.0f) };

			if (m_nTime == m_nTimeMax / 3 && m_nNumWarn == 1)
			{
				CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR2(350.0f, 100.0f),
					aString, CFont::FONT_BESTTEN, &pFont, true, &pShadow);
				m_nNumWarn++;
			}
			else if (m_nTime == m_nTimeMax * 2 / 3 && m_nNumWarn == 0)
			{
				CText::Create(CText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR2(350.0f, 100.0f),
					aString, CFont::FONT_BESTTEN, &pFont, true, &pShadow);
				m_nNumWarn++;
			}
		}
	}
}

//========================================
// 描画処理
//========================================
void CTime::Draw(void)
{

}

//========================================
// 設定
//========================================
void CTime::SetTime(int nTime)
{
	// 時間設定
	if (!m_bSetTime)
	{
		// タイム代入
		m_nTime = nTime;
		m_nTimeMax = nTime;

		// タイムを文字列に設定
		char aString[TXT_MAX];
		sprintf(aString, "TIME ：%02d", m_nTime);

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

		m_Time = CText::Create(CText::BOX_NORMAL_RECT,
			D3DXVECTOR3(SCREEN_WIDTH - 290.0f, 21.0f, 0.0f),
			D3DXVECTOR2(0.0f, 0.0f),
			aString,
			CFont::FONT_BESTTEN,
			&pFont,false,&pShadow);

		m_bSetTime = true;
	}
	else
	{
		m_bUpdateTime = true;
	}
}