//========================================
// 
// ゲーム
// 
//========================================
// *** game.h ***
//========================================
#ifndef _GAME_H_
#define _GAME_H_

#include "../main.h"
#include "../manager.h"

//****************************************
// 前方宣言
//****************************************
class CTime;	// タイム
class CScore;	// スコア
class CPlayer;	// プレイヤー
class CText;	// テキスト

//****************************************
// クラス
//****************************************
class CGame : public CScene
{
	static const int ADDTIME_MAX = 90;		// ボーナスをスコアに加算する時間
	static const int MAX_CUBE = 30;			// ステージで配置出来るキューブの制限
	static const char* STAGE_INFO_FILE;		// ステージ情報のファイルパス
	static const char* SIDE_STAGE_FILE;		// 側面のファイル
	static const char* FLOOR_STAGE_FILE;	// 床のファイル

	static const int TIME_SCORE = 50;		// １秒のスコアの加算値
	static const int CUBE_SCORE = 200;		// １個のスコアの加算値

public:

	// ***** 列挙型 *****

	// ステージ
	enum Stage
	{
		STAGE_00 = 0,
		STAGE_01,
		STAGE_02,
		STAGE_03,
		STAGE_04,
		STAGE_MAX
	};

	// ***** 関数 *****
	CGame();
	virtual~CGame();

	// -- メイン ------------------------------------------
	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);
	/* 生成		*/static CGame *Create(void);
	
	// -- 読み込み ------------------------------------------
	/* ステージ情報	*/static void LoodStage(void);
	/* 背景　天井	*/static void LoodCeiling(void);
	/* 背景　側面	*/static void LoodSide(void);
	/* 床			*/static void LoodFloor(void);
	/* ブロック		*/static void LoodBlock(void);
	/* エネミー		*/static void LoodEnemy(void);

	// -- 設定 ------------------------------------------
	/* ステージ選択	*/static void SetStage(int nStage);
	/* リセット		*/static void Reset(void);

	// -- 取得 ------------------------------------------
	/* プレイヤー	*/static CPlayer *GetPlayer() { return m_pPlayer; }
	/* タイム		*/static CTime *GetTime() { return m_pTime; }
	/* スコア		*/static CScore *GetScore() { return m_pScore; };
	/* 時間			*/static bool IsTime() { return m_bTime; }
	/* 終了			*/static bool IsEnd() { return m_bEnd; }
	
	// ステージ情報
	struct StageInfo
	{
		int nTime[STAGE_MAX];
		int nCube[STAGE_MAX];
		int nClearBonus[STAGE_MAX];
		char aBlockFile[STAGE_MAX][TXT_MAX];
		char aEnemyFile[TXT_MAX];
		D3DXVECTOR3 PlayerPos[STAGE_MAX];
		float PlayerRot[STAGE_MAX];
	};

	static StageInfo m_aStageInfo;

private:

	// ***** 列挙型 *****

	// リザルト演出
	enum Rst
	{
		RST_TEXT = 0,	// クリアボーナス！
		RST_TIME,		// タイムボーナス(テキスト)
		RST_TIME_CALC,	// タイムボーナス(計算)
		RST_CLEAR,		// クリアボーナス(テキスト)
		RST_CLEAR_CALC,	// クリアボーナス(数値)
		RST_BONUS,		// ボーナスの合計(テキスト)
		RST_BONUS_CALC,	// ボーナスの合計(計算)
		RST_ADD_SCORE,	// スコア加算
		RST_STAND,		// 待機
		RST_END,		// ランキング画面に移動
		RST_MAX
	};

	// ゲーム評価
	enum Eve
	{
		EVE_PERFECT = 0,	// パーフェクト Perfect
		EVE_GREAT,			// グレイト　great
		EVE_USUALLY,		//　ユージュアリー usually
		EVE_MAX
	};

	// ***** 関数 *****
	/* リザルト	*/void  Result(void);

	// ***** 変数 *****
	/* プレイヤー	*/static CPlayer *m_pPlayer;
	/* タイム		*/static CTime *m_pTime;
	/* スコア		*/static CScore *m_pScore;

	D3DXVECTOR3 m_rot;					// 向き
	float m_nMoveRot;					// 向きの加速度
	int m_nStartTime;					// 開始時間
	int m_nEndTime;						// 終了時間
	int m_nRstStgType;					// リザルト演出の種類
	int m_nTextCreate;					// 次のテキストの生成するまでの時間

	int m_nTimeTotal;					// タイムボーナスの合計値
	int m_nClearTotal;					// クリアボーナスの合計値
	int m_nTotal;						// 全ての合計値
	int m_nAddTime;						// スコアの加算時間
	int m_nStandTime;					// 待機時間(リザルト演出終了後)
	int m_nEveGame;						// ゲーム評価
	CText *m_RstText[RST_ADD_SCORE];	// テキスト表示
	bool m_bAddScore;					// スコアの加算フラグ
	bool m_bSpecial;					// 特殊クリア
	static bool m_bTime;				// 時間フラグ
	static int m_nScore;				// スコアの保存用
	static int m_nStage;				// 現在のステージ 
	static int m_nSelectStage;			// 現在選択しているステージ 
	static bool m_bEnd;					// 終了フラグ
	static bool m_bGemeOver;			// ゲームオーバー

	int nCntTest;
	int nCntTime;
};

#endif