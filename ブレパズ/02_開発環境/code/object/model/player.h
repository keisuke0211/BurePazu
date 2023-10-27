//========================================
// 
// プレイヤー処理
// 
//========================================
// *** player.h ***
//========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../../main.h"
#include "motion_model.h"

//****************************************
// クラス
//****************************************
class CPlayer : public CMotionModel
{
public:

	// ***** 定義 *****
	static const int MAX_TEXTURE = 50;	// テクスチャの最大数

	static const int PLAYER_MAX = 1;	// プレイヤーの最大数
	static const float PLAYER_SPEED;	// プレイヤーの移動量

	// ***** 構造体 *****

	// プレイヤー情報
	struct Info
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 posOld;		// 位置(過去)
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 rotOld;		// 向き(過去)
		D3DXVECTOR3 moveRot;	// 移動向き
		D3DXVECTOR3 targetRot;	// 目標向き
		D3DXVECTOR3 move;		// 移動量
		D3DXCOLOR col;			// 頂点カラー
		float fWidth;			// 幅
		float fHeight;			// 高さ
		float fDepth;			// 奥行き
		bool bMove;				// 移動フラグ
		bool bMotion;			// モーションフラグ
		bool bAction;			// 行動フラグ
		bool bRotMove;			// 回転フラグ

	};

	// ***** 関数 *****
	CPlayer(int nPriority = PRIO_OBJECT);
	~CPlayer();

	/* メイン */

	// 生成
	static CPlayer *Create(D3DXVECTOR3 pos,float rotY);

	HRESULT Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

	/* 設定 */
	void PlayerSetPos(D3DXVECTOR3 pos) { m_Info.pos = m_Info.posOld = pos; }

	/* 取得 */
	D3DXVECTOR3 GetPos() { return m_Info.pos; }
	D3DXVECTOR3 GetPlayerMove() { return m_Info.move; }

private:
	// ***** 関数 *****
	/* 当たり判定			*/D3DXVECTOR3 Collision(PRIO nPrio, TYPE nType,VECTOR vector,D3DXVECTOR3 pos);

	/* 移動 */
	void MoveInput(DIRECTION drct);	// キーボードの移動処理
	void MovePos(float fMove);	// 移動
	void UpdatePos(void);		// 位置更新

	/* 入力 */
	void KeyInput(void);	// キー入力処理
	void BulletStop(void);	// 弾停止

	// ***** 変数 *****
	Info m_Info;			// プレイヤー情報
};
#endif