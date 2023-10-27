//========================================
// 
// モーション処理
// 
//========================================
// *** motion.h ***
//========================================
#include "../../main.h"

#ifndef _MOTION_H_
#define _MOTION_H_

//****************************************
// マクロ定義
//****************************************
#define MYMAX_MOTION		(8)		// モーションの最大数
#define MAX_MODEL_PARTS		(16)	// モデル数の最大数
#define MAX_KEY				(16)	// キーの最大数
#define MAX_KEYSET			(16)	// キー設定の最大数
#define MOTION_BLEND_FRAM	(12)	// モーションのブレンドのフレーム数

//****************************************
// 前方宣言
//****************************************
class CModel;
class CParts;

//****************************************
// クラス
//****************************************
class CMotion
{
public:

	// ***** 構造体 *****

	// キー情報
	struct Key
	{
		D3DXVECTOR3		pos;	// 現在位置
		D3DXVECTOR3		rot;	// 現在の向き
	};

	// キー設定
	struct KeySet
	{
		int	nFrame;		// フレーム数
		Key	*pKey;		// キー情報
	};

	// モーション設定
	struct Motion
	{
		int		nNumKey;		// キー数
		int		nCntKeySet;		// キーセットカウント
		int		nCntFrame;		// フレームカウント
		bool	bLoop;			// ループ使用状況
		bool	bMotion;		// モーションを行っているか
		KeySet	*pKeySet;		// キー設定情報
	};



	// ***** 関数 *****

	CMotion(const char *pFileName);	// コンストラクタ
	virtual~CMotion();						// デストラクタ

	/* メイン */
	void Update(void);		// 更新
	void Uninit(void);		// 終了

	/* パーツ */

	// 設定
	void SetParts(D3DXMATRIX mtxWorld, const bool IsColor, const D3DXCOLOR color);

	// 元の場所に配置
	void SetPartsOrigin(void);	

	/* モーション */
	void SetMotion(const int nCntMotionSet);	// 初期設定
	void SetNumMotion(const int nNumMotion,bool bStop);	// モーション番号の設定

	// モーションを実行しているかの設定
	void SetUseMotion(bool bMotion) { m_bMotion = bMotion; }

	// モーションブレンドを実行しているか設定
	void SetMotionBlend(bool bMotionBlend) { m_bMotionBlend = bMotionBlend; }

	/* 取得 */
	int GetMaxParts(void) { return m_nMaxParts; }		// パーツの最大数
	bool GetMotion(void) { return m_bMotion; }			// モーションを実行しているか
	bool GetMotionBlend() { return m_bMotionBlend; }	// モーションブレンドを実行しているか

	float NormalizeAngle(float *pAngle);				// 角度の正規化

private:

	// ***** 関数 *****
	void Init(void);							// 初期化
	void PlayMotion(void);						// モーションの再生
	void MotionBlend(void);						// モーションブレンド
	void LoodMotion(const char *pFileName);		// モーション読み込み
	void CntReset(const int nNumMotionOld);		// カウントリセット

	// ***** 変数 *****
	CModel		*m_pParent;								// 親
	Motion		*m_motion;								// モーション
	CParts		**m_ppParts;							// パーツ
	char		m_partsFile[MAX_MODEL_PARTS][0xff];		// パーツのXファイル名
	int			m_nMaxParts;							// パーツ数
	int			m_nNumMotion;							// 扱うモーション
	bool		m_bMotion;								// モーションを行うか
	bool		m_bMotionBlend;							// モーションブレンド
};

#endif