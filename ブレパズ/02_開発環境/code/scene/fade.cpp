//========================================
// 
// フェード
// 
//========================================
// *** fade.cpp ***
//========================================
#include "fade.h"
#include "../manager.h"

CFade::FADE CFade::m_fade = FADE_NONE;
CScene::MODE CFade::m_modeNext = CScene::MODE_TITLE;

//========================================
// コンストラクタ
//========================================
CFade::CFade(int nPriority) : CObject2D(nPriority)
{
	m_color = INIT_D3DXCOLOR;
}

//========================================
// デストラクタ
//========================================
CFade::~CFade()
{

}

//========================================
// 初期化
//========================================
HRESULT CFade::Init(void)
{
	CObject2D::Init();

	m_fade = FADE_IN;

	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 黒いポリゴン(不透明)にする

	BindTexture(-1);
	SetType(TYPE_NONE);
	SetPos(D3DXVECTOR3(640, 360, 0.0f));
	SetSize(1280, 720);
	SetColar(m_color);

	// モード設定
	CManager::SetMode(m_modeNext);
	return S_OK;
}

//========================================
// 終了
//========================================
void CFade::Uninit(void)
{
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{//　フェードイン
			m_color.a -= (float)FADE_TRANSITION / 1000;

			if (m_color.a <= 0.0f)
			{// 透明度が0以下の時

				m_color.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{// フェードアウト
			m_color.a += (float)FADE_TRANSITION / 1000;

			if (m_color.a > 1.0f)
			{// 透明度が最大値を上回った時、

				m_color.a = 1.0f;
				m_fade = FADE_IN;

				if (m_modeNext != CScene::MODE_MAX)
				{// 次のモードが無しでない時,

					CManager::SetMode(m_modeNext);
				}
			}
		}
	}

	SetColar(m_color);
	CObject2D::Update();
}

//========================================
// 描画
//========================================
void CFade::Draw(void)
{
	CObject2D::Draw();
}

//========================================
// 設定
//========================================
void CFade::SetFade(CScene::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_color = D3DXCOLOR(0, 0, 0, 0);
}