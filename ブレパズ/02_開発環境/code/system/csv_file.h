//========================================
// 
// csvファイル入出力処理
// 
//========================================
// *** csv_file.h ***
//========================================
#ifndef _CSV_FILE_H_	// このマクロ定義がされていなかった時
#define _CSV_FILE_H_	// 二重インクルード防止のマクロを定義する

#pragma once
#include <iostream>	// 外部の装置と入出力
#include <fstream>	// ファイル の入出力
#include <vector>
#include <string>
#include <sstream>	// カンマで区切るために必要

using namespace std;

//****************************************
// クラス
//****************************************
class CSVFILE
{
public:
	
	// ***** 列挙型 *****

	// 端数処理
	enum FORMAT
	{
		FORMAT_ROUNF,	// 四捨五入
		FORMAT_CEIL,	// 切り上げ
		FORMAT_FLOOR,	// 切り捨て
		FORMAT_MAX,
	};

	// ***** コンストラクタ・デストラクタ *****
	CSVFILE();
	virtual~CSVFILE();

	// ***** 関数 *****

	//--------------------------------------------------
	// 読み込み処理
	// 引数1  : string filepath / ファイルパス
	// 引数2  : bool bHeader	/ ヘッダーの有無
	// 引数3  : bool bIndex		/ インデックスの有無
	// 引数4  : char delim		/ 区切り文字
	//--------------------------------------------------
	void FileLood(string filepath, bool bHeader, bool bIndex, char delim);

	//--------------------------------------------------
	// 書き出し処理
	// 引数1  : string filepath / ファイルパス
	// 引数2  : char delim		/ 区切り文字
	//--------------------------------------------------
	void FileSave(string filepath, char delim);

	//--------------------------------------------------
	// コンソール出力処理
	//--------------------------------------------------
	void FileShow(void);

	// -- 構成 -------------------------------------------------------------------

	//--------------------------------------------------
	// 端数処理 (float型)
	// 引数1  : float &val		/ データ
	// 引数2  : ing nLen		/ 桁数
	// 引数3  : FORMAT format	/ 処理
	// 返値   : float			/ データ(変換後)
	//--------------------------------------------------
	float Format(float val, int nLen, FORMAT format);

	//--------------------------------------------------
	// 端数処理 (double型)
	// 引数1  : double &val		/ データ
	// 引数2  : ing nLen		/ 桁数
	// 引数3  : FORMAT format	/ 処理
	// 返値   : double			/ データ(変換後)
	//--------------------------------------------------
	double Format(double val, int nLen, FORMAT format);

	// -- 変換 -------------------------------------------------------------------

	//--------------------------------------------------
	// 文字列 → int型
	// 引数1  : int &val    / データ(変換先)
	// 引数2  : string &str / データ(変換元)
	// 返値   : bool        / 変換できたか
	//--------------------------------------------------
	bool ToValue(int &val, const string &str);

	//--------------------------------------------------
	// 文字列 → float型
	// 引数1  : float &val  / データ(変換先)
	// 引数2  : string &str / データ(変換元)
	// 返値   : bool        / 変換できたか
	//--------------------------------------------------
	bool ToValue(float &val, const string &str);

	//--------------------------------------------------
	// 文字列 → double型
	// 引数1  : double &val / データ(変換先)
	// 引数2  : string &str / データ(変換元)
	// 返値   : bool        / 変換できたか
	//--------------------------------------------------
	bool ToValue(double &val, const string &str);

	//--------------------------------------------------
	// 文字列 → char型
	// 引数1  : char &val	/ データ(変換先)
	// 引数2  : string &str / データ(変換元)
	// 返値   : bool        / 変換できたか
	//--------------------------------------------------
	bool ToValue(char* &val, const string &str);

	//--------------------------------------------------
	// 文字列 → bool型
	// 引数1  : bool &val	/ データ(変換先)
	// 引数2  : string &str / データ(変換元)
	// 返値   : bool        / 変換できたか
	//--------------------------------------------------
	bool ToValue(bool &val, const string &str);

	//--------------------------------------------------
	// int型 → 文字列
	// 引数1  : int &val / データ
	// 返値   : string	/ データ(string)
	//--------------------------------------------------
	string CastCell(int &val) { return to_string(val); }

	//--------------------------------------------------
	// float型 → 文字列
	// 引数1  : float &val  / データ
	// 返値   : string	   / データ(string)
	//--------------------------------------------------
	string CastCell(float &val){ return to_string(Format(val, 2, FORMAT_FLOOR)); }

	//--------------------------------------------------
	// double型 → 文字列
	// 引数1  : double &val / データ
	// 返値   : string	   / データ(string)
	//--------------------------------------------------
	string CastCell(double &val) { return to_string(val); }

	//--------------------------------------------------
	// char型 → 文字列
	// 引数1  : char &val / データ
	// 返値   : string	   / データ(string)
	//--------------------------------------------------
	string CastCell(char &val) { return &val; }

	//--------------------------------------------------
	// char型 → 文字列
	// 引数1  : char* &val / データ
	// 返値   : string	   / データ(string)
	//--------------------------------------------------
	string CastCell(char* &val) { return * &val; }

	// -- 設定 -----------------------------------------
	/* ヘッダー		*/void SetHeader(string str, char delim);
	/* インデックス	*/void SetIndex(string str, char delim);

	// -- 設定・追加 ------------------------------------------
	/* 要素	*/void SetCell(string str, int row, int line);

	// -- 取得 ------------------------------------------
	/* 行数	*/int GetRowSize(void) { return cell.size(); }
	/* 列数	*/int GetLineSize(int row) { return cell.at(row).size(); }
	/* 要素 */string GetData(int row, int line) { return cell.at(row).at(line); }

private:

	// ***** 変数 *****
	string filepath;	// ファイルパス
	bool bHeader;		// ヘッダーの有無
	bool bIndex;		// インデックスの有無
	char delim;			// 区切り文字

	/* 保管 */
	vector<string> header;			// ヘッダー名
	vector<string> index;			// インデックス名
	vector<vector<string>> cell;	// 要素
};

#endif