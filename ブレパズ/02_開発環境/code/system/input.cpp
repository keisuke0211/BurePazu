//========================================
// 
// 入力処理
// 
//========================================
// *** input.cpp ***
//========================================
#include "input.h"

// 静的メンバ変数
LPDIRECTINPUT8 CInput::m_pInput = NULL;
LPDIRECTINPUT8 CInput::m_pMouswInput = NULL;

//========================================
// コンストラクタ
//========================================
CInput::CInput()
{
	
}

//========================================
// デストラクタ
//========================================
CInput::~CInput()
{

}

//========================================
// 初期化
//========================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// キーボード
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	// マウス
	if (m_pMouswInput == NULL)
	{
		//DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pMouswInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//========================================
// 終了
//========================================
void CInput::Uninit(void)
{

	return;
	// キーボード
	{
		// 入力デバイスの放棄
		if (m_pDevice != NULL)
		{
			m_pDevice->Unacquire();
			m_pDevice->Release();
			m_pDevice = NULL;
		}

		// DirectInputオブジェクトの破壊
		if (m_pInput != NULL)
		{
			m_pInput->Release();
			m_pInput = NULL;
		}
	}

	// マウス
	{
		// 入力デバイスの放棄
		if (m_pDeviceMouse != NULL)
		{
			m_pDeviceMouse->Unacquire();
			m_pDeviceMouse->Release();
			m_pDeviceMouse = NULL;
		}

		// DirectInputオブジェクトの破壊
		if (m_pMouswInput != NULL)
		{
			m_pMouswInput->Release();
			m_pMouswInput = NULL;
		}
	}
}

//========================================
//----------------------------------------
// キーボード クラス
//----------------------------------------
//========================================

// コンストラクタ
CKeyboard::CKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		// キー情報のクリア
		m_aKeyState[nCntKey] = false;
		m_aKeyStateTrigger[nCntKey] = false;
		m_aKeyStateRelease[nCntKey] = false;
		m_aKeyStateRepeat[nCntKey] = false;
		m_aKeyCurrentTime[nCntKey] = false;
		m_aKeyExecLastTime[nCntKey] = false;
	}
}

// デストラクタ
CKeyboard::~CKeyboard()
{

}

//========================================
// 初期化
//========================================
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// マウスカーソルを表示
	ShowCursor(false);

	//　デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//========================================
// 終了
//========================================
void CKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//========================================
// 更新
//========================================
void CKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// キーボードのトリガー情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

			// キーボードのリリース情報を保存
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&~aKeyState[nCntKey];

			// 現在の時間を取得
			m_aKeyCurrentTime[nCntKey] = timeGetTime();

			if (aKeyState[nCntKey] && ((m_aKeyCurrentTime[nCntKey] - m_aKeyExecLastTime[nCntKey]) > REPEATE_INTERVAL))
			{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
				// 最後に真を返した時間を保存
				m_aKeyExecLastTime[nCntKey] = m_aKeyCurrentTime[nCntKey];

				// キーボードのリピート情報を保存
				m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}
			else
			{
				// キーボードのリピート情報を保存
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			// キーボードのプレス情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{// 入力デバイスからデータを取得できなかった時、
	 // キーボードへのアクセス権を取得
		m_pDevice->Acquire();
	}
}

//========================================
// プレスの取得
//========================================
bool CKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80);
}
//========================================
// トリガー情報を取得
//========================================
bool CKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80);
}

//========================================
// リピート情報を取得 -
//========================================
bool CKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80);
}

//========================================
// リリース情報を取得
//========================================
bool CKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80);
}

//========================================
//----------------------------------------
// マウス クラス
//----------------------------------------
//========================================

// コンストラクタ
CMouse::CMouse()
{
	for (int nCntKey = 0; nCntKey < MOUSE_KEY_MAX; nCntKey++)
	{
		// キー情報のクリア
		m_aMouseState.rgbButtons[nCntKey] = false;
		m_aMouseStateTrigger.rgbButtons[nCntKey] = false;
		m_aMouseStateRelease.rgbButtons[nCntKey] = false;
		m_aMouseStateRepeat.rgbButtons[nCntKey] = false;
		m_aMouseCurrentTime.rgbButtons[nCntKey] = false;
		m_aMouseExecLastTime.rgbButtons[nCntKey] = false;
	}
	g_MousePosTemp = INIT_D3DXVECTOR3;	// 位置保存
	m_MouseMove = INIT_D3DXVECTOR3;		// 移動量
}

// デストラクタ
CMouse::~CMouse()
{

}

//========================================
// 初期化
//========================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイスの生成
	if (FAILED(m_pMouswInput->CreateDevice(GUID_SysMouse, &m_pDeviceMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDeviceMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDeviceMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//ウィンドウハンドルの保管
	m_hMouseWnd = hWnd;

	//キーボードへのアクセス権を獲得
	m_pDeviceMouse->Acquire();

	return S_OK;
}

//========================================
// 終了
//========================================
void CMouse::Uninit(void)
{
	CInput::Uninit();
}

//========================================
// 更新
//========================================
void CMouse::Update(void)
{
	DIMOUSESTATE2 aKeyState;	// 入力情報

	if (SUCCEEDED(m_pDeviceMouse->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{// 入力デバイスからデータを取得
		for (int nCntKey = 0; nCntKey < MOUSE_KEY_MAX; nCntKey++)
		{
			WORD wButtons = m_aMouseState.rgbButtons[nCntKey];
			WORD wOldButtons = aKeyState.rgbButtons[nCntKey];
			m_aMouseStateTrigger.rgbButtons[nCntKey] = ~wButtons & wOldButtons; // トリガー情報を保存
			m_aMouseStateRelease.rgbButtons[nCntKey] = wButtons & ~wOldButtons; // リリース情報を保存

			// 現在の時間を取得
			m_aMouseCurrentTime.rgbButtons[nCntKey] = timeGetTime();

			if (aKeyState.rgbButtons[nCntKey] && ((m_aMouseCurrentTime.rgbButtons[nCntKey] - m_aMouseExecLastTime.rgbButtons[nCntKey]) > REPEATE_INTERVAL))
			{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
			 // 最後に真を返した時間を保存
				m_aMouseExecLastTime.rgbButtons[nCntKey] = m_aMouseCurrentTime.rgbButtons[nCntKey];

				// マウスのリピート情報を保存
				m_aMouseStateRepeat.rgbButtons[nCntKey] = aKeyState.rgbButtons[nCntKey];
			}
			else
			{// キーが入力されていない、もしくは現在の時間と最後に真を返した時間の差がリピートの間隔を越えていない時、
			 // マウスのリピート情報を保存
				m_aMouseStateRepeat.rgbButtons[nCntKey] = 0;
			}
		}
		m_aMouseState = aKeyState;		// プレス情報を保存
	}
	else
	{
		m_pDeviceMouse->Acquire();			// アクセス権を獲得
	}

	// カーソルの移動量を設定
	m_MouseMove = GetPos() - g_MousePosTemp;

	// カーソルの位置保存
	g_MousePosTemp = GetPos();
}

//========================================
// プレスの取得
//========================================
bool CMouse::GetPress(MOUSE Mouse)
{
	return (m_aMouseState.rgbButtons[Mouse] & 0x80);
}

//========================================
// トリガー情報を取得
//========================================
bool CMouse::GetTrigger(MOUSE Mouse)
{
	return (m_aMouseStateTrigger.rgbButtons[Mouse] & 0x80);
}

//========================================
// リピート情報を取得 -
//========================================
bool CMouse::GetRepeat(MOUSE Mouse)
{
	return (m_aMouseStateRepeat.rgbButtons[Mouse] & 0x80);
}

//========================================
// リリース情報を取得
//========================================
bool CMouse::GetRelease(MOUSE Mouse)
{
	return (m_aMouseStateRelease.rgbButtons[Mouse] & 0x80);
}

//========================================
//　マウスポインターの位置
//========================================
D3DXVECTOR3 CMouse::GetPos(void)
{
	POINT MousePos;		// カーソル用

	//画面上のマウスポインターの位置
	GetCursorPos(&MousePos);

	//ウィンドウ上のマウスポインターの位置
	ScreenToClient(m_hMouseWnd, &MousePos);

	return D3DXVECTOR3((float)MousePos.x, (float)MousePos.y, 0.0f);
}

//========================================
// マウスのホイールの動き感知
//========================================
int CMouse::GetWheel(void)
{
	return (int)m_aMouseState.lZ;
}

//========================================
//----------------------------------------
// ジョイパット クラス
//----------------------------------------
//========================================

// コンストラクタ
CJoypad::CJoypad()
{
	for (int nCntPat = 0; nCntPat < MAX_PAT; nCntPat++)
	{
		m_JoyKeyState[nCntPat].Gamepad.wButtons = false;
		m_JoyKeyStateTrigger[nCntPat].Gamepad.wButtons = false;
		m_JoyKeyStateRelease[nCntPat].Gamepad.wButtons = false;
		m_aJoyKeyStateRepeat[nCntPat].Gamepad.wButtons = false;
		m_aJoyKeyCurrentTime[nCntPat].Gamepad.wButtons = false;
		m_aJoyKeyExecLastTime[nCntPat].Gamepad.wButtons = false;
	}
}

// デストラクタ
CJoypad::~CJoypad()
{

}

//========================================
// 初期化
//========================================
HRESULT CJoypad::Init(void)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < MAX_PAT; nCnt++)
	{
		//メモリーのクリア
		memset(&m_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		//ジョイパッドの振動制御の０クリア
		ZeroMemory(&m_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//振動制御用の初期化
		m_nStrength[nCnt] = 0;
		m_nTime[nCnt] = 0;
	}
	return S_OK;
}

//========================================
// 終了
//========================================
void CJoypad::Uninit(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//========================================
// 更新
//========================================
void CJoypad::Update(void)
{
	XINPUT_STATE JoyKeyState[MAX_PAT];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < MAX_PAT; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			WORD wButtons = m_JoyKeyState[nCnt].Gamepad.wButtons;
			WORD wOldButtons = JoyKeyState[nCnt].Gamepad.wButtons;
			m_JoyKeyStateTrigger[nCnt].Gamepad.wButtons = ~wButtons & wOldButtons; //トリガー情報を保存
			m_JoyKeyStateRelease[nCnt].Gamepad.wButtons = wButtons & ~wOldButtons; //リリース情報を保存
			m_JoyKeyState[nCnt] = JoyKeyState[nCnt];  //プレス処理

			// 現在の時間を取得
			m_aJoyKeyCurrentTime[nCnt].Gamepad.wButtons = timeGetTime();

			if (JoyKeyState[nCnt].Gamepad.wButtons && ((m_aJoyKeyCurrentTime[nCnt].Gamepad.wButtons - m_aJoyKeyExecLastTime[nCnt].Gamepad.wButtons) > REPEATE_INTERVAL))
			{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
			 // 最後に真を返した時間を保存
				m_aJoyKeyExecLastTime[nCnt].Gamepad.wButtons = m_aJoyKeyCurrentTime[nCnt].Gamepad.wButtons;

				// リピート情報を保存
				m_aJoyKeyStateRepeat[nCnt] = JoyKeyState[nCnt];
			}
			else
			{
				// リピート情報を保存
				m_aJoyKeyStateRepeat[nCnt].Gamepad.wButtons = 0;
			}

		}

		// 振動
		m_JoyMoter[nCnt].wLeftMotorSpeed = m_nStrength[nCnt];	// 左
		m_JoyMoter[nCnt].wRightMotorSpeed = m_nStrength[nCnt];	// 右
		XInputSetState(nCnt, &m_JoyMoter[nCnt]);				// 振動情報を送る

		// 振動時間の減算
		if (--m_nTime[nCnt] >= 0)
		{
			m_nStrength[nCnt] = 0;
			m_nTime[nCnt] = 0;
		}
	}
}

//========================================
// プレス情報
//========================================
bool CJoypad::GetPress(JOYKEY Key, int nPatNum)
{
	return (m_JoyKeyState[nPatNum].Gamepad.wButtons & (0x01 << Key));
}

//========================================
// トリガー情報
//========================================
bool CJoypad::GetTrigger(JOYKEY Key, int nPatNum)
{
	return (m_JoyKeyStateTrigger[nPatNum].Gamepad.wButtons & (0x01 << Key));
}

//========================================
// リリース情報
//========================================
bool CJoypad::GetRelese(JOYKEY Key, int nPatNum)
{
	return (m_JoyKeyStateRelease[nPatNum].Gamepad.wButtons & (0x01 << Key));
}

//========================================
// リピート情報
//========================================
bool CJoypad::GetRepeat(JOYKEY Key, int nPatNum)
{
	return (m_aJoyKeyStateRepeat[nPatNum].Gamepad.wButtons & (0x01 << Key));
}

//========================================
// トリガーペダル処理
//========================================
int CJoypad::GetTriggerPedal(JOYKEY Key, int nPatNum)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = m_JoyKeyState[nPatNum].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = m_JoyKeyState[nPatNum].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//========================================
// コントローラーの振動制御
//========================================
void CJoypad::Vibration(int nTime, WORD nStrength, int nPatNum)
{
	m_nTime[nPatNum] = nTime;			//振動持続時間
	m_nStrength[nPatNum] = nStrength;	//振動の強さ
}

//========================================
// スティックの入力情報を取得
//========================================
void CJoypad::UpdateStick(void)
{
	for (int nPatNum = 0; nPatNum < MAX_PAT; nPatNum++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nPatNum, &m_xInput) == ERROR_SUCCESS)
		{
			for (int nStick = 0; nStick < STICK_TYPE_MAX; nStick++)
			{
				float X;		// スティックのX軸
				float Y;		// スティックのY軸

				// 種類に応じたスティックの軸の値を取得
				switch (nStick)
				{
					//========== *** 左スティック ***
				case STICK_TYPE_LEFT:
					X = (GetXInputState())->Gamepad.sThumbLX;
					Y = (GetXInputState())->Gamepad.sThumbLY;
					break;
					//========== *** 右スティック ***
				case STICK_TYPE_RIGHT:
					X = (GetXInputState())->Gamepad.sThumbRX;
					Y = (GetXInputState())->Gamepad.sThumbRY;
					break;
				}

				// 角度を取得
				m_stick[nPatNum].aAngle[nStick] = FindAngle(D3DXVECTOR3(X, Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * -1;

				// スティックの倒し具合を取得
				m_stick[nPatNum].aTplDiameter[nStick] = fabsf(X);

				if (m_stick[nPatNum].aTplDiameter[nStick] < fabsf(Y))
				{
					m_stick[nPatNum].aTplDiameter[nStick] = fabsf(Y);
				}
				m_stick[nPatNum].aTplDiameter[nStick] /= 32768.0f;


				// 方向入力フラグを初期化
				for (int nCntAngle = 0; nCntAngle < STICK_ANGLE_MAX; nCntAngle++)
				{
					m_bAngle[nPatNum][nStick][nCntAngle] = false;
				}

				if (m_stick[nPatNum].aTplDiameter[nStick] > 0.5f)
				{// スティックが倒されている時、

					if ((m_stick[nPatNum].aAngle[nStick] < D3DX_PI * -0.75)
						|| (m_stick[nPatNum].aAngle[nStick] > D3DX_PI * 0.75))
					{// 角度が四分割で上に位置する時、上フラグを真にする
						m_bAngle[nPatNum][nStick][STICK_ANGLE_UP] = true;
					}
					else if ((m_stick[nPatNum].aAngle[nStick] > D3DX_PI * -0.25)
						&& (m_stick[nPatNum].aAngle[nStick] < D3DX_PI * 0.25))
					{// 角度が四分割で下に位置する時、下フラグを真にする
						m_bAngle[nPatNum][nStick][STICK_ANGLE_DOWN] = true;
					}
					else if ((m_stick[nPatNum].aAngle[nStick] > D3DX_PI * -0.75)
						&& (m_stick[nPatNum].aAngle[nStick] < D3DX_PI * -0.25))
					{// 角度が四分割で左に位置する時、左フラグを真にする
						m_bAngle[nPatNum][nStick][STICK_ANGLE_LEFT] = true;
					}
					else if ((m_stick[nPatNum].aAngle[nStick] > D3DX_PI * 0.25)
						&& (m_stick[nPatNum].aAngle[nStick] < D3DX_PI * 0.75))
					{// 角度が四分割で右に位置する時、右フラグを真にする
						m_bAngle[nPatNum][nStick][STICK_ANGLE_RIGHT] = true;
					}
				}

				// 角度に応じた入力処理
				for (int nAngle = 0; nAngle < STICK_ANGLE_MAX; nAngle++)
				{
					// スティックのトリガー情報を保存
					m_stick[nPatNum].aAngleTrigger[nStick][nAngle] = (m_stick[nPatNum].aAnglePress[nStick][nAngle] ^ m_bAngle[nPatNum][nStick][nAngle])&m_bAngle[nPatNum][nStick][nAngle];

					// スティックのリリース情報を保存
					m_stick[nPatNum].aAngleRelease[nStick][nAngle] = (m_stick[nPatNum].aAnglePress[nStick][nAngle] ^ m_bAngle[nPatNum][nStick][nAngle])&~m_bAngle[nPatNum][nStick][nAngle];

					// 現在の時間を取得
					m_aStickCurrentTime[nPatNum][nStick][nAngle] = timeGetTime();

					if (m_bAngle[nPatNum][nStick][nAngle] && ((m_aStickCurrentTime[nPatNum][nStick][nAngle] - m_aStickExecLastTime[nPatNum][nStick][nAngle]) > REPEATE_INTERVAL))
					{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
					 // 最後に真を返した時間を保存
						m_aStickExecLastTime[nPatNum][nStick][nAngle] = m_aStickCurrentTime[nPatNum][nStick][nAngle];

						// スティックのリピート情報を保存
						m_stick[nPatNum].aAngleRepeat[nStick][nAngle] = m_bAngle[nPatNum][nStick][nAngle];
					}
					else
					{
						// スティックのリピート情報を保存
						m_stick[nPatNum].aAngleRepeat[nStick][nAngle] = 0;
					}

					// スティックのプレス情報を保存
					m_stick[nPatNum].aAnglePress[nStick][nAngle] = m_bAngle[nPatNum][nStick][nAngle];
				}
			}
		}
	}
}

//========================================
// スティックの入力情報を取得
//========================================
XINPUT_STATE *CJoypad::GetXInputState(void)
{
	return &m_xInput;
}