//========================================
// 
// 王冠
// 
//========================================
// *** corown.h ***
//========================================
#include "crown.h"
#include "../../manager.h"
#include "../../system/renderer.h"
#include "../../system/texture.h"

//========================================
// コンストラクタ
//========================================
CCrown::CCrown(int nPriority) : CObject2D(nPriority)
{
	m_Info.pos = INIT_D3DXVECTOR3;
	m_Info.size = INIT_D3DXVECTOR2;
	m_Info.col = INIT_D3DXCOLOR;
	m_Info.nType = 0;
}

//========================================
// デストラクタ
//========================================
CCrown::~CCrown()
{

}

//========================================
// 生成
//========================================
CCrown *CCrown::Create(int nType)
{
	CCrown *pObj = new CCrown;

	pObj->m_Info.pos = D3DXVECTOR3(1075.0f, 100.0f, 0.0f);
	pObj->m_Info.size = D3DXVECTOR2(65, 65);
	pObj->m_Info.nType = nType;
	pObj->Init();

	return pObj;
}

//========================================
// 初期化
//========================================
HRESULT CCrown::Init(void)
{
	CObject2D::Init();

	// 種類の設定
	SetType(TYPE_BG);

	// 設定
	SetPos(m_Info.pos);
	SetSize(m_Info.size.x, m_Info.size.y);
	SetColar(D3DXCOLOR(1, 1, 1, 1));

	switch (m_Info.nType)
	{
	case 0:
		BindTexture(CTexture::TEX_CROWN00);
		break;
	case 1:
		BindTexture(CTexture::TEX_CROWN01);
		break;
	case 2:
		BindTexture(CTexture::TEX_CROWN02);
		break;
	default:
		BindTexture(CTexture::TEX_CROWN00);
		break;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CCrown::Uninit(void)
{
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CCrown::Update(void)
{
	CObject2D::Update();
}

//========================================
// 描画
//========================================
void CCrown::Draw(void)
{
	CObject2D::Draw();
}