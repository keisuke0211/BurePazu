//========================================
// 
// モーションモデル処理
// 
//========================================
// *** motion_model.h ***
//========================================
#ifndef _MOTION_MODEL_H_			// このマクロ定義がされてなかったら
#define _MOTION_MODEL_H_			// 二重インクルード防止のマクロ定義

#include "../object.h"
#include "../../main.h"

//****************************************
// 前方宣言
//****************************************
class CMotion;

//****************************************
// クラス
//****************************************
class CMotionModel : public CObject
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CMotionModel *Create();			// 3Dモデルの生成

	CMotionModel(int nPriority = PRIO_OBJECT);		// コンストラクタ
	virtual~CMotionModel();							// デストラクタ

	// ***** 関数 *****
	HRESULT Init(void);													// 初期化
	void Uninit(void);													// 終了
	void Update(void);													// 更新
	void Draw(void);													// 描画
	void ChangeColar(D3DXCOLOR color);									// 色の変更

	/* 設定 */
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = m_posOld =pos; };		// 位置
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = m_rotOld = rot; };	// 向き
	void SetScale(const D3DXVECTOR3 &scale) { m_scale = scale; }		// サイズ
	void SetColor(D3DXCOLOR color) { m_color = color; }					// 色
	void SetIsColor(bool bcolor) { m_bcolor = bcolor; }					// 色変更
	void SetSize(const D3DXVECTOR3 &size) { m_size = size; }			// 当たり判定
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }	// ワールドマトリックス
	void SetMotion(const char *pName);									// モーション情報
	void SetStencil(bool isStencil) { m_Stencil = isStencil; }			// ステンシルバッファ
	void SetQuat(D3DXQUATERNION quat) { m_quat = quat; }				// クオータニオン


	/* 取得 */
	D3DXVECTOR3 GetPos() { return m_pos; }								// 位置
	D3DXVECTOR3 GetPosOld() { return m_posOld; }						// 位置(過去)
	D3DXVECTOR3 GetRot() { return m_rot; }								// 向き
	D3DXVECTOR3 GetRotOld() { return m_rotOld; }						// 向き(過去)
	D3DXVECTOR3 GetScale() { return m_scale; }							// サイズ
	D3DXCOLOR GetColor() { return m_color; }							// 色
	bool GetIsColor() { return m_bcolor; }								// 色変更
	D3DXVECTOR3 GetSize() { return m_size; }							// 当たり判定
	D3DXMATRIX* GetMtxWorld() { return &m_mtxWorld; }					// ワールドマトリックス
	CMotion *GetMotion() { return m_pMotion; }							// モーション情報
	D3DXQUATERNION GetQuat() { return m_quat; }							// クオータニオン

private:
	// ***** 変数 *****
	CMotion			*m_pMotion;				// モーションインスタンス
	D3DXMATRIX		m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3		m_pos;					// 位置
	D3DXVECTOR3		m_posOld;				// 位置(過去)
	D3DXVECTOR3		m_rot;					// 向き
	D3DXVECTOR3		m_rotOld;				// 向き(過去)
	D3DXVECTOR3		m_scale;				// スケール
	D3DXVECTOR3		m_size;					// 当たり判定
	D3DXCOLOR		m_color;				// 色
	D3DXQUATERNION m_quat;					// クオータニオン
	bool m_Stencil;							// ステンシルバッファの有効かどうか
	bool m_bcolor;							// 色を変えるか
};
#endif