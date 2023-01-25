//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "battleplayer.h"
#include "shadow.h"
#include "buttlebullet.h"
#include "debugproc.h"
#include "meshfield.h"
#include "command.h"
#include "shawar.h"
#include "battleenemy.h"
#include "thander.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_bPLAYER		"data/MODEL/body2.obj"			// 読み込むモデル名
#define	MODEL_bPLAYER_r_arm	"data/MODEL/r_arm2.obj"			// 読み込むモデル名
#define	MODEL_bPLAYER_l_arm	"data/MODEL/l_arm2.obj"			// 読み込むモデル名
#define	MODEL_bPLAYER_r_leg	"data/MODEL/leg2.obj"			// 読み込むモデル名
#define	MODEL_bPLAYER_l_leg	"data/MODEL/leg2.obj"			// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 回転量

#define bPLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(14.0f)							// プレイヤーの足元をあわせる

#define bPLAYER_PARTS_MAX	(4)								// プレイヤーのパーツの数



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static bPLAYER		g_bPlayer;						// プレイヤー

static bPLAYER		g_Parts[bPLAYER_PARTS_MAX];		// プレイヤーのパーツ用

static float		roty = 0.0f;

static LIGHT		g_Light;





// プレイヤーの階層アニメーションデータ

//右腕のアニメーション
static INTERPOLATION_DATA move_tbl_right_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     XM_PI / 4),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     XM_PI / 2),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2, XM_PI * 3 / 4),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     XM_PI / 2),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },

};

//左腕のアニメーション
static INTERPOLATION_DATA move_tbl_left_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     -XM_PI / 4),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     -XM_PI / 2),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2, -XM_PI * 3 / 4),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     -XM_PI / 2),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

//右足のアニメーション
static INTERPOLATION_DATA move_tbl_right_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(XM_PI / 4, 0.0f,  0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(0.0f,      0.0f,  0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(0.0f,      0.0f,  0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },

};

//左足のアニメーション
static INTERPOLATION_DATA move_tbl_left_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(2.5f,  -5.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(2.5f,  -5.0f, 0.0f), XMFLOAT3(0.0f,       0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(2.5f,  -5.0f, 0.0f), XMFLOAT3(XM_PI / 4,  0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(2.5f,  -5.0f, 0.0f), XMFLOAT3(0.0f,       0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	move_tbl_right_arm,
	move_tbl_left_arm,
	move_tbl_right_leg,
	move_tbl_left_leg,
};

//構造体の中からポインターを消したい
static bPLAYER* Parent[] =
{
	&g_bPlayer,
};





//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattlePlayer(void)
{
	g_bPlayer.load = TRUE;
	LoadModel(MODEL_bPLAYER, &g_bPlayer.model);

	g_bPlayer.pos = XMFLOAT3(-10.0f, PLAYER_OFFSET_Y + 50.0f, -50.0f);
	g_bPlayer.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_bPlayer.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_bPlayer.spd = 0.0f;			// 移動スピードクリア

	g_bPlayer.use = TRUE;			// true:生きてる
	g_bPlayer.size = bPLAYER_SIZE;	// 当たり判定の大きさ

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_bPlayer.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_bPlayer.shadowIdx = CreateShadow(pos, bPLAYER_SHADOW_SIZE, bPLAYER_SHADOW_SIZE);

	// キーを押した時のプレイヤーの向き
	roty = 0.0f;

	g_bPlayer.parent = -1;				// 本体（親）なので変なのが入らないようにー１


	// 階層アニメーションの初期化
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		g_Parts[i].parent = -1;			// ← ここに親のアドレスを入れる　

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].move_time = 0.0f;	// 実行時間をクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット



		// パーツの読み込み
		g_Parts[i].load = FALSE;
	}


	//右腕の起動
	g_Parts[0].pos = XMFLOAT3(-5.0f, 5.0f, 0.0f);
	g_Parts[0].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
	g_Parts[0].use = true;
	g_Parts[0].parent = 0;																	// ← ここに親のアドレスを入れる
	g_Parts[0].tblNo = 0;																	// 再生する行動データテーブルNoをセット
	g_Parts[0].tblMax = sizeof(move_tbl_right_arm) / sizeof(INTERPOLATION_DATA);			// 再生する行動データテーブルのレコード数をセット
	g_Parts[0].load = true;
	LoadModel(MODEL_bPLAYER_r_arm, &g_Parts[0].model);

	//左腕の起動
	g_Parts[1].pos = XMFLOAT3(5.0f, 5.0f, 0.0f);
	g_Parts[1].rot = XMFLOAT3(0.0f, XM_PI / 2, -XM_PI / 2);
	g_Parts[1].use = true;
	g_Parts[1].parent = 0;																// ← ここに親のアドレスを入れる
	g_Parts[1].tblNo = 1;																// 再生する行動データテーブルNoをセット
	g_Parts[1].tblMax = sizeof(move_tbl_left_arm) / sizeof(INTERPOLATION_DATA);			// 再生する行動データテーブルのレコード数をセット
	g_Parts[1].load = true;
	LoadModel(MODEL_bPLAYER_l_arm, &g_Parts[1].model);

	//右足の起動
	g_Parts[2].pos = XMFLOAT3(-2.5f, -5.0f, 0.0f);
	g_Parts[2].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	g_Parts[2].use = true;
	g_Parts[2].parent = 0;																	// ← ここに親のアドレスを入れる
	g_Parts[2].tblNo = 2;																	// 再生する行動データテーブルNoをセット
	g_Parts[2].tblMax = sizeof(move_tbl_right_leg) / sizeof(INTERPOLATION_DATA);			// 再生する行動データテーブルのレコード数をセット
	g_Parts[2].load = true;
	LoadModel(MODEL_bPLAYER_r_leg, &g_Parts[2].model);

	//左足の起動
	g_Parts[3].pos = XMFLOAT3(2.5f, -5.0f, 0.0f);
	g_Parts[3].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	g_Parts[3].use = true;
	g_Parts[3].parent = 0;																// ← ここに親のアドレスを入れる
	g_Parts[3].tblNo = 3;																// 再生する行動データテーブルNoをセット
	g_Parts[3].tblMax = sizeof(move_tbl_left_leg) / sizeof(INTERPOLATION_DATA);			// 再生する行動データテーブルのレコード数をセット
	g_Parts[3].load = true;
	LoadModel(MODEL_bPLAYER_l_leg, &g_Parts[3].model);


	// クォータニオンの初期化
	XMStoreFloat4(&g_bPlayer.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattlePlayer(void)
{
	// モデルの解放処理
	if (g_bPlayer.load == TRUE)
	{
		UnloadModel(&g_bPlayer.model);
		g_bPlayer.load = FALSE;
	}

	// パーツの解放処理
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// パーツの解放処理
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattlePlayer(void)
{
	CAMERA* cam = GetCamera();
	bENEMY* enemy = GetBattleEnemy();


	int* cmd = GetCommand();

	g_bPlayer.spd *= 0.1f;


#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_bPlayer.pos.z = g_bPlayer.pos.x = 0.0f;
		g_bPlayer.spd = 0.0f;
		roty = 0.0f;
	}
#endif


	// レイキャストして足元の高さを求める
	XMFLOAT3 HitPosition;		// 交点
	XMFLOAT3 Normal;			// ぶつかったポリゴンの法線ベクトル（向き）
	bool ans = RayHitField(g_bPlayer.pos, &HitPosition, &Normal);
	if (ans)
	{
		g_bPlayer.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_bPlayer.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}


	// 弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		if (cmd[0] == 0)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move = {0.0f,0.0f,0.0f};
			float fAngle, fLength;
			int nLife;
			float fSizey,fSizex;

			pos = enemy[0].pos;


			nLife = rand() % 5 + 5;

			fSizey = (float)(rand() % 30 + 120);
			fSizex = fSizey / 5;


			SetThander(pos, move, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.85f), fSizex, fSizey, nLife);
		}
		if (cmd[0] == 1)
		{
			SetButtleBullet(g_bPlayer.pos, g_bPlayer.rot);
		}
		if (cmd[0] == 2)
		{
			for (int j = 0; j < 10; j++)
			{
				XMFLOAT3 pos;
				XMFLOAT3 move;
				float fAngle, fLength;
				int nLife;
				float fSize;

				pos = g_bPlayer.pos;

				fAngle = (float)(rand() % 628 - 314) / 100.0f;
				fLength = rand() % (int)(5.0f * 200) / 100.0f - 5.0f;
				move.x = sinf(fAngle) * fLength;
				move.z = rand() % 300 / 100.0f + 10.0f;
				move.y = cosf(fAngle) * fLength;

				nLife = rand() % 5 + 5;

				fSize = (float)(rand() % 30 + 20);

				pos.y = fSize / 2;

				// ビルボードの設定
				SetShawar(pos, move, XMFLOAT4(0.0f, 0.0f, 1.0f, 0.85f), fSize, fSize, nLife);

			}
		}
	}
	if (IsButtonTriggered(0, BUTTON_B))
	{
		if (cmd[0] == 0)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move = { 0.0f,0.0f,0.0f };
			float fAngle, fLength;
			int nLife;
			float fSizey, fSizex;

			pos = enemy[0].pos;

			nLife = rand() % 5 + 5;

			fSizey = (float)(rand() % 30 + 120);
			fSizex = fSizey / 5;

			SetThander(pos, move, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.85f), fSizex, fSizey, nLife);
		}
		if (cmd[0] == 1)
		{
			SetButtleBullet(g_bPlayer.pos, g_bPlayer.rot);
		}
		if (cmd[0] == 2)
		{
			for (int j = 0; j < 10; j++)
			{
				XMFLOAT3 pos;
				XMFLOAT3 move;
				float fAngle, fLength;
				int nLife;
				float fSize;

				pos = g_bPlayer.pos;

				fAngle = (float)(rand() % 628 - 314) / 100.0f;
				fLength = rand() % (int)(5.0f * 200) / 100.0f - 5.0f;
				move.x = sinf(fAngle) * fLength;
				move.z = rand() % 300 / 100.0f + 10.0f;
				move.y = cosf(fAngle) * fLength;

				nLife = rand() % 5 + 5;

				fSize = (float)(rand() % 30 + 20);

				pos.y = fSize / 2;

				// ビルボードの設定
				SetShawar(pos, move, XMFLOAT4(0.0f, 0.0f, 1.0f, 0.85f), fSize, fSize, nLife);

			}
		}
	}


	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_bPlayer.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_bPlayer.shadowIdx, pos);


	// 階層アニメーション
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		// 使われているなら処理する
		if (g_Parts[i].use == TRUE)
		{
			//// 移動処理
			int		index = (int)g_Parts[i].move_time;
			float	time = g_Parts[i].move_time - index;
			int		size = sizeof(move_tbl_right_arm) / sizeof(INTERPOLATION_DATA);

			float dt = 1.0f / g_MoveTblAdr[g_Parts[i].tblNo][index].frame;				// 1フレームで進める時間
			g_Parts[i].move_time += dt;													// アニメーションの合計時間に足す

			if (index > (size - 2))														// ゴールをオーバーしていたら、最初へ戻す
			{
				g_Parts[i].move_time = 0.0f;
				index = 0;
			}

			// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
			XMVECTOR p1 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 1].pos);	// 次の場所
			XMVECTOR p0 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 0].pos);	// 現在の場所
			XMVECTOR vec = p1 - p0;
			XMStoreFloat3(&g_Parts[i].pos, p0 + vec * time);

			// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
			XMVECTOR r1 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 1].rot);	// 次の角度
			XMVECTOR r0 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 0].rot);	// 現在の角度
			XMVECTOR rot = r1 - r0;
			XMStoreFloat3(&g_Parts[i].rot, r0 + rot * time);

			// scaleを求める S = StartX + (EndX - StartX) * 今の時間
			XMVECTOR s1 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 1].scl);	// 次のScale
			XMVECTOR s0 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 0].scl);	// 現在のScale
			XMVECTOR scl = s1 - s0;
			XMStoreFloat3(&g_Parts[i].scl, s0 + scl * time);

		}
	}

	if (g_bPlayer.spd == 0.0f)
	{	//動いてないときはinitのポーズ
		g_Parts[0].pos = XMFLOAT3(-5.0f, 5.0f, 0.0f);
		g_Parts[0].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
		g_Parts[1].pos = XMFLOAT3(5.0f, 5.0f, 0.0f);
		g_Parts[1].rot = XMFLOAT3(0.0f, XM_PI / 2, -XM_PI / 2);
		g_Parts[2].pos = XMFLOAT3(-2.5f, -5.0f, 0.0f);
		g_Parts[2].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
		g_Parts[3].pos = XMFLOAT3(2.5f, -5.0f, 0.0f);
		g_Parts[3].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	}



	// ポイントライト
	{
		LIGHT* light = GetLightData(1);
		XMFLOAT3 pos = g_bPlayer.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}



	//////////////////////////////////////////////////////////////////////
	// 姿勢制御
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_bPlayer.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_bPlayer.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_bPlayer.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_bPlayer.Quaternion, quat);




#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_bPlayer.pos.x, g_bPlayer.pos.y, g_bPlayer.pos.z, Normal.y);
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattlePlayer(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_bPlayer.scl.x, g_bPlayer.scl.y, g_bPlayer.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_bPlayer.rot.x, g_bPlayer.rot.y + XM_PI, g_bPlayer.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_bPlayer.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_bPlayer.pos.x, g_bPlayer.pos.y, g_bPlayer.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_bPlayer.mtxWorld, mtxWorld);


	// 縁取りの設定
	SetFuchi(1);

	// モデル描画
	DrawModel(&g_bPlayer.model);

	// 階層アニメーション
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent == 0)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&Parent[g_Parts[i].parent]->mtxWorld));
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する
		if (g_Parts[i].use == false) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);


		// モデル描画
		DrawModel(&g_Parts[i].model);

	}

	SetFuchi(0);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
bPLAYER* GetbPlayer(void)
{
	return &g_bPlayer;
}
