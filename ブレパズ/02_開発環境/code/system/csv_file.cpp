//========================================
// 
// csvファイル入出力処理
// 
//========================================
// *** csv_file.cpp ***
//========================================
#include "csv_file.h"

//========================================
// コンストラクタ
//========================================
CSVFILE::CSVFILE()
{
	bHeader = false;	// ヘッダー
	bIndex = false;		// インデックス
	delim = ',';		// 区切り文字
}

//========================================
// デストラク
//========================================
CSVFILE::~CSVFILE()
{

}

//================================================================================
// 読み込み処理
// 引数：ファイルパス / ヘッダー有無 / インデックス有無 / 区切り文字
//================================================================================
void CSVFILE::FileLood(string filepath, bool bHeader, bool bIndex, char delim)
{
	// 各要素を保存
	this->filepath = filepath;
	this->bHeader = bHeader;
	this->bIndex = bIndex;
	this->delim = delim;

	string str_buf;			// １行のデータを格納する変数
	string str_comma_buf;	// delim 区切り したデータを格納する変数

	// ファイルを開く (ifstreamのコンストラクタで開く)
	ifstream ifs_csv_file(filepath);

	// １行ずつ読み込む (読み込んだ内容を str_buf に格納)
	for (int nRow = 0; getline(ifs_csv_file, str_buf); nRow++)
	{
		cell.push_back(vector<string>());

		// delim 区切りごとにデータを読み込むために istringstream にする
		istringstream i_stream(str_buf);

		for (int nLine = 0; getline(i_stream, str_comma_buf, delim); nLine++)
		{
			// -- 読み込み------------------------
			if (bHeader && bIndex)
			{// ヘッダーとインデックスがある時

				/* 折り返す		*/if (nRow == 0 && nLine == 0) continue;
				/* ヘッダー		*/if (nRow == 0 && nLine != 0) header.push_back(str_comma_buf);
				/* インデックス */if (nRow != 0 && nLine == 0) index.push_back(str_comma_buf);
				/* 要素			*/if (nRow != 0 && nLine != 0) cell.at(nRow - 1).push_back(str_comma_buf);
			}
			else if (bHeader)
			{// ヘッダーがある時
				/* ヘッダー		*/if (nRow == 0)				header.push_back(str_comma_buf);
				/* インデックス */if (nRow != 0 && nLine == 0)	index.push_back(string());
				/* 要素			*/if (nRow != 0)				cell.at(nRow - 1).push_back(str_comma_buf);
			}
			else if (bIndex)
			{// インデックスがある時
				/* ヘッダー		*/if (nRow == 0 && nLine != 0)	header.push_back(str_comma_buf);
				/* インデックス */if (nLine == 0)				index.push_back(str_comma_buf);
				/* 要素			*/if (nLine != 0)				cell.at(nRow).push_back(str_comma_buf);
			}
			else
			{// 要素のみ
				/* ヘッダー		*/if (nRow == 0)	header.push_back(str_comma_buf);
				/* インデックス */if (nLine == 0)	index.push_back(str_comma_buf);
				/* 要素			*/					cell.at(nRow).push_back(str_comma_buf);
			}
		}
	}
}

//================================================================================
// 書き出し処理
// 引数：ファイルパス / 区切り文字
//================================================================================
void CSVFILE::FileSave(string filepath, char delim)
{
	// 書き込むファイルを開く (ofstreamのコンストラクタで開く)
	ofstream ofs_csv_file(filepath);

	if (bHeader)
	{
		for (int nRow = 0; nRow < header.size(); nRow++)
		{
			/* 空白		*/if (bIndex && nRow == 0)ofs_csv_file << delim;
			/* ヘッダー */ofs_csv_file << header.at(nRow) << delim;
		}
		ofs_csv_file << endl;
	}

	for (int nLine = 0; nLine < index.size(); nLine++)
	{
		if (bIndex)
		{
			/* インデックス */ofs_csv_file << index.at(nLine) << delim;
		}
		for (int nRow = 0; nRow < header.size(); nRow++)
		{
			/* 要素 */ofs_csv_file << cell.at(nLine).at(nRow) << delim;
		}
		ofs_csv_file << endl;
	}
}

//================================================================================
// コンソール出力処理
//================================================================================
void CSVFILE::FileShow(void)
{
	/* ファイルパス			*/cout << "filepath = " << filepath << ", ";
	/* ヘッダー				*/cout << "bHeader = " << int(bHeader) << ", ";
	/* インデックス			*/cout << "bIndex = " << int(bIndex) << ", ";
	/* 要素					*/cout << "delim = " << delim << endl;
	/* ヘッダー サイズ		*/cout << "header size = " << header.size() << ", ";
	/* インデックス サイズ	*/cout << "index size = " << index.size() << endl;

	if (bHeader)
	{
		for (int nRow = 0; nRow < header.size(); nRow++)
		{
			if (bIndex && nRow == 0) cout << "\t";
			cout << header.at(nRow) << "(h)" << "\t";
		}
		cout << endl;
	}
	for (int nLine = 0; nLine < index.size(); nLine++)
	{
		if (bIndex)
		{
			cout << index.at(nLine) << "(i)" << "\t";
		}
		for (int j = 0; j < header.size(); j++)
		{
			cout << cell.at(nLine).at(j) << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

//================================================================================
// ヘッダーの設定
// 引数：ヘッダー名 / 区切り文字
//================================================================================
void CSVFILE::SetHeader(string str, char delim)
{
	string str_comma_buf;	// delim 区切り したデータを格納する変数

	// delim 区切りごとにデータを読み込むために istringstream にする
	istringstream i_stream(str);

	for (int nLine = 0; getline(i_stream, str_comma_buf, delim); nLine++)
	{
		header.push_back(str_comma_buf);
	}

	bHeader = true;
}

//================================================================================
// インデックスの設定 (一括)
// 引数：インデックス名 / 区切り文字
//================================================================================
void CSVFILE::SetIndex(string str, char delim)
{
	string str_comma_buf;	// delim 区切り したデータを格納する変数

	// delim 区切りごとにデータを読み込むために istringstream にする
	istringstream i_stream(str);

	for (int nLine = 0; getline(i_stream, str_comma_buf, delim); nLine++)
	{
		index.push_back(str_comma_buf);
	}

	bIndex = true;
}

//================================================================================
// データ(要素)の設定・追加
// 引数：データ / 行番号 / 列番号
//================================================================================
void CSVFILE::SetCell(string str, int row,int line)
{
	cell.push_back(vector<string>());

	if (!bIndex && line == 0)
	{
		index.push_back(string());
	}

	cell.at(row).push_back(str);
}

//================================================================================
// 桁数を制御 (float型)
// 引数：データ / 桁数 / 処理
// 返値：データ(変換後)
//================================================================================
float CSVFILE::Format(float val, int nLen, FORMAT format)
{	
	float fRet;

	fRet = val * powf(10.0, nLen);

	switch (format)
	{
	case FORMAT_ROUNF: { /* 四捨五入 */
		fRet = (int)(fRet + 0.5);
	}
	  break;
	case FORMAT_CEIL: { /* 切り上げ */
		fRet = (int)(fRet + 0.9);
	}
	  break;
	case FORMAT_FLOOR: { /* 切り捨て */
		fRet = (int)(fRet);
	}
	  break;
	}

	return fRet * powf(10.0, -nLen);
}

//================================================================================
// 桁数を制御 (float型)
// 引数：データ / 桁数 / 処理
// 返値：データ(変換後)
//================================================================================
double CSVFILE::Format(double val, int nLen, FORMAT format)
{
	float fRet;

	fRet = val * pow(10.0, nLen);

	switch (format)
	{
	case FORMAT_ROUNF: { /* 四捨五入 */
		fRet = (double)(int)(fRet + 0.5);
	}
	  break;
	case FORMAT_CEIL: { /* 切り上げ */
		fRet = (double)(int)(fRet + 0.9);
	}
	  break;
	case FORMAT_FLOOR: { /* 切り捨て */
		fRet = (double)(int)(fRet);
	}
	  break;
	}

	return fRet * pow(10.0, -nLen);
}

//================================================================================
// 文字列 → int型
// 引数：データ(変換先) / データ(変換元)
// 返値：変換出来たか
//================================================================================
bool CSVFILE::ToValue(int &val, const string &str)
{
	try 
	{
		val = stoi(str);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

//================================================================================
// 文字列 → float型
// 引数：データ(変換先) / データ(変換元)
// 返値：変換出来たか
//================================================================================
bool CSVFILE::ToValue(float &val, const string &str)
{
	try 
	{
		val = stof(str);
		return true;
	}
	catch (...) 
	{
		return false;
	}
}

//================================================================================
// 文字列 → double型
// 引数：データ(変換先) / データ(変換元)
// 返値：変換出来たか
//================================================================================
bool CSVFILE::ToValue(double &val, const string &str)
{
	try
	{
		val = stod(str);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

//================================================================================
// 文字列 → char型
// 引数：データ(変換先) / データ(変換元)
// 返値：変換出来たか
//================================================================================
bool CSVFILE::ToValue(char* &val, const string &str)
{
	try
	{
		char* cstr = new char[str.size() + 1]; // メモリ確保

		char_traits<char>::copy(cstr, str.c_str(), str.size() + 1);

		val = cstr;
		return true;
	}
	catch (...)
	{
		return false;
	}
}

//================================================================================
// 文字列 → bool型
// 引数：データ(変換先) / データ(変換元)
// 返値：変換出来たか
//================================================================================
bool CSVFILE::ToValue(bool &val, const string &str)
{
	try
	{
		val = stoi(str);
		return true;
	}
	catch (...)
	{
		return false;
	}
}