//========================================
// 
// エネミー処理
// 
//========================================
// *** enemy.h ***
//========================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "../../main.h"
#include "../objectX.h"

//****************************************
// クラス
//****************************************
class CEnemy : public CObjectX
{
public:

	// ***** 定義 *****
	static const int MAX_ENEMY = 30;	// 敵の最大数
	static const int STAND_TIME = 30;	// 待機時間の最大値
	static const int STOP_TIME = 90;	// 停止時間の最大値

	// ***** 列挙型 *****

	// 状態
	enum STATE
	{
		STATE_NORMAL,	// 通常
		STATE_STAND,	// 待機
		STATE_STOP,		// 停止
		STATE_DAMAGE,	// ダメージ
		STATE_MAX,
	};

	// ***** 構造体 *****

	// 敵情報
	struct Info
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 posOld;		// 位置(過去)
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 moveRot;	// 移動向き
		D3DXVECTOR3 targetRot;	// 目標向き
		D3DXVECTOR3 move;		// 移動量
		D3DCOLOR col;			// 頂点カラー
		int nType;				// 種類
		int nMove;				// 移動種類
		int nLife;				// 寿命
		int nLifeMax;			// 寿命の最大値
		STATE state;			// 状態
		STATE stateOld;			// 過去の状態
		int nCntState;			// 状態管理カウンター
		int nCntStop;			// 停止時間
		int nCntTime;			// 行動カウンター
		int nTimeMax;			// 行動時間の最大値
		int nStandTime;			// 待機時間
		bool bRotMove;			// 向きの推移フラグ
		bool bCube;				// キューブに当たったか
		bool bGravity;			// 重力
		int nID;				// ID
	};

	// ***** 関数 *****
	CEnemy(int nPriority = PRIO_OBJECT);
	virtual~CEnemy();

	/* メイン */

	// 生成
	static CEnemy *Create(int nType,int nMove,D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nCntTime);

	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);
	/* リセット	*/static void Reset(void);
	/* Hit処理	*/void HitLife(int nDamage);

	/* 設定	*/
	/* 状態設定		*/void SetState(STATE state);
	/* 全体の重力	*/static void SetGravity(bool bSet) { m_bGravity = bSet; }

	/* 取得 */
	/* 敵情報		*/Info GetInfo(void) { return m_Info; }
	/* 位置			*/D3DXVECTOR3 GetPos(void) { return m_Info.pos; }
	/* 過去位置		*/D3DXVECTOR3 GetPosOld(void) { return m_Info.posOld; }
	/* 全体の重力	*/static bool IsmGravity(void) { return m_bGravity; }
	/* 敵の総数		*/static int GetEnemyAll(void) { return m_nNumAll; }
	/* ID			*/int GetID(void) { return m_Info.nID; }

private:

	// ***** 構造体 *****

	// 配置情報
	typedef struct
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		int nType;			// 種類
		int nSpeed;			// 移動量
		int nStage;			// ステージID
		int nUnit;			// 部隊ID
		bool bSet;			// 配置したかどうか

	} SetInfo;

	// ***** 関数 *****
	/* 状態推移				*/void StateShift(void);
	/* 当たり判定(方向指定)	*/bool Collision(PRIO nPrio, TYPE nType, VECTOR vector, D3DXVECTOR3 pos);
	/* 当たり判定(全体)		*/void ModelCollsion(PRIO nPrio, TYPE nType, D3DXVECTOR3 pos);

	// ***** 変数 *****
	Info m_Info;			// 情報
	static SetInfo *pSet;	// 配置

	static int m_nNumAll;		// 敵の総数
	static bool m_bGravity;		// 全体の重力
};
#endif