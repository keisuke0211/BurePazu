//========================================
// 
// テキスト(2D)
// 
//========================================
//  *** text2D.cpp ***
//========================================
#include "text2D.h"
#include "../object2D.h"
#include "../../system/texture.h"

//========================================
// コンストラクタ
//========================================
CText2D::CText2D(int nPriority) : CObject(nPriority)
{
	sprintf(m_aString, "");		// 文字列のポインタ
	m_disp = DISPLAY_LEFT;		// 表示形式
	m_pos = INIT_D3DXVECTOR3;	// 位置
	m_rot = INIT_D3DXVECTOR3;	// 向き
	m_fWidth = 0.0f;			// 幅
	m_fHeight = 0.0f;			// 高さ

	// オブジェクト2Dのポインタ配列
	for (int nCntObj = 0; nCntObj < TXT_MAX; nCntObj++)
	{
		m_apObject2D[nCntObj] = NULL;
	}
}

//========================================
// デストラクタ
//========================================
CText2D::~CText2D() 
{

}

//========================================
// 初期化処理
//========================================
HRESULT CText2D::Init(void) 
{
	// オブジェクト2Dのポインタ配列解放
	for (int nCntObj = 0; nCntObj < TXT_MAX; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != NULL)
		{
			m_apObject2D[nCntObj]->Uninit();
			m_apObject2D[nCntObj] = NULL;
		}
	}

	// 種類設定
	SetType(TYPE_TEXT2D);

	return S_OK;
}

//========================================
// 終了処理
//========================================
void CText2D::Uninit(void) 
{
	// オブジェクト2Dのポインタ配列解放
	for (int nCntObj = 0; nCntObj < TXT_MAX; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != NULL)
		{
			m_apObject2D[nCntObj]->Uninit();
			m_apObject2D[nCntObj] = NULL;
		}
	}

	Release();
}

//========================================
// 更新処理
//========================================
void CText2D::Update(void) 
{

}

//========================================
// 描画処理
//========================================
void CText2D::Draw(void)
{

}

//========================================
// 文字列設定処理
//========================================
void CText2D::SetString(char *pString) 
{
	// 文字列代入
	sprintf(m_aString, pString);

	int nStrLen = (int)strlen(m_aString);

	for (int nCntDigit = 0; nCntDigit < TXT_MAX; nCntDigit++)
	{
		if (nCntDigit >= nStrLen)
		{// 文字カウントが文字列の長さに達していた時、
		 // オブジェクトの解放
			if (m_apObject2D[nCntDigit] != NULL)
			{
				m_apObject2D[nCntDigit]->Uninit();
				m_apObject2D[nCntDigit] = NULL;
			}
			continue;	// 折り返す
		}
		else if (m_aString[nCntDigit] == ' ')
		{// カウントの文字が空白の時、
		 // オブジェクトの解放
			if (m_apObject2D[nCntDigit] != NULL)
			{
				m_apObject2D[nCntDigit]->Uninit();
				m_apObject2D[nCntDigit] = NULL;
			}
			continue;	// 折り返す
		}

		// 生成
		if (m_apObject2D[nCntDigit] == NULL)
		{
			// オブジェクト2D
			CObject2D *pObject2D = NULL;

			// オブジェクト2Dの生成
			pObject2D = new CObject2D(PRIO_UI);

			// 初期化処理
			pObject2D->Init();

			// アドレス代入
			m_apObject2D[nCntDigit] = pObject2D;
		}

		// 設定位置
		D3DXVECTOR3 setPos = m_pos;

		// 表示形式に応じた設定位置の設定
		switch (m_disp)
		{
		case DISPLAY_CENTER:
			// 設定位置に文字の最終的な長さの半分を減算
			setPos.x -= m_fWidth * nStrLen * 0.5f;
			// 設定位置に配置間隔Xを適用(加算)
			setPos.x += ((float)nCntDigit + 0.5f) * m_fWidth;
			break;
		case DISPLAY_LEFT:
			// 設定位置に配置間隔Xを適用(加算)
			setPos.x += nCntDigit * m_fWidth;
			break;
		case DISPLAY_RIGHT:
			// 設定位置に文字の最終的な長さを減算
			setPos.x -= m_fWidth * (nStrLen - 1);
			// 設定位置に配置間隔Xを適用(加算)
			setPos.x += nCntDigit * m_fWidth;
			break;
		}

		// 位置設定
		m_apObject2D[nCntDigit]->SetPos(setPos);

		// 向き設定
		m_apObject2D[nCntDigit]->SetRot(INIT_D3DXVECTOR3);

		// サイズ設定
		m_apObject2D[nCntDigit]->SetSize(m_fWidth, m_fHeight);

		// パターン情報設定
		m_apObject2D[nCntDigit]->SetPtn((int)m_aString[nCntDigit] - START_CHAR);
		m_apObject2D[nCntDigit]->SetPtnInfo(PTN_WIDTH, PTN_HEIGHT, PTN_MAX);

		// テクスチャ割り当て
		m_apObject2D[nCntDigit]->BindTexture(CTexture::TEX_SEA01);
	}
}

//========================================
// 生成処理
//========================================
CText2D *CText2D::Create(void)
{
	CText2D *pText2D = NULL;

	// テキスト2Dの生成
	pText2D = new CText2D;

	// 初期化処理
	pText2D->Init();

	return pText2D;
}