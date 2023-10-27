//========================================
// 
// ランキング
// 
//========================================
// *** ranking.h ***
//========================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "../main.h"
#include "../manager.h"
#include "game.h"

//****************************************
// 前方宣言
//****************************************
class CText;

//****************************************
// クラス
//****************************************
class CRanking : public CScene
{
	static const char* STAGE_FILE;			// ファイルパス
	static const char* TEXT_FILE_PATH;		// 読み込むファイルパス
	static const int RANK_NUM = 8;			// 順位数
	static const int NAME_NUM = 5;			// ランキングのの文字数
	static const int EXPLAIN_MAX = 7;		// 操作説明のテキスト数
	static const int TEXT_MAX = 10;			// 文字表示のテキスト数
	static const int NAME_START_DEX = 5;	// 名前の開始位置
	static const int BLINK_TIME = 4;		// 点滅時間

#define BLINK_COLOR		D3DXCOLOR{0.057f,0.737f,0.0f,1.0f}		// ランキングフレームの点滅色
#define RANKING_COLOR	D3DXCOLOR{0.23f,1.0f,0.0f,1.0f}			// ランキングフレームの更新色

public:

	// ***** 関数 *****
	CRanking();
	~CRanking();

	// -- メイン ------------------------------------------
	/* 初期化		*/HRESULT Init(void);
	/* 終了			*/void Uninit(void);
	/* 更新			*/void Update(void);
	/* 描画			*/void Draw(void);
	/* 生成			*/static CRanking *Create(void);

	// -- 設定 ------------------------------------------
	/* スコア	*/int SetScore(int nScore);
	/* 名前入力	*/void SetNameEntry(int nUpdateRank);
	/* スコア	*/static void SetScore11(int nScore);
	/* 全体表示	*/static void SetAllStage(bool bAll) { m_bRankingAll = bAll; }

	// -- 取得 ------------------------------------------
	/* 順位の表示形式の取得	*/char *GetRankText(int nRank) { return m_aRank[nRank].aRankText; }

	// ***** 変数 *****
	static int m_nGameScore;
	static bool m_bSetScore;

private:

	// ***** 構造体 *****
	
	// 文字
	struct Letter
	{
		char aConv[4];		// 文字
	};

	// 変換数
	struct Conv
	{
		Letter *pLetter;
	};

	// 文字列
	struct String
	{
		Conv	*pConv;		// 文字
		int		nLettreMax;	// 文字の最大数
		int		nConvMax;	// 変換文字の最大数
	};

	// 共通情報
	struct Info
	{
		int		nCounterState;			// 状態カウンター
		bool	bNameEntry;				// 名前入力
		int		nCntName;				// 名前カウント
		int		nNameChar[NAME_NUM];	// 名前の文字数 (半角英数 1 : 全角英数 2)
		int		nCntChar;				// 文字カウント
		int		nUpdateRank;			// 更新順位
		int		nCntBlink;				// 点滅カウンター
		bool	bNameInput;				// 入力フラグ

		int		nCntString;		// 文字列カウント
		int		nCntLetter;		// 文字カウント
		int		nCntConv;		// 変換文字カウント

		int		nCounterBlink;	// 点滅カウンター
	};

	// 各順位のテキスト
	struct Rank
	{
		char aRankText[TXT_MAX];	// テキスト
	};

	// ランキング情報
	struct Ranking
	{
		int nScore;				// スコア
		char aName[TXT_MAX];	// 名前 
	};

	// ***** 関数 *****
	/* ステージ			*/void Load(void);
	/* 文字読み込み		*/void WordsLoad(void);
	/* 書き出し			*/void Save(void);
	/* 状態処理			*/void State(void);
	/* 名前入力			*/void NameEntry(void);
	/* 名前入力			*/void NameInput(void);
	/* ランキング切替	*/void RankingSwitch(void);
	/* ソート降順		*/void SortDesc(int *nUpdateRank);
	/* 空白埋め			*/void strinit(char *pData, int nNum);
	/* 文字表示(設定)	*/void SetWords(void);
	/* 文字表示(更新)	*/void UpdateWords(void);

	// ***** 変数 *****
	Info m_Info;						// 共通情報
	Ranking m_Ranking[RANK_NUM];		// 順位情報
	CText *m_Text[RANK_NUM];			// 順位テキスト
	char m_aNameData[TXT_MAX];			// 表示テキスト

	CText *m_Explain[EXPLAIN_MAX];		// 操作説明テキスト
	CText *m_Words[TEXT_MAX];			// テキスト表示

	String	*m_pString;					// 文字列
	int		nStringMax;					// 文字列の最大数

	static bool m_bRankingAll;			// 全ステージのランキングの表示フラグ

	// 順位の表示形式
	Rank m_aRank[RANK_NUM] =
	{
		"1ST",
		"2ND",
		"3RD",
		"4TH",
		"5TH",
		"6TH",
		"7TH",
		"8TH",
	};
};

#endif