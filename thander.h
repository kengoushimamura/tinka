//=============================================================================
//
// 雷用ポリゴン処理 [thander.h]
// Author : 島村健剛
//
//=============================================================================
#pragma once
#define	MAX_THANDER			(64)		// パーティクル最大数
#define	THANDER_SIZE			(15.0F)		//当たり判定

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scale;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	int				nIdxShadow;		// 影ID
	int				nLife;			// 寿命
	bool			bUse;			// 使用しているかどうか

} THANDER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitThander(void);
void UninitThander(void);
void UpdateThander(void);
void DrawThander(void);

int SetThander(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorThander(int nIdxParticle, XMFLOAT4 col);
THANDER* GetThander(void);
