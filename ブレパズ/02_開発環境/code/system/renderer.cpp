//========================================
// 
// 描画処理
// 
//========================================
// *** renderer.cpp ***
//========================================
#include "renderer.h"
#include "../object\object.h"

// 静的変数
LPD3DXFONT CRenderer::m_pFont = NULL;	// フォント

//========================================
// コンストラクタ
//========================================
CRenderer::CRenderer()
{
	// 値をクリアする
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//========================================
// デストラクタ
//========================================
CRenderer::~CRenderer()
{

}

//========================================
// 描画の初期化処理
//========================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;        //ディスプレイモード

								 //Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));   //パラメータのゼロクリア

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;								//ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	m_d3dpp.BackBufferCount = 1;										//バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;								//デプスバッファとステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	m_d3dpp.Windowed = bWindow;											//ウインドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//インターバル

																		//Direct3Dデバイスも生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,                     //描画処理
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,//頂点処理
		&m_d3dpp,
		&m_pD3DDevice)))                    //描画処理と頂点処理をハードウェアで行う
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,//描画処理
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,//頂点処理
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,                     //描画処理
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,//頂点処理
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//デバック表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PALETTE,
		"Terminal", &m_pFont);

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//D3DCULL_NONE　カリング無し D3DCULL_CW 表面カリング　D3DCULL_CCW　裏面カリング
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	return S_OK;
}

//========================================
// 描画の終了処理
//========================================
void CRenderer::Uninit(void)
{
	// オブジェクトの全体の破棄
	CObject::ReleaseAll();

	//デバック表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}


	// オブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//========================================
// 描画の更新処理
//========================================
void CRenderer::Update(void)
{
	CObject::UpdateAll();		// オブジェクト全体
}

//========================================
// 描画の描画処理
//========================================
void CRenderer::Draw(void)
{
	//画面クリア(バックバッファ&Zバッファのクリア)
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

	 // オブジェクト全体の描画処理
		CObject::DrawAll();

		//描画終了
		m_pD3DDevice->EndScene();

#ifdef _DEBUG

		//FPSの表示
		DrawFPS();

#endif // _DEBUG
	}
	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// FPS処理
//========================================
void CRenderer::DrawFPS(void)
{
	//テキストの描画処理
	RECT rect = { 0,230,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[521];

	//文字列に代入
	sprintf(&aStr[0], "FPS:%d\n", GetFps());

	//テキストの描画
	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}