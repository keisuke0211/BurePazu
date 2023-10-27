//========================================
// 
// ライト処理
// 
//========================================
// *** light.h ***
//========================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../main.h"

#define NUM_LIGHT			(3)		//ライトの数
#define LIGHT_CLOR			(1.0f)	//全体のライト色
//****************************************
// クラス
//****************************************
class CLight
{
public:

	// ライト情報
	typedef struct
	{
		// 位置関連
		D3DXVECTOR3	rot;	// 向き
		D3DXCOLOR	col;	// 色
	}Light;

	// ***** 関数 *****
	CLight();	// コンストラクタ
	~CLight();	// デストラクタ

	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

private:

	// ***** 変数 *****
	D3DLIGHT9 m_lihgt;				// ライトの情報
	D3DLIGHT9 m_aLight[NUM_LIGHT];	// ライト情報

	const Light m_lihgtInformation[NUM_LIGHT] =
	{
		{ D3DXVECTOR3(-0.9f , -0.9f , 0.5f),D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// 上・前
		{ D3DXVECTOR3(0.2f , 0.9f , -0.2f)	,D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// 下
		{ D3DXVECTOR3(0.9f , -0.9f , -0.5f),D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// 右・後
	};
};
#endif