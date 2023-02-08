//=============================================================================
//
// 弾発射処理 [bullet.h]
// Author : 島村健剛
//
//=============================================================================
#pragma once
#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET		(256)	// 弾最大数
#define BULLET_SIZE		(5.0f)		//当たり判定サイズ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ワールドマトリックス
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	rot;			// 角度
	XMFLOAT3	scl;			// スケール
	MATERIAL	material;		// マテリアル
	float		spd;			// 移動量
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	int			shadowIdx;		// 影ID
	bool		use;			// 使用しているかどうか

	//XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	//XMFLOAT3			pos;				// モデルの位置
	//XMFLOAT3			rot;				// モデルの向き(回転)
	//XMFLOAT3			scl;				// モデルの大きさ(スケール)

	//bool				use;
	//bool				load;
	//DX11_MODEL			model;				// モデル情報
	//XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	//float				spd;				// 移動スピード
	//float				size;				// 当たり判定の大きさ
	//int					shadowIdx;			// 影のインデックス番号



} BULLET;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BULLET *GetBullet(void);

