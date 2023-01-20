//=============================================================================
//
// バレットモデル処理 [buttlebullet.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "buttlebullet.h"
#include "sound.h"
#include "battleplayer.h"
#include "battleenemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BULLET			"data/MODEL/aquabullet.obj"	// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量
#define	BUTTLEBULLET_SPEED		(5.0f)						// 弾の移動スピード


#define BULLET_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define BULLET_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BUTTLEBULLET			g_bBullet[MAX_BBULLET];				// 弾

static BOOL				g_Load = FALSE;




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitButtleBullet(void)
{
	for (int i = 0; i < MAX_BBULLET; i++)
	{

		g_bBullet[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_bBullet[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_bBullet[i].scl = XMFLOAT3(0.0f, 0.0f, 0.0f);

		g_bBullet[i].spd = BUTTLEBULLET_SPEED;			// 移動スピードクリア
		g_bBullet[i].size = BBULLET_SIZE;	// 当たり判定の大きさ

		g_bBullet[i].t = 0;

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_bBullet[0].model, &g_bBullet[0].diffuse[0]);

		g_bBullet[i].use = false;			// 未使用

	}



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitButtleBullet(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_BBULLET; i++)
	{
		if (g_bBullet[i].load)
		{
			UnloadModel(&g_bBullet[i].model);
			g_bBullet[i].load = false;
		}
	}

	g_Load = FALSE;

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateButtleBullet(void)
{

	bENEMY* enemy = GetBattleEnemy();
	bPLAYER* player = GetbPlayer();

	for (int i = 0; i < MAX_BBULLET; i++)
	{
		for (int j = 0; j < MAX_B_ENEMY; j++)
		{
			if (g_bBullet[i].use == true)
			{
				//XMFLOAT3 epos = enemy[j].pos;
				//XMFLOAT3 ppos = player[0].pos;
				//XMFLOAT3 mpos;
				//mpos.x = (epos.x + ppos.x) / 2;
				//mpos.y = (epos.y + ppos.y) / 2;
				//mpos.z = (epos.z + ppos.z) / 2;
				//XMVECTOR temp = XMLoadFloat3(&epos) - XMLoadFloat3(&ppos);
				//temp = XMVector3LengthSq(temp);			// 2点間の距離（2乗した物）
				//float lenSq = 0.0f;
				//XMStoreFloat(&lenSq, temp);

				//XMFLOAT3 apos;
				//XMFLOAT3 bpos;

				//apos.x = (1 - g_bBullet[i].t) * ppos.x + g_bBullet[i].t * mpos.x;
				//apos.y = (1 - g_bBullet[i].t) * ppos.y + g_bBullet[i].t * mpos.y;
				//apos.z = (1 - g_bBullet[i].t) * ppos.z + g_bBullet[i].t * mpos.z;
				//bpos.x = (1 - g_bBullet[i].t) * mpos.x + g_bBullet[i].t * epos.x;
				//bpos.y = (1 - g_bBullet[i].t) * mpos.y + g_bBullet[i].t * epos.y;
				//bpos.z = (1 - g_bBullet[i].t) * mpos.z + g_bBullet[i].t * epos.z;

				//g_bBullet[i].pos.x -= (1 - g_bBullet[i].t) * apos.x + g_bBullet[i].t * bpos.x;
				//g_bBullet[i].pos.y -= (1 - g_bBullet[i].t) * apos.y + g_bBullet[i].t * bpos.y;
				//g_bBullet[i].pos.z -= (1 - g_bBullet[i].t) * apos.z + g_bBullet[i].t * bpos.z;

				// 弾の移動処理
				g_bBullet[i].pos.x -= sinf(g_bBullet[i].rot.y) * g_bBullet[i].spd;
				g_bBullet[i].pos.z -= cosf(g_bBullet[i].rot.y) * g_bBullet[i].spd;

				// 影の位置設定
				SetPositionShadow(g_bBullet[i].shadowIdx, XMFLOAT3(g_bBullet[i].pos.x, 0.1f, g_bBullet[i].pos.z));


				// フィールドの外に出たら弾を消す処理
				if (g_bBullet[i].pos.x < MAP_LEFT
					|| g_bBullet[i].pos.x > MAP_RIGHT
					|| g_bBullet[i].pos.z < MAP_DOWN
					|| g_bBullet[i].pos.z > MAP_TOP)
				{
					g_bBullet[i].use = false;
					ReleaseShadow(g_bBullet[i].shadowIdx);
				}
				g_bBullet[i].t += 0.01f;

			}
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawButtleBullet(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BBULLET; i++)
	{
		if (g_bBullet[i].use == false) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_bBullet[i].scl.x, g_bBullet[i].scl.y, g_bBullet[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_bBullet[i].rot.x, g_bBullet[i].rot.y + XM_PI, g_bBullet[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_bBullet[i].pos.x, g_bBullet[i].pos.y, g_bBullet[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_bBullet[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_bBullet[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// 弾のパラメータをセット
//=============================================================================
int SetButtleBullet(XMFLOAT3 pos, XMFLOAT3 rot)
{
	int nIdxBullet = -1;

	for (int nCntBullet = 0; nCntBullet < MAX_BBULLET; nCntBullet++)
	{
		if (!g_bBullet[nCntBullet].use)
		{
			LoadModel(MODEL_BULLET, &g_bBullet[nCntBullet].model);
			g_bBullet[nCntBullet].load = true;

			g_bBullet[nCntBullet].pos = pos;
			g_bBullet[nCntBullet].rot = rot;
			g_bBullet[nCntBullet].scl = { 0.7f, 0.7f, 0.7f };
			g_bBullet[nCntBullet].use = true;

			// 影の設定
			g_bBullet[nCntBullet].shadowIdx = CreateShadow(g_bBullet[nCntBullet].pos, 0.5f, 0.5f);

			nIdxBullet = nCntBullet;

			PlaySound(SOUND_LABEL_SE_shot000);

			break;
		}
	}

	return nIdxBullet;
}


//=============================================================================
// エネミーの取得
//=============================================================================
BUTTLEBULLET* GetButtleBullet()
{
	return &g_bBullet[0];
}
