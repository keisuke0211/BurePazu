//========================================
// 
// 描画処理
// 
//========================================
// *** renderer.h ***
//========================================
#include "../main.h"

#ifndef _RENDERER_H_
#define _RENDERER_H_

//****************************************
// クラス
//****************************************
class CRenderer
{
public:

	/* 定義 */
	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット【2D】

	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;	//頂点座標
		float rhw;			//座標変換係数（1.0fで固定）
		D3DCOLOR col;		//頂点カラー
		D3DXVECTOR2 tex;	//テクスチャ座標
	};

	// ***** 関数 *****

	CRenderer();
	~CRenderer();

	/* レンダラー */
	HRESULT Init(HWND hWnd, BOOL bWindow);	// 初期化
	void Uninit(void);						// 終了
	void Update(void);						// 更新
	void Draw(void);						// 描画

	/* 取得 */
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// デバイス

	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void);

private:

	// ***** 関数 *****
	void DrawFPS(void);		// FPS表示

	// ***** 変数 *****
	D3DPRESENT_PARAMETERS m_d3dpp = {};	// プレゼンテーションパラメータ
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ

	static LPD3DXFONT m_pFont;			// フォントへのポインタ
};
#endif