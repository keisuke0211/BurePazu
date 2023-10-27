//========================================
// 
// カメラ処理
// 
//========================================
// *** camera.cpp ***
//========================================
#include "camera.h"
#include "../manager.h"
#include "renderer.h"
#include "input.h"
#include "../scene/pause.h"
#include "../scene/title.h"
#include "../scene/game.h"
#include "../object/model/block.h"

//****************************************
// マクロ定義
//****************************************
#define CAMERA_ROT_FORCE_BY_CURSOR	(D3DXVECTOR3(0.0005f,0.00075f,0.0f))	// カーソルの回転力
#define CAMERA_ROT_FORCE_BY_STICK	(D3DXVECTOR3(0.004f,-0.006f,0.0f))		// スティックの回転力
#define CAMERA_SPIN_DAMP			(0.9f)									// 回転の減少値
#define CAMERA_SPEED				(50)									// 移動速度（X・Z座標）
#define ROT_DIAMETER	(0.15f)												// 回転倍率

//========================================
// コンストラクタ
//========================================
CCamera::CCamera()
{
	/* カメラ情報 */
	m_Info.posR = INIT_D3DXVECTOR3;		// 現在の視点
	m_Info.posV = INIT_D3DXVECTOR3;		// 現在の注視点
	m_Info.posOldR = INIT_D3DXVECTOR3;	// 前回の視点R3;
	m_Info.posOldV = INIT_D3DXVECTOR3;	// 前回の注視点R3;
	m_Info.vecU = INIT_VEC;				// 上方向ベクトル
	m_Info.rot = INIT_D3DXVECTOR3;		// 向き
	m_Info.targetRot = INIT_D3DXVECTOR3;// 目標向き
	m_Info.spin = INIT_D3DXVECTOR3;		// 回転量
	m_Info.fDistance = INIT_FLOAT;		// 距離
	m_Info.fTargetDistance = INIT_FLOAT;// 目標距離
	m_Info.fHeight = INIT_FLOAT;		// 高さ
	m_Info.fTargetHeight = INIT_FLOAT;	// 目標高さ
	m_Info.fVerticalMove = INIT_FLOAT;	// 縦の移動量
	m_Info.nScreen = SCREEN_NONE;		// 投影モード
	m_Info.bRotMove = false;
	m_Info.nRotType = 0;

	/* 保存情報 */
	m_Save.posV = INIT_D3DXVECTOR3;		// 視点
	m_Save.posR = INIT_D3DXVECTOR3;		// 注視点
	m_Save.rot = INIT_D3DXVECTOR3;		// 向き
	m_Save.spin = INIT_D3DXVECTOR3;		// 回転量
	m_Save.fVerticalMove = INIT_FLOAT;	// 縦の移動量
	m_Save.fHeight = INIT_FLOAT;		// 高さ
	m_Save.fDistance = INIT_FLOAT;		// 距離
}

//========================================
// デストラクタ
//========================================
CCamera::~CCamera()
{

}

//========================================
// 初期化
//========================================
HRESULT CCamera::Init(void)
{
	/* カメラ情報 */
	m_Info.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの位置
	m_Info.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 見る場所
	m_Info.posOldR = INIT_D3DXVECTOR3;				// 前回の視点R3;
	m_Info.posOldV = INIT_D3DXVECTOR3;				// 前回の注視点R3;
	m_Info.vecU = INIT_VEC;							// 上方向ベクトル
	m_Info.rot = INIT_D3DXVECTOR3;					// 向き
	m_Info.targetRot = INIT_D3DXVECTOR3;			// 目標向き
	m_Info.spin = INIT_D3DXVECTOR3;					// 回転力
	m_Info.fDistance = 300.0f;						// 距離
	m_Info.fTargetDistance = 300.0f;				// 目標距離
	m_Info.fHeight = 0.05f;							// 高さ
	m_Info.fTargetHeight = 0.05f;					// 目標高さ
	m_Info.fVerticalMove = INIT_FLOAT;				// 縦の移動量
	m_Info.nScreen = SCREEN_3D;						// 投影モード
	m_Info.bRotMove = false;
	m_Info.nRotType = 0;

	return S_OK;
}

//========================================
// 終了
//========================================
void CCamera::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CCamera::Update(void)
{
	// --- 取得---------------------------------
	CKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// キーボード
	CMouse *pInputMouse = CManager::GetInputMouse();			// マウス
	CJoypad *pInputJoypad = CManager::GetInputJoypad();		// ジョイパット

	// 過去の位置を代入
	m_Info.posOldV = m_Info.posV;
	m_Info.posOldR = m_Info.posR;

	// 視点移動
	{
		bool bPause = CPause::IsPause();
		bool bClear = CGame::IsEnd();

		if (!bPause)
		{
			bool bStart = CTitle::IsStart();
			if (bStart && !bClear)
			{
				if (pInputMouse->GetTrigger(CMouse::MOUSE_RIGHT))
				{// マウスの右ボタンが押されている間
					m_Info.nRotType++;
				}
				else if (pInputJoypad->GetTrigger(CJoypad::JOYKEY_LEFT_SHOULDER) || pInputJoypad->GetStick().aAngleTrigger[CJoypad::STICK_TYPE_RIGHT][CJoypad::STICK_ANGLE_LEFT])
				{
					m_Info.nRotType++;
				}
				else if (pInputJoypad->GetTrigger(CJoypad::JOYKEY_RIGHT_SHOULDER) || pInputJoypad->GetStick().aAngleTrigger[CJoypad::STICK_TYPE_RIGHT][CJoypad::STICK_ANGLE_RIGHT])
				{
					m_Info.nRotType--;
				}

				IntLoopControl(&m_Info.nRotType, ROT_MAX, 0);

				switch (m_Info.nRotType)
				{
				case ROT_UP:
					m_Info.spin.y = 0.0f;
					break;
				case ROT_RIGHT:
					m_Info.spin.y = -1.57f;
					break;
				case ROT_DOWN:
					m_Info.spin.y = 3.14f;
					break;
				case ROT_LEFT:
					m_Info.spin.y = 1.57f;
					break;
				}
			}
		}
	}

	if (m_Info.bRotMove)
	{
		m_Info.rot.y += m_Info.spin.y;					// 向きに移動向きを代入
		m_Info.spin *= CAMERA_SPIN_DAMP;				// 回転量を減衰
	}
	else
	{
		m_Info.targetRot = m_Info.spin;				// 目標向きに移動向きを代入
	}
	m_Info.fTargetHeight = m_Info.fVerticalMove;	// 目標高さに移動量を代入

	// 向きを制御
	RotControl(&m_Info.rot);

	// 高さを制御
	FloatControl(&m_Info.fHeight, D3DX_PI * 0.30f, D3DX_PI * 0.01f);

	if (!m_Info.bRotMove)
	{
		// 高さを目標に向けて推移する
		m_Info.rot.y += AngleDifference(m_Info.rot.y, m_Info.targetRot.y) * ROT_DIAMETER;
	}

	// 高さを目標に向けて推移する
	m_Info.fHeight += AngleDifference(m_Info.fHeight, m_Info.fTargetHeight) * ROT_DIAMETER;

	// 距離を目標距離に向けて推移する
	m_Info.fDistance += AngleDifference(m_Info.fDistance, m_Info.fTargetDistance) * ROT_DIAMETER;

	if (m_Info.bRotMove)
	{
		m_Info.bRotMove = false;
	}
}

//========================================
// 設定
//========================================
void CCamera::SetCamera(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_Info.mtxProjection);

	//プロジェクションマトリックスを作成
	switch (m_Info.nScreen)
	{
	case SCREEN_2D: {	// 平行投影
		D3DXMatrixOrthoLH(&m_Info.mtxProjection,
			SCREEN_WIDTH / 2,						/* 画面の幅 */
			SCREEN_HEIGHT / 2,						/* 画面の高さ */
			1.0f,									/* Z値の最小値 */
			1500.0f);								/* Z値の最大値 */
	}
		break;

	case SCREEN_3D: {	// 透視投影
		D3DXMatrixPerspectiveFovLH(&m_Info.mtxProjection,
			D3DXToRadian(45.0f),							/* 視野角 */
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		/*画面のアスペクト比*/
			10.0f,											/*Z値の最小値*/
			2500.0f);										/*Z値の最大値*/
	}
		break;
	}

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_Info.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_Info.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_Info.mtxView,
		&m_Info.posV,		/*視点*/
		&m_Info.posR,		/*注視点*/
		&m_Info.vecU);		/*上方向ベクトル*/

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_Info.mtxView);
}

//========================================
// カメラの画面設定
//========================================
void CCamera::SetScreen(void)
{
	// 投影モードの切り替え

	switch (m_Info.nScreen)
	{
	case SCREEN_2D: {	// 平行投影
		
		// 保存情報を代入する
		m_Info.posV = m_Save.posV;						// 視点
		m_Info.posR = m_Save.posR;						// 注視点
		m_Info.rot  = m_Save.rot;						// 向き
		m_Info.spin = m_Save.spin;						// 回転量
		m_Info.fTargetHeight = m_Save.fHeight;			// 高さ
		m_Info.fDistance = m_Save.fDistance;			// 距離
		m_Info.fVerticalMove = m_Save.fHeight;			// 縦の移動量

		m_Info.nScreen = SCREEN_3D;
	}
	 break;

	case SCREEN_3D: {	// 透視投影
		
		// 情報を保存する
		m_Save.posV = m_Info.posV;						// 視点
		m_Save.posR = m_Info.posR;						// 注視点
		m_Save.rot	= m_Info.rot;						// 向き
		m_Save.spin	= m_Info.spin;						// 回転量
		m_Save.fHeight = m_Info.fHeight;				// 高さ
		m_Save.fDistance = m_Info.fDistance;			// 距離
		m_Save.fVerticalMove = m_Info.fVerticalMove;	// 縦の移動量

		// 向きを初期化
		m_Info.rot.y = 0.0f;

		m_Info.fVerticalMove = 0.05f;

		m_Info.nScreen = SCREEN_2D;
	}
	 break;
	}
}

//========================================
// カメラの回転処理
//========================================
void CCamera::AxisRotationCamera(DIRECTION drct, float fRot, int nIdx)
{
	// 方向に応じて向きの移動量を更新
	switch (drct)
	{
	case DIRECTION_UP:m_Info.fVerticalMove += fRot; break;
	case DIRECTION_DOWN:m_Info.fVerticalMove -= fRot; break;
	case DIRECTION_LEFT:m_Info.spin.y -= fRot; break;
	case DIRECTION_RIGHT:m_Info.spin.y += fRot; break;
	}
}

//========================================
// カメラの視点設定
//========================================
void CCamera::SetPosV(D3DXVECTOR3 pos, int nIdx)
{
	// 視点の位置を代入
	m_Info.posV = pos;

	// 注視点の位置を設定
	m_Info.posR.x = m_Info.posV.x + (sinf(m_Info.rot.y) * (m_Info.fDistance * (1.0f - fabsf(m_Info.fHeight))));
	m_Info.posR.y = m_Info.posV.y + (m_Info.fDistance * m_Info.fHeight);
	m_Info.posR.z = m_Info.posV.z + (cosf(m_Info.rot.y) * (m_Info.fDistance * (1.0f - fabsf(m_Info.fHeight))));
}

//========================================
// カメラの注視点設定
//========================================
void CCamera::SetPosR(D3DXVECTOR3 pos, int nIdx)
{
	// 注視点の位置を代入
	m_Info.posR = pos;

	// 視点の位置を設定
	m_Info.posV.x = m_Info.posR.x + (sinf(m_Info.rot.y) * (m_Info.fDistance * (1.0f - fabsf(m_Info.fHeight))));
	m_Info.posV.y = m_Info.posR.y + (m_Info.fDistance * m_Info.fHeight);
	m_Info.posV.z = m_Info.posR.z + (cosf(m_Info.rot.y) * (m_Info.fDistance * (1.0f - fabsf(m_Info.fHeight))));
}

//========================================
// カメラの向き設定
//========================================
void CCamera::SetRot(D3DXVECTOR3 rot, int nIdx)
{
	m_Info.rot = rot;
	m_Info.targetRot = rot;
	m_Info.bRotMove = true;
}

//========================================
// カメラの高さ
//========================================
void CCamera::SetHeigth(float Heigth)
{
	m_Info.fVerticalMove = Heigth;
}

//========================================
// カメラの距離
//========================================
void CCamera::SetDistance(float fDistance)
{
	m_Info.fTargetDistance = fDistance;
}
