//========================================
// 
// テキスト表示
// 
//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_
#define _TEXT_H_

#include "../../object/object2D.h"
#include "words.h"

//****************************************
// 構造体
//****************************************

// テキスト情報(生成用)
struct FormFont
{
	D3DXCOLOR col;		// 文字の色
	float fTextSize;	// 文字のサイズ(初期値 20)
	int nAppearTime;	// 1文字目が表示されるまでの時間(初期値 1)
	int nStandTime;		// 待機時間(初期値 10)
	int nEraseTime;		// 消えるまでの時間(初期値 1) ※ 0 は消えない
};

// 影情報(生成用)
struct FormShadow
{
	D3DXCOLOR col;			// 影の色
	bool bShadow;			// 影フラグ
	D3DXVECTOR3 AddPos;		// 文字の位置からずらす値 (初期値 0,0,0)	(元の文字 + AddPos)
	D3DXVECTOR2 AddSize;	// 文字のサイズの加算値 (初期値 0,0)		(元の文字 + AddSize)
};

//****************************************
// 前方宣言
//****************************************
class CFont;

//****************************************
// クラス
//****************************************
class CText : public CObject2D
{
public:

	// ***** 列挙型 *****

	// テキストボックスの種類
	enum Box
	{
		BOX_NORMAL_RECT = 0,	// ノーマル(長方形)
		BOX_NORMAL_SQR,			// ノーマル(正方形)
		BOX_MAX
	};

	// ***** 関数 *****
	CText(int nPriority = PRIO_TEXT);
	~CText();

	// -- メイン ------------------------------------------
	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	//--------------------------------------------------
	// テキスト生成
	// 引数1  : Box type            / テキストボックスの画像
	// 引数2  : D3DXVECTOR3 pos     / テキストボックスの位置
	// 引数3  : D3DXVECTOR2 size    / テキストボックスのサイズ
	// 引数4  : const char *Text　　/ テキスト
	// 引数5  : CFont::FONT Type　　/ フォント種類
	// 引数6  : FormFont *pFont		/ フォント関連の情報(色・時間など) 無くても大丈夫
	// 引数7  : bool bTextBok　　　 / メッセージボックスの表示・非表示 (初期値 true)
	// 引数8  : FormShadow *Shadow  / フォントの影の情報 無くても大丈夫
	//--------------------------------------------------
	static CText *CText::Create(Box type, D3DXVECTOR3 pos, D3DXVECTOR2 size, const char *Text, CFont::FONT FontType, FormFont *pFont = NULL, bool bTextBok = true, FormShadow *Shadow = NULL);

	/* 削除 */void Disap(bool bDisap,int nTime);

	// -- 設定 ------------------------------------------
	/* 空白表示			*/void SetSpace(bool bSpace) { m_Info.bSpace = bSpace; }
	/* ポーズ中の生成	*/void SetTetPause(bool bPause);
	/* ボックスの色		*/void SetBoxColor(D3DXCOLOR col);
	/* テキストの色		*/bool SetTextColor(D3DXCOLOR col);
	/* 文字変更(単体)	*/bool ChgWords(char* Text, int nIdx, D3DXCOLOR col);
	/* 文字変更(全体)	*/bool ChgText(char* Text, D3DXCOLOR col);// ※ 元のテキストより多いと使えない また半角英数のみ

	// -- 設定 ------------------------------------------
	/*　サイズ	*/float GetTxtSize() { return m_Info.fTextSize; }

private:

	// ***** 構造体 *****

	// 影
	struct Shadow
	{
		D3DXCOLOR col;			// 影の色
		bool bShadow;			// 影フラグ
		CWords** shadow;		// 文字(影)
		D3DXVECTOR3 AddPos;		// 文字の位置からずらす値 (初期値 0,0,0)	(元の文字 + AddPos)
		D3DXVECTOR2 AddSize;	// 文字のサイズの加算値 (初期値 0,0)		(元の文字 + AddSize)
	};

	// テキスト情報
	struct Info
	{
		D3DXCOLOR TextBoxCol;	// テキストボックスの色
		D3DXCOLOR FontCol;		// 文字の色
		D3DXCOLOR TextBoxColOld;// 前回のテキストボックスの色
		D3DXCOLOR FontColOld;	// 前回の文字の色
		bool bCol;				// 色変更フラグ(ポーズ切替)

		float fTextSize;		// 文字のサイズ
		int nTextLength;		// 文字の長さ
		int nAppearTime;		// 1つの文字が加算されるまでの時間
		int nAddCount;			// 文字が加算されるまでのカウント
		int nAddLetter;			// 何文字目入れるか指定
		int nLetterPopCount;	// 文字を出した最大数
		int nLetterPopCountX;	// 横に文字を出した数
		int nNiCount;			// 改行の数

		int nStandTime;			// 待機時間
		bool bStand;			// 待機フラグ

		int nDisapTime;			// 消える時間
		int nDisapTimeMax;		// 消える最大時間
		bool bRelease;			// 消すフラグ

		Shadow aShadow;			// 影

		bool bTextBok;			// テキストボックスの表示フラグ
		bool bPause;			// ポーズ中でも動くか（false：動かない）
		bool bSpace;			// 空白表示フラグ(sprintf用)
		string sText;			// 表示するテキスト
		string sALLText;		// テキストの全体
		CWords** words;			// 文字
		CFont::FONT FontType;	// フォント種類
	};

	// ***** 関数 *****
	/* 文字生成	*/void LetterForm(void);
	/* 待機		*/void StandTime(void);
	/* 削除		*/void DisapTime(void);
	/* 文字判定	*/bool CheckLeadByte(int nLetter);

	// -- 設定 ------------------------------------------
	/* テキスト サイズ	*/void SetTextSize(float TextSize);
	/* 待機時間			*/void SetStandTime(int StandTime);
	/* 文字の消す時間	*/void EraseTime(int time);
	/* 文字の出現時間	*/void TextLetter(const char *Text, int DispText);

	// ***** 変数 *****
	Info m_Info;

};

#endif