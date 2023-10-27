//========================================
// 
// タイトル背景
// 
//========================================
// *** UI_title.h ***
//========================================
#include "UI_title.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../system/texture.h"

//========================================
// コンストラクタ
//========================================
CUiTitle::CUiTitle(int nPriority) : CObject2D(nPriority)
{
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.col = INIT_D3DXCOLOR;
	m_Info.nType = 0;
}

//========================================
// デストラクタ
//========================================
CUiTitle::~CUiTitle()
{

}

//========================================
// 生成
//========================================
CUiTitle *CUiTitle::Create(void)
{
	CUiTitle *pTitle = new CUiTitle;

	pTitle->m_Info.pos = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,0.0f);
	pTitle->m_Info.size = D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT);
	pTitle->Init();

	return pTitle;
}

//========================================
// 初期化
//========================================
HRESULT CUiTitle::Init(void)
{
	CObject2D::Init();

	// 種類の設定
	SetType(TYPE_BG);

	// 設定
	SetPos(m_Info.pos);
	SetSize(m_Info.size.x, m_Info.size.y);
	SetColar(INIT_D3DXCOLOR);

	BindTexture(CTexture::TEX_TITLE_BG);

	return S_OK;
}

//========================================
// 終了
//========================================
void CUiTitle::Uninit(void)
{
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CUiTitle::Update(void)
{
	CObject2D::Update();
}

//========================================
// 描画
//========================================
void CUiTitle::Draw(void)
{
	CObject2D::Draw();
}