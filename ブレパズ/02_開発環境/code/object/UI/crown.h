//========================================
// 
// 王冠
// 
//========================================
// *** corown.h ***
//========================================
#ifndef _CROWN_H_	//このマクロ定義がされてなかったら
#define _CROWN_H_	//２重インクルード防止のマクロ定義

#include "../../main.h"
#include "../object2D.h"

//****************************************
//　クラス
//****************************************
class CCrown : public CObject2D
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
	CCrown(int nPriority = PRIO_UI);
	~CCrown();

	// -- メイン ------------------------------------------
	/* 初期化		*/HRESULT Init(void);
	/* 終了			*/void Uninit(void);
	/* 更新			*/void Update(void);
	/* 描画			*/void Draw(void);
	/* 生成			*/static CCrown *Create(int nType);

private:
	// ***** 変数 *****
	Info m_Info;		// 情報
};

#endif