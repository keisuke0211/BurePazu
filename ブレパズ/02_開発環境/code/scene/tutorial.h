//========================================
// 
// チュートリアル
// 
//========================================
// *** tutorial.h ***
//========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "../main.h"
#include "../manager.h"

//****************************************
// 前方宣言
//****************************************
class CText;
class CPlayer;
class CEnemy;

//****************************************
// クラス
//****************************************
class CTutorial : public CScene
{
	// ***** 定義 *****
	static const char* FILE_PATH;		// 読み込むファイルパス
	static const int MAX_TIME = 90;		// 制限時間
	static const int NUM_TEXT = 10;		// テキスト数

	static const char* SIDE_FILE;		// 側面のファイルパス
	static const char* FLOOR_FILE;		// 床のファイルパス
	static const char* BLOCK_FILE1;		// ブロックのファイルパス
	static const char* ENEMY_FILE1;		// エネミーのファイルパス
public:

	// ***** 列挙型 *****

	// 行動
	enum ACTION
	{
		ACTION_MOVE = 0,	// 移動
		ACTION_CAMERA,		// 視点移動
		ACTION_SHOT,		// 発射
		ACTION_SET,			// 配置(説明)
		ACTION_SET1,		// 配置(説明)
		ACTION_ENEMY,		// 敵
		ACTION_CLEAR,		// キューブ説明
		ACTION_CLEAR1,		// キューブ説明
		ACTION_FREE,		// 終了
		ACTION_MAX
	};

	// ***** 関数 *****
	CTutorial();
	~CTutorial();

	// -- メイン ------------------------------------------
	/* 初期化			*/HRESULT Init(void);
	/* 終了				*/void Uninit(void);
	/* 更新				*/void Update(void);
	/* 描画				*/void Draw(void);
	/* 生成				*/static CTutorial *Create(void);
	/* 読み込み			*/void TextLoad(void);

	// -- 読み込み ----------
	/* 背景　天井	*/static void LoodCeiling(void);
	/* 背景　側面	*/static void LoodSide(void);
	/* 床			*/static void LoodFloor(void);
	/* ブロック		*/static void LoodBlock(void);
	/* エネミー		*/static void LoodEnemy(void);

	// -- 取得 ------------------------
	/* プレイヤー	*/static CPlayer *GetPlayer() { return m_pPlayer; }
	/* タイム		*/static CTime *GetTime() { return m_pTime; }
	/* スコア		*/static CScore *GetScore() { return m_pScore; };
	/* 現在の行動	*/static int GetCurAction() { return m_nCurAction; }

private:

	// ***** 構造体 *****

	// テキスト情報
	struct LoadTxt
	{
		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR2 size;			// サイズ
		int nType;					// 種類
		int nStartTime;				// 開始時間
		int nStandTime;				// 待機時間
		int nDisapTime;				// 消える時間
		int nCreateTime;			// 生成待機時間
		char *ActionTex[TXT_MAX];	// 行動テキスト
		float nTextSize;			// テキストサイズ
		bool bTextBok;				// テキストボックスの表示フラグ
	};

	// 
	struct CreateTxt
	{
		int nNumCur;				// 現在の行動の生成数
		int nNumCurAll;				// 全体で生成した総数
		int nCurAction;				// 現在の行動
		int nCreateTime;			// 次のテキストを生成するまでの時間
		int nCurMax[ACTION_MAX];	// 各行動の最大数
		bool bCreate[ACTION_MAX];	// 生成フラグ
	};

	// ***** 関数 *****
	/* テキスト情報初期化	*/void TxtInit(int nIdx);
	/* テキスト生成			*/void TxtCreate(int nType);
	/* テキスト削除			*/void TxtDelete(int nType,int nNextType);
	/* チュートリアル		*/void TutorialTex(void);

	// ***** 静的変数 *****
	/* テキスト情報	*/static LoadTxt*m_LoadText;
	/* プレイヤー	*/static CPlayer*m_pPlayer;
	/* タイム		*/static CTime	*m_pTime;
	/* スコア		*/static CScore *m_pScore;
	/* 現在の行動	*/static int m_nCurAction;

	// ***** 変数 *****
	CText *m_Txt[3];			// テキスト
	ACTION Action;				// 行動種類
	CreateTxt m_aCreateText;	// 生成情報

	int m_nStartTime;		// 開始時間
	int m_nEndTime;			// 終了時間
	bool m_bEnd;			// 終了フラグ
};

#endif