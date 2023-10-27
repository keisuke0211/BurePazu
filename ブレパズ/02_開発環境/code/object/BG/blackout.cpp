//========================================
// 
// 暗転
// 
//========================================
// *** blackout.cpp ***
//========================================
#include "blackout.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../system/texture.h"

//========================================
// コンストラクタ
//========================================
CBlackout::CBlackout(int nPriority) : CObject2D(nPriority)
{
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.size = INIT_D3DXVECTOR2;
	m_Info.col = INIT_D3DXCOLOR;
	m_Info.nType = 0;
}

//========================================
// デストラクタ
//========================================
CBlackout::~CBlackout()
{

}

//========================================
// 生成
//========================================
CBlackout *CBlackout::Create(void)
{
	CBlackout *pObj = new CBlackout;

	pObj->m_Info.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	pObj->m_Info.size = D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT);
	pObj->Init();

	return pObj;
}

//========================================
// 初期化
//========================================
HRESULT CBlackout::Init(void)
{
	CObject2D::Init();

	// 種類の設定
	SetType(TYPE_BG);

	// 設定
	SetPos(m_Info.pos);
	SetSize(m_Info.size.x, m_Info.size.y);
	SetColar(D3DXCOLOR(0,0,0,0.7f));

	BindTexture(-1);

	return S_OK;
}

//========================================
// 終了
//========================================
void CBlackout::Uninit(void)
{
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CBlackout::Update(void)
{
	CObject2D::Update();
}

//========================================
// 描画
//========================================
void CBlackout::Draw(void)
{
	CObject2D::Draw();
}