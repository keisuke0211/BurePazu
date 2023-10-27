//========================================
// 
// フォント
// 
//========================================
// *** font.cpp ***
//========================================
#include "font.h"
#include "../../manager.h"
#include "../renderer.h"

//　フォントパス
const char* CFont::s_FileName[] =
{
	"data/font/Nosutaru.ttf",
	"data/font/BestTen-CRT.otf",
	"data/font/FZゴンタかな.otf",
	"data/font/meiryob.ttc",
};
static_assert(sizeof(CFont::s_FileName) / sizeof(CFont::s_FileName[0]) == CFont::FONT_MAX, "bagu");

// フォント名
const char* CFont::s_FontName[] =
{
	"ノスタルドット（M+）",
	"BestTen-CRT",
	"FZゴンタかな",
	"メイリオ",
};
static_assert(sizeof(CFont::s_FontName) / sizeof(CFont::s_FontName[0]) == CFont::FONT_MAX, "aho");


//========================================
// コンストラクタ
//========================================
CFont::CFont() :
	m_texFont()
{
	memset(m_texFont, 0, sizeof(m_texFont));
}

//========================================
// デストラクタ
//========================================
CFont::~CFont()
{

}

//========================================
// フォント作成
//========================================
void CFont::FontCreate(FONT nFont)
{
	assert(nFont >= 0 && nFont < FONT_MAX);

	if (m_texFont[nFont] != NULL)
	{// テクスチャの読み込みがされている
		return;
	}

	// フォントを使えるようにする
	DESIGNVECTOR design;

	AddFontResourceEx(
		s_FileName[nFont], //ttfファイルへのパス
		FR_PRIVATE,
		&design
	);

	// フォントの生成
	int fontsize = 60;
	m_logFont[nFont] = { fontsize, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN };

	strcpy(m_logFont[nFont].lfFaceName, s_FontName[nFont]);
}

//========================================
// テクスチャ作成
//========================================
void CFont::TextureCreate(string nWords, FONT nFont)
{
	if (!(m_hFont[nFont] = CreateFontIndirect(&m_logFont[nFont])))
	{

	}

	// デバイスコンテキスト取得
	// ※デバイスにフォントを持たせないとGetGlyphOutline関数はエラーになる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, m_hFont[nFont]);

	string words = nWords;
	// 文字コード取得
	const char *c = words.c_str();
	UINT code = 0;
#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換
	code = (UINT)*c;
#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]
	if (IsDBCSLeadByte(*c))
		code = (BYTE)c[0] << 8 | (BYTE)c[1];
	else
		code = c[0];
#endif

	// フォントビットマップ取得
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 2, NULL, &Mat);
	BYTE *ptr = new BYTE[size];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(m_hFont[nFont]);
	ReleaseDC(NULL, hdc);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ作成
	if (FAILED(pDevice->CreateTexture(GM.gmCellIncX, TM.tmHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_texFont[nFont], NULL)))
		if (FAILED(pDevice->CreateTexture(GM.gmCellIncX, TM.tmHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texFont[nFont], NULL)))
		{

		}

	// テクスチャにフォントビットマップ書き込み
	D3DLOCKED_RECT LockedRect;
	if (FAILED(m_texFont[nFont]->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD)))
		if (FAILED(m_texFont[nFont]->LockRect(0, &LockedRect, NULL, 0)))
		{

		}

	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
	int iOfs_x = GM.gmptGlyphOrigin.x;
	int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	int iBmp_h = GM.gmBlackBoxY;
	int Level = 17;
	DWORD Alpha, Color;
	FillMemory(LockedRect.pBits, LockedRect.Pitch * TM.tmHeight, 0);

	int nBlackBox = GM.gmBlackBoxX;
	for (int y = iOfs_y; y<iOfs_y + iBmp_h; y++)
		for (int x = iOfs_x; x<iOfs_x + nBlackBox; x++) {
			Alpha = (255 * ptr[x - iOfs_x + iBmp_w*(y - iOfs_y)]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy((BYTE*)LockedRect.pBits + LockedRect.Pitch*y + 4 * x, &Color, sizeof(DWORD));
		}

	// テクスチャのロック
	m_texFont[nFont]->UnlockRect(0);

	delete[] ptr;
}

//========================================
// 全て破棄
//========================================
void CFont::ReleaseAll(void)
{
	for (int nFont = 0; nFont < FONT_MAX; ++nFont)
	{
		if (m_texFont[nFont] != NULL)
		{// テクスチャの解放
			m_texFont[nFont]->Release();
			m_texFont[nFont] = NULL;
		}
	}
}

//========================================
// 指定の破棄
//========================================
void CFont::Release(FONT nFont)
{
	assert(nFont >= 0 && nFont < FONT_MAX);
	DESIGNVECTOR design;

	if (m_texFont[nFont] != NULL)
	{// テクスチャの解放
		m_texFont[nFont]->Release();
		m_texFont[nFont] = NULL;
	}
	if (s_FileName[nFont] != NULL)
	{
		// フォント削除
		RemoveFontResourceEx(
			TEXT(s_FileName[nFont]), //ttfファイルへのパス
			FR_PRIVATE,
			&design
		);
	}
}

//========================================
// テクスチャ情報の取得
//========================================
LPDIRECT3DTEXTURE9 CFont::GetFont(string words, FONT nFont)
{
	if (nFont == FONT_NONE)
	{// テクスチャを使用しない
		return NULL;
	}

	assert(nFont >= 0 && nFont < FONT_MAX);

	// 読み込み
	FontCreate(nFont);

	TextureCreate(words, nFont);

	return m_texFont[nFont];
}