//=============================================================================
//
// バトル用エネミーモデル処理 [buttleenemy.cpp]
// Author : 島村健剛
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "battleenemy.h"
#include "shadow.h"
#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_B_ENEMY			"data/MODEL/makihito.obj"		// 読み込むモデル名


#define	VALUE_MOVE			(5.0f)								// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)						// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)								// 影の大きさ
#define ENEMY_OFFSET_Y		(3.0f)								// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static bENEMY			g_bEnemy[MAX_B_ENEMY];				// エネミー

static BOOL				g_bLoad = FALSE;




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattleEnemy(void)
{
	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		LoadModel(MODEL_B_ENEMY, &g_bEnemy[i].model);
		g_bEnemy[i].load = true;

		g_bEnemy[i].pos = XMFLOAT3(0.0f, ENEMY_OFFSET_Y, 30.0f);
		g_bEnemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_bEnemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_bEnemy[i].spd  = 0.0f;			// 移動スピードクリア
		g_bEnemy[i].size = bENEMY_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_bEnemy[0].model, &g_bEnemy[0].diffuse[0]);

		XMFLOAT3 pos = g_bEnemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_bEnemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_bEnemy[i].move_time = 0.0f;	// 線形補間用のタイマーをクリア
		g_bEnemy[i].tbl_adr = NULL;		// 再生するアニメデータの先頭アドレスをセット
		g_bEnemy[i].tbl_size = 0;		// 再生するアニメデータのレコード数をセット

		g_bEnemy[i].use = true;			// true:生きてる

	}



	g_bLoad = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattleEnemy(void)
{
	if (g_bLoad == FALSE) return;

	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		if (g_bEnemy[i].load)
		{
			UnloadModel(&g_bEnemy[i].model);
			g_bEnemy[i].load = false;
		}
	}

	g_bLoad = FALSE;

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattleEnemy(void)
{
	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		if (g_bEnemy[i].use == true)			// このエネミーが使われている？
		{										// Yes
			if (g_bEnemy[i].tbl_adr != NULL)	// 線形補間を実行する？
			{									// 線形補間の処理
				// 移動処理
				int		index = (int)g_bEnemy[i].move_time;
				float	time = g_bEnemy[i].move_time - index;
				int		size = g_bEnemy[i].tbl_size;

				float dt = 1.0f / g_bEnemy[i].tbl_adr[index].frame;		// 1フレームで進める時間
				g_bEnemy[i].move_time += dt;							// アニメーションの合計時間に足す

				if (index > (size - 2))	// ゴールをオーバーしていたら、最初へ戻す
				{
					g_bEnemy[i].move_time = 0.0f;
					index = 0;
				}

				// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
				XMVECTOR p1 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 1].pos);	// 次の場所
				XMVECTOR p0 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 0].pos);	// 現在の場所
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_bEnemy[i].pos, p0 + vec * time);

				// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
				XMVECTOR r1 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 1].rot);	// 次の角度
				XMVECTOR r0 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 0].rot);	// 現在の角度
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_bEnemy[i].rot, r0 + rot * time);

				// scaleを求める S = StartX + (EndX - StartX) * 今の時間
				XMVECTOR s1 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 1].scl);	// 次のScale
				XMVECTOR s0 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 0].scl);	// 現在のScale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_bEnemy[i].scl, s0 + scl * time);

			}


			// レイキャストして足元の高さを求める
			XMFLOAT3 HitPosition;		// 交点
			XMFLOAT3 Normal;			// ぶつかったポリゴンの法線ベクトル（向き）
			bool ans = RayHitField(g_bEnemy[i].pos, &HitPosition, &Normal);
			if (ans)
			{
				g_bEnemy[i].pos.y = HitPosition.y + ENEMY_OFFSET_Y;
			}
			else
			{
				g_bEnemy[i].pos.y = ENEMY_OFFSET_Y;
				Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			}


			// 影もエネミーの位置に合わせる
			XMFLOAT3 pos = g_bEnemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_bEnemy[i].shadowIdx, pos);
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattleEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		if (g_bEnemy[i].use == false) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_bEnemy[i].scl.x, g_bEnemy[i].scl.y, g_bEnemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_bEnemy[i].rot.x, g_bEnemy[i].rot.y + XM_PI, g_bEnemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_bEnemy[i].pos.x, g_bEnemy[i].pos.y, g_bEnemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_bEnemy[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_bEnemy[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
bENEMY* GetBattleEnemy()
{
	return &g_bEnemy[0];
}
