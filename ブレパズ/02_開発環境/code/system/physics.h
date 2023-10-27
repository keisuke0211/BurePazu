//========================================
// 
// 物理関連 & 共通情報
// 
//========================================
//  *** physics.h ***
//========================================
#ifndef _PHYSICS_H_
#define _PHYSICS_H_
#include "../main.h"

//****************************************
// クラス
//****************************************
class CPhysics
{
public:

	// ***** 列挙型 *****

	// モデル種類
	enum MODEL
	{
		MODEL_GRASS_00 = 0,	// 草 1×1×1
		MODEL_GRASS_01,		// 草 3×1×3
		MODEL_GRASS_02,		// 草 5×1×5
		MODEL_STONE,		// 石 1×1×1
		MODEL_WOOD_BOX,		// 木箱
		MODEL_IRON_BOX,		// 鉄箱
		MODEL_BOMB,			// 爆弾
		MODEL_FENCE,		// 柵・横
		MODEL_FENCE1,		// 柵・縦
		MODEL_BULLET,		// 弾
		MODEL_PLAYER_00,	// プレイヤー 体
		MODEL_PLAYER_01,	// プレイヤー 頭
		MODEL_PLAYER_02,	// プレイヤー 左手
		MODEL_PLAYER_03,	// プレイヤー 右手
		MODEL_PLAYER_04,	// プレイヤー 左足
		MODEL_PLAYER_05,	// プレイヤー 右足
		MODEL_ENEMY_00,		// 仮エネミー
		MODEL_EFFECT_00,	// エフェクト　粒子
		MODEL_MAX
	};

	// ベクトルの種類
	enum VECTOR
	{
		VECTOR_X,		// X方向
		VECTOR_Y,		// Y方向
		VECTOR_Z,		// Z方向
		VECTOR_NONE,	// 無し
		VECTOR_MAX
	};

	// 4方向
	enum DIRECTION
	{
		DIRECTION_UP,			// 上
		DIRECTION_DOWN,			// 下
		DIRECTION_LEFT,			// 左
		DIRECTION_RIGHT,		// 右
		DIRECTION_BACK,			// 奥
		DIRECTION_FRONT,		// 手前

		DIRECTION_LEFT_BACK,	// 左奥
		DIRECTION_LEFT_FRONT,	// 左手前
		DIRECTION_RIGHT_BACK,	// 右奥
		DIRECTION_RIGHT_FRONT,	// 右手前
		DIRECTION_MAX,
	};

	enum ROT
	{
		ROT_UP,		// 上
		ROT_RIGHT,	// 左
		ROT_DOWN,	// 下
		ROT_LEFT,	// 右
		ROT_MAX
	};

	// ***** 関数 *****
	CPhysics();		// コンストラクタ
	virtual~CPhysics();	// デストラクタ

	/* int型の値の制御			*/void IntControl(int *pData, int nMax, int nMin);
	/* int型の値のループ制御	*/void IntLoopControl(int *pData, int nMax, int nMin);
	/* float型の値の制御		*/void FloatControl(float *pData, float fMax, float fMin);
	/* float型の値のループ制御	*/void FloatLoopControl(float *pData, float fMax, float fMin);

	/* 2点の角度を求める		*/float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
	/* 向きを制御				*/void RotControl(D3DXVECTOR3 *pRot);

	/* 角度の差を求める			*/float AngleDifference(float fAngle, float fTargetAngle);
	/* 角度の制御				*/void ControlAngle(float *pAngle);
	/* 位置の差を求める			*/float PosDifference(float fPos, float fTargetPos);
	/* 距離を求める				*/D3DXVECTOR3 Length(D3DXVECTOR3 pos, D3DXVECTOR3 PairPos);
	/* 当たり判定(全方向)		*/bool Collsion(D3DXVECTOR3 pos, D3DXVECTOR3 PairPos, D3DXVECTOR3 size, D3DXVECTOR3	PairSize);
	/* 当たり判定(ｘ)			*/bool CollsionX(D3DXVECTOR3 pos, D3DXVECTOR3 PairPos, D3DXVECTOR3 size, D3DXVECTOR3 PairSize);
	/* 当たり判定(ｙ)			*/bool CollsionY(D3DXVECTOR3 pos, D3DXVECTOR3 PairPos, D3DXVECTOR3 size, D3DXVECTOR3 PairSize);
	/* 当たり判定(ｚ)			*/bool CollsionZ(D3DXVECTOR3 pos, D3DXVECTOR3 PairPos, D3DXVECTOR3 size, D3DXVECTOR3 PairSize);
	/* 当たり判定(指定方向)		*/bool CollsionDirection(D3DXVECTOR3 pos, D3DXVECTOR3 PairPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 PairPosOld, D3DXVECTOR3 size, D3DXVECTOR3 PairSize, DIRECTION nDirection);

private:

};
#endif