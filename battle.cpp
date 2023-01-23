//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "battleplayer.h"
#include "battleenemy.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "buttlebullet.h"
#include "score.h"
#include "particle.h"
#include "collision.h"
#include "debugproc.h"
#include "enemy.h"
#include "command.h"
#include "shawar.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit2(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Battle = TYPE_FULL_SCREEN;

static bool	g_bPause = true;	// ポーズON/OFF


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle(void)
{
	g_ViewPortType_Battle = TYPE_FULL_SCREEN;

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitBattlePlayer();

	// エネミーの初期化
	InitBattleEnemy();

	// 壁の初期化
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// 壁(裏側用の半透明)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// 弾の初期化
	InitButtleBullet();

	// スコアの初期化
	//InitScore();

	InitShawar();

	InitCommand();


	// パーティクルの初期化
	InitParticle();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_sample001);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle(void)
{
	// パーティクルの終了処理
	UninitParticle();

	UninitCommand();

	UninitShawar();

	// スコアの終了処理
	//UninitScore();

	// 弾の終了処理
	UninitButtleBullet();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	// エネミーの終了処理
	UninitBattleEnemy();

	// プレイヤーの終了処理
	UninitBattlePlayer();

	// 影の終了処理
	UninitShadow();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Battle = (g_ViewPortType_Battle + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Battle);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? false : true;
	}


#endif

	if (g_bPause == false)
		return;

	// 地面処理の更新
	UpdateMeshField();

	UpdateCommand();

	// プレイヤーの更新処理
	UpdateBattlePlayer();

	// エネミーの更新処理
	UpdateBattleEnemy();

	// 壁処理の更新
	UpdateMeshWall();

	// 弾の更新処理
	UpdateButtleBullet();

	// パーティクルの更新処理
	UpdateParticle();

	UpdateShawar();

	// 影の更新処理
	UpdateShadow();

	// 当たり判定処理
	CheckHit2();

	// スコアの更新処理
	UpdateScore();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle0(void)
{
	// 3Dの物を描画する処理
	// 地面の描画処理
	DrawMeshField();

	// 影の描画処理
	DrawShadow();

	// エネミーの描画処理
	DrawBattleEnemy();

	// プレイヤーの描画処理
	DrawBattlePlayer();

	// 弾の描画処理
	DrawButtleBullet();

	// 壁の描画処理
	DrawMeshWall();

	// パーティクルの描画処理
	DrawParticle();

	DrawShawar();

	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(false);

	// ライティングを無効
	SetLightEnable(false);

	// スコアの描画処理
	DrawScore();

	DrawCommand();

	// ライティングを有効に
	SetLightEnable(true);

	// Z比較あり
	SetDepthEnable(true);
}


void DrawBattle(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Battle);

#endif

	// プレイヤー視点
	pos = GetbPlayer()->pos;
	pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	SetCameraAT(pos);
	SetCamera();

	switch (g_ViewPortType_Battle)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawBattle0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawBattle0();

		// エネミー視点
		pos = GetBattleEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawBattle0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawBattle0();

		// エネミー視点
		pos = GetBattleEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawBattle0();
		break;

	}

}


//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit2(void)
{
	bENEMY* benemy = GetBattleEnemy();		// バトル用エネミーのポインターを初期化
	ENEMY* enemy = GetEnemy();				//エネミーのポインターを初期化
	bPLAYER* bplayer = GetbPlayer();		// プレイヤーのポインターを初期化
	BUTTLEBULLET* bbullet = GetButtleBullet();	// 弾のポインターを初期化
	SHAWAR* shawar = GetShawar();				//水のポインターを初期化


	// 敵とプレイヤーキャラ
	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		//敵の有効フラグをチェックする
		if (benemy[i].use == false)
			continue;

		//BCの当たり判定
		if (CollisionBC(bplayer->pos, benemy[i].pos, bplayer->size, benemy[i].size))
		{
			// 敵キャラクターは倒される
			benemy[i].use = false;
			ReleaseShadow(benemy[i].shadowIdx);

			// スコアを足す
			AddScore(100);
		}
	}


	//弾と敵
	for (int i = 0; i < MAX_BBULLET; i++)
	{
		//弾の有効フラグをチェックする
		if (bbullet[i].use == false)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_B_ENEMY; j++)
		{
			//敵の有効フラグをチェックする
			if (benemy[j].use == false)
				continue;

			//BCの当たり判定
			if (CollisionBC(bbullet[i].pos, benemy[j].pos, bbullet[i].size, benemy[j].size))
			{
				// 当たったから弾は未使用に戻す
				bbullet[i].use = false;
				ReleaseShadow(bbullet[i].shadowIdx);

				// 敵キャラクターは倒される
				benemy[j].use = false;
				ReleaseShadow(enemy[j].shadowIdx);

				// スコアを足す
				AddScore(10);
			}
		}

	}

	//水と敵
	for (int i = 0; i < MAX_SHAWAR; i++)
	{
		//水の有効フラグをチェックする
		if (shawar[i].bUse == false)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_B_ENEMY; j++)
		{
			//敵の有効フラグをチェックする
			if (benemy[j].use == false)
				continue;

			//BCの当たり判定
			if (CollisionBC(shawar[i].pos, benemy[j].pos, SHAWAR_SIZE, benemy[j].size))
			{
				// 当たったから水は未使用に戻す
				shawar[i].bUse = false;
				ReleaseShadow(shawar[i].nIdxShadow);

				// 敵キャラクターは倒される
				benemy[j].use = false;
				ReleaseShadow(enemy[j].shadowIdx);

				// スコアを足す
				AddScore(10);
			}
		}

	}



	// バトル用エネミーが全部死亡したら状態遷移
	int benemy_count = 0;
	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		if (benemy[i].use == false) continue;
		benemy_count++;
	}

	// バトル用エネミーが０匹
	if (benemy_count == 0)
	{
		// ゲームモードエネミーが全部死亡したら状態遷移
		int enemy_count = 0;
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].use == false) continue;
			enemy_count++;
		}

		// ゲームモードエネミーが０匹？
		if (enemy_count == 0)
		{//リザルト画面へ
			SetFade(FADE_OUT, MODE_RESULT);
		}
		else//まだいる
		{//ゲームモードへ戻る
			SetFade(FADE_OUT, MODE_GAME);
		}
	}



}


