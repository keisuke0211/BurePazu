//========================================
// 
// サウンドのヘッダファイル
// Author : AKIRA TANAKA
// |
// Arrange: KEISUKE OTONO
// 
//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SOUND_H_	// このマクロ定義がされていなかった時
#define _SOUND_H_	// 二重インクルード防止のマクロを定義する

#include "../main.h"

//****************************************
// クラス
//****************************************
class CSound
{
public:
	// ***** 定義 *****
	static const int SOUND_MAX = 64;	// サウンドの最大数
	static const char* FILE_PATH;		// 読み込むファイルパス

	// ***** 列挙型 *****

	// サウンドの種類
	enum SOUND_TYPE
	{
		SOUND_TYPE_BGM,	// 音楽
		SOUND_TYPE_SE,	// 効果音
		SOUND_TYPE_MAX,
	};

	// サウンドの種類
	enum TYPE
	{
		TYPE_TITLE,		// タイトル
		TYPE_GAME00,	// ゲーム0
		TYPE_GAME01,	// ゲーム1
		TYPE_RANKING,	// ランキング
		TYPE_SELECT,	// 選択
		TYPE_CYBER,		// 決定
		TYPE_SET,		// ブロック配置
		TYPE_ERASE,		// ブロック消滅
		TYPE_HIT,		// ヒット
		TYPE_ADD_SCORE,	// スコア加算
		TYPE_MAX,
	};

	// ***** 構造体 *****

	// サウンド情報
	struct SOUNDINFO
	{
		char		aFileName	// ファイル名
			[TXT_MAX];
		int			nCntLoop;	// ループカウント
		float		fVolume;	// 音量
		SOUND_TYPE	type;		// サウンドの種類
	};

	// ***** 関数 *****
	CSound();
	virtual~CSound();

	/* メイン */
	HRESULT Init(HWND hWnd);	// 初期化
	void Uninit(void);			// 終了
	void Load(void);			// 読み込み

	HRESULT PlaySound(int nSoundIdx);	// セグメント再生(再生中なら停止)
	void StopSound(int nSoundIdx);		// セグメント停止(ラベル指定)
	void StopSound(void);				// セグメント(全て)

	void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);	// 種類毎に音量変更
	void StopSoundType(SOUND_TYPE type);						// 種類毎に再生停止

	/* 取得 */
	SOUNDINFO *GetInfo() { return m_aSoundInfo; }

private:

	// ***** 関数 *****
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// チャンクのチェック
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータの読み込み

	// ***** 変数 *****
	IXAudio2 *m_pXAudio2 = NULL;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;		// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_MAX] = {};						// オーディオデータサイズ

	static float m_aSoundVolume[SOUND_TYPE_MAX];	// 音量
	static float m_aSettingVolume[SOUND_TYPE_MAX];	// 設定の音量
	SOUNDINFO m_aSoundInfo[SOUND_MAX];				// サウンドの情報
	int m_nSoundNum;								// サウンドの数
};
#endif
