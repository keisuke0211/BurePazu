//========================================
// 
// ブロック処理
// 
//========================================
// *** block.h ***
//========================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "../../main.h"
#include "../objectX.h"

//****************************************
// クラス
//****************************************
class CBlock : public CObjectX
{
public:

	// 状態
	enum STATE
	{
		STATE_NORMAL = 0,	// 通常
		STATE_BREAK,		// 破壊可
		STATE_ALLIE,		// 貫通
		STATE_MAX,
	};

	// ***** 構造体 *****

	// 種類情報
	struct TypeInfo
	{
		int nModelID;	// モデルID
		int nState;		// 状態
	};

	// ブロック情報
	struct Info
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 posOld;		// 位置(過去)
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 rotOld;		// 向き(過去)
		D3DXVECTOR3	size;		// サイズ
		D3DXCOLOR col;			// 頂点カラー
		int nModelID;			// 種類
		STATE state;			// 状態
		float fRadius;			// 半径
		int nCntRadius;			// 半径・推移時間
		float fRadiusRate;		// 半径の割合
		bool bSet;				// 配置フラグ
		int nID;				// 自身のID

		int nEraseTime;			// 消すまでの時間
		bool bErase;			// 消すフラグ
	};

	// ***** 関数 *****
	CBlock(int nPriority = PRIO_BLOCK);
	~CBlock();

	// 生成
	static CBlock *Create(int nType,D3DXVECTOR3 pos,int nState);

	/* 初期化		*/HRESULT Init(void);
	/* 終了			*/void Uninit(void);
	/* 更新			*/void Update(void);
	/* 描画			*/void Draw(void);
	/* 読み込み		*/static void Load(void);
	/* HIT			*/void HitBlock(void);
	
	// -- 設定 ---------------------------------------------
	/* 位置		*/virtual void SetBlockPos(const D3DXVECTOR3 pos) { m_Info.pos = m_Info.posOld = pos; }
	/* 向き		*/void SetBlockRot(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	/* 色		*/void SetBlockColor(const D3DXCOLOR col) { m_Info.col = col; }
	/* 大きさ	*/void SetBlockSize(const D3DXVECTOR3 size) { m_Info.size = size; }
	/* リセット	*/static void Reset(void);

	// -- 所得 ---------------------------------------------
	/* 位置		*/D3DXVECTOR3 GetBlockPos(void) { return m_Info.pos; }
	/* 向き		*/D3DXVECTOR3 GetBlockRot(void) { return m_Info.rot; }
	/* 色		*/D3DXCOLOR GetBlockColor(void) { return m_Info.col; }
	/* 大きさ	*/D3DXVECTOR3 GetBlockSize(void) { return m_Info.size; }
	/* 状態		*/STATE GetBlockState(void) { return m_Info.state; }
	/* 種類		*/int GetBlockType(void) { return m_Info.nModelID; }
	/* ID		*/int GetID(void) { return m_Info.nID; }

private:	

	// ***** 関数 *****
	/* 爆発判定		*/void BombCollsion(PRIO nPrio, TYPE nType, VECTOR vector,D3DXVECTOR3 pos);
	/* 当たり判定	*/D3DXVECTOR3 Collision(PRIO nPrio, TYPE nType, VECTOR vector, D3DXVECTOR3 pos);
	/* 爆弾			*/void Bomb(void);
	/* 木箱			*/void WoodenBox(void);
	/* 鉄箱			*/void IronBox(void);

	// ***** 変数 *****
	static int m_nNumAll;			// キューブの総数
	static D3DXVECTOR3 m_CameraRot;	// 向きの保存
	static float m_CameraHeigth;	// 高さの保存

	Info m_Info;					// ブロック情報
	static TypeInfo *m_TypeInfo;	// 種類情報

};
#endif