//========================================
// 
// タイトル背景
// 
//========================================
// *** UI_title.h ***
//========================================
#ifndef _UI_TITLE_H_	//このマクロ定義がされてなかったら
#define _UI_TITLE_H_	//２重インクルード防止のマクロ定義

#include "../../main.h"
#include "../object2D.h"

//****************************************
//　クラス
//****************************************
class CUiTitle : public CObject2D
{
public:

	// ***** 構造体 *****

	// 共通情報
	struct Info
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR2 size;		// 大きさ
		D3DXCOLOR col;			// 色
		int nType;				// 種類
	};

	// ***** 関数 *****
	CUiTitle(int nPriority = PRIO_OBJECT);
	~CUiTitle();

	// -- メイン ------------------------------------------
	/* 初期化		*/HRESULT Init(void);
	/* 終了			*/void Uninit(void);
	/* 更新			*/void Update(void);
	/* 描画			*/void Draw(void);
	/* 生成			*/static CUiTitle *Create(void);

private:
	// ***** 変数 *****
	Info m_Info;		// 情報
};

#endif

