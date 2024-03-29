//=============================================================================
//
// バトル用プレイヤーモデル処理 [battleplayer.h]
// Author : 島村健剛
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_bPLAYER		(1)					// プレイヤーの数

#define	bPLAYER_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct bPLAYER
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	float				spd;				// 移動スピード

	BOOL				load;
	BOOL				use;

	DX11_MODEL			model;				// モデル情報

	int					shadowIdx;			// 影のインデックス番号


	float				size;

	// 階層アニメーション用のメンバー変数
	float				move_time;			// 実行時間
	int					tblNo;		// 行動データのテーブル番号
	int					tblMax;		// そのテーブルのデータ数


	// 親は、NULL、子供は親のアドレスを入れる
	int					parent;		//構造体からポインターを消したい

	// クォータニオン
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// 自分が立っている所

};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattlePlayer(void);
void UninitBattlePlayer(void);
void UpdateBattlePlayer(void);
void DrawBattlePlayer(void);

bPLAYER* GetbPlayer(void);
