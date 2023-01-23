//=============================================================================
//
// パーティクル処理 [shawar.h]
// Author : 
//
//=============================================================================
#pragma once
#define	MAX_SHAWAR			(64)		// パーティクル最大数
#define	SHAWAR_SIZE			(15.0F)		//当たり判定

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

} SHAWAR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShawar(void);
void UninitShawar(void);
void UpdateShawar(void);
void DrawShawar(void);

int SetShawar(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorShawar(int nIdxParticle, XMFLOAT4 col);
SHAWAR* GetShawar(void);
