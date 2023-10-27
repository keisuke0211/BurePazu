//========================================
// 
// カメラ処理
// 
//========================================
// *** camera.h ***
//========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../main.h"
#include "physics.h"

//****************************************
// クラス
//****************************************
class CCamera : public CPhysics
{
public:

	// ***** 列挙型 *****

	// カメラの投影種類
	enum SCREEN
	{
		SCREEN_NONE = 0,	// 無し
		SCREEN_2D,			// 平行投影
		SCREEN_3D,			// 透視投影
		SCREEN_MAX
	};

	// ***** 構造体 *****

	// カメラ情報
	struct Info
	{
		D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX mtxView;			// ビューマトリックス

		D3DXVECTOR3 posV;			// 現在の視点
		D3DXVECTOR3 posR;			// 現在の注視点
		D3DXVECTOR3 posOldV;		// 前回の視点
		D3DXVECTOR3 posOldR;		// 前回の注視点
		D3DXVECTOR3 vecU;			// 上方向ベクトル
		D3DXVECTOR3 rot;			// 向き
		D3DXVECTOR3 targetRot;		// 目標向き
		D3DXVECTOR3 spin;			// 回転量

		int nScreen;				// 投影モード

		float fDistance;			// 距離
		float fTargetDistance;		// 目標距離
		float fHeight;				// 高さ
		float fTargetHeight;		// 目標高さ
		float fVerticalMove;		// 縦の移動量
		bool bRotMove;				// 向きの移動切り替えフラグ

		int nRotType;				// 向き種類
	};

	// ***** 関数 *****
	CCamera();	// コンストラクタ
	~CCamera();	// デストラクタ

	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

	/* 設定 */
	void SetCamera(void);							// カメラ
	void SetScreen(void);							// 画面設定
	void SetPosV(D3DXVECTOR3 pos, int nIdx = 0);	// 視点
	void SetPosR(D3DXVECTOR3 pos, int nIdx = 0);	// 注視点
	void SetRot(D3DXVECTOR3 rot, int nIdx = 0);		// 向き
	void SetHeigth(float Heigth);					// 高さ
	void SetDistance(float fDistance);				// 距離

	/* 取得 */
	Info GetInfo() { return m_Info; }			// カメラの情報

private:

	// ***** 構造体 *****

	// カメラの保存情報
	typedef struct
	{
		D3DXVECTOR3 posV;			// 視点
		D3DXVECTOR3 posR;			// 注視点
		D3DXVECTOR3 rot;			// 向き
		D3DXVECTOR3 spin;			// 回転量

		float fDistance;			// 距離
		float fHeight;				// 高さ
		float fVerticalMove;		// 縦の移動量
	} Save;

	// ***** 関数 *****
	void AxisRotationCamera(DIRECTION drct, float fRot, int nIdx = 0); //回転処理

	// ***** 変数 *****
	Info m_Info;		// カメラ
	Save m_Save;		// 保存 

};

#endif