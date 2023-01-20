//=============================================================================
//
// バレットモデル処理 [buttlebullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BBULLET		(20)					// 弾数

#define	BBULLET_SIZE		(5.0f)				// 当たり判定の大きさ



//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct BUTTLEBULLET
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	bool				use;
	bool				load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				spd;				// 移動スピード
	float				size;				// 当たり判定の大きさ
	int					shadowIdx;			// 影のインデックス番号
	int					t;					//経過時間

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitButtleBullet(void);
void UninitButtleBullet(void);
void UpdateButtleBullet(void);
void DrawButtleBullet(void);

int SetButtleBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BUTTLEBULLET* GetButtleBullet(void);

