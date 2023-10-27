//========================================
// 
// テクスチャ管理
// 
//========================================
// *** texture.cpp ***
//========================================
#include "texture.h"
#include "../manager.h"
#include "renderer.h"

// 定義
const char* CTexture::FILE_PATH = "data\\GAMEDATA\\TEXTURE\\TEXTURE_DATA.txt";

// 静的メンバ変数
int CTexture::m_NumAll = 0;
char CTexture::m_FileName[CTexture::MAX_TEXTURE][TXT_MAX] = { NULL };

//========================================
// コンストラクタ
//========================================
CTexture::CTexture()
{

}

//========================================
// デストラクタ
//========================================
CTexture::~CTexture()
{

}

//========================================
// 読み込み
//========================================
HRESULT CTexture::Load(void)
{
	char aDataSearch[TXT_MAX];	// データ検索用

	// ファイルの読み込み
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return S_OK;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", &aDataSearch[0]);

		if (!strcmp(aDataSearch, "END"))
		{// 読み込みを終了
			fclose(pFile);
			break;
		}
		if (aDataSearch[0] == '#')
		{// 折り返す
			continue;
		}

		if (!strcmp(aDataSearch, "TEXTURE"))
		{
			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%s", &m_FileName[m_NumAll][0]); // ファイル名

			Regist(&m_FileName[m_NumAll][0]);
		}
	}

	return S_OK;
}

//========================================
// テクスチャ破棄
//========================================
void CTexture::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < m_NumAll; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//========================================
// 指定テクスチャの読み込み
//========================================
int CTexture::Regist(const char* pFilename)
{
	int nIdx = -1;	// テクスチャ番号

	if (m_NumAll >= MAX_TEXTURE)
	{// 最大数超えたか
		for (int nCntFile = 0; nCntFile < m_NumAll; nCntFile++)
		{
			if (strcmp(&m_FileName[nCntFile][0], pFilename) == 0)
			{
				return nCntFile;
			}
		}
		return 0;
	}

	// 既に読み込んだか調べる
	for (int nCntFile = 0; nCntFile < m_NumAll; nCntFile++)
	{
		if (strcmp(&m_FileName[nCntFile][0], pFilename) == 0)
		{
			return nCntFile;
		}
	}

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, pFilename, &m_apTexture[m_NumAll])))
	{
		m_apTexture[m_NumAll] = NULL;
	}

	// ファイルパスを保存
	strcpy(&m_FileName[m_NumAll][0],pFilename);

	nIdx = m_NumAll;

	m_NumAll++;	// 総数を加算

	return nIdx;
}

//========================================
// 指定テクスチャの取得
//========================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx == -1)
	{// テクスチャを使用しない
		return NULL;
	}

	assert(nIdx >= -1 && nIdx <= m_NumAll + 1);

	return m_apTexture[nIdx];
}