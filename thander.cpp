//=============================================================================
//
// 雷用ポリゴン処理 [thander.cpp]
// Author : 島村健剛
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "thander.h"
#include "player.h"
#include "enemy.h"
#include "battleenemy.h"
#include "battleplayer.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数

#define	THANDER_SIZE_X		(5.0f)		// 頂点サイズ
#define	THANDER_SIZE_Y		(200.0f)		// 頂点サイズ
#define	VALUE_MOVE_THANDER	(5.0f)		// 移動速度


#define	DISP_SHADOW						// 影の表示
//#undef DISP_SHADOW


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexThander(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点バッファ

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;						// テクスチャ番号

static THANDER					g_aThander[MAX_THANDER];				// パーティクルワーク
static XMFLOAT3					g_posBase;							// ビルボード発生位置
static float					g_fWidthBase = 5.0f;				// 基準の幅
static float					g_fHeightBase = 10.0f;				// 基準の高さ
static float					g_roty = 0.0f;						// 移動方向
static float					g_spd = 0.0f;						// 移動スピード

static char* g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/thander.png",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitThander(void)
{
	// 頂点情報の作成
	MakeVertexThander();

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// パーティクルワークの初期化
	for (int nCntThander = 0; nCntThander < MAX_THANDER; nCntThander++)
	{
		g_aThander[nCntThander].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aThander[nCntThander].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aThander[nCntThander].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aThander[nCntThander].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aThander[nCntThander].material, sizeof(g_aThander[nCntThander].material));
		g_aThander[nCntThander].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aThander[nCntThander].fSizeX = THANDER_SIZE_X;
		g_aThander[nCntThander].fSizeY = THANDER_SIZE_Y;
		g_aThander[nCntThander].nIdxShadow = -1;
		g_aThander[nCntThander].nLife = 0;
		g_aThander[nCntThander].bUse = false;
	}

	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitThander(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// 頂点バッファの解放
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateThander(void)
{

	for (int nCntThander = 0; nCntThander < MAX_THANDER; nCntThander++)
	{
		if (g_aThander[nCntThander].bUse)
		{	// 使用中
			//g_aThander[nCntThander].pos.x += g_aThander[nCntThander].move.x;
			//g_aThander[nCntThander].pos.z += g_aThander[nCntThander].move.z;
			//g_aThander[nCntThander].pos.y += g_aThander[nCntThander].move.y;

			//g_aThander[nCntThander].move.x += (0.0f - g_aThander[nCntThander].move.x) * 0.015f;
			//g_aThander[nCntThander].move.y -= (0.0f - g_aThander[nCntThander].move.y) * 0.015f;
			//g_aThander[nCntThander].move.z += 0.25f;

#ifdef DISP_SHADOW
			if (g_aThander[nCntThander].nIdxShadow != -1)
			{// 影使用中
				float colA;

				// 影の位置設定
				SetPositionShadow(g_aThander[nCntThander].nIdxShadow, XMFLOAT3(g_aThander[nCntThander].pos.x, 0.1f, g_aThander[nCntThander].pos.z));

				// 影の色の設定
				colA = g_aThander[nCntThander].material.Diffuse.w;
				SetColorShadow(g_aThander[nCntThander].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
			}
#endif

			g_aThander[nCntThander].nLife--;
			if (g_aThander[nCntThander].nLife <= 0)
			{
				g_aThander[nCntThander].bUse = false;
				ReleaseShadow(g_aThander[nCntThander].nIdxShadow);
				g_aThander[nCntThander].nIdxShadow = -1;
			}
			else
			{
				if (g_aThander[nCntThander].nLife <= 20)
				{
					// α値設定
					g_aThander[nCntThander].material.Diffuse.w -= 0.05f;
					if (g_aThander[nCntThander].material.Diffuse.w < 0.0f)
					{
						g_aThander[nCntThander].material.Diffuse.w = 0.0f;
					}
				}
			}
		}
	}




}

//=============================================================================
// 描画処理
//=============================================================================
void DrawThander(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	// ライティングを無効に
	SetLightEnable(false);

	// 加算合成に設定
	SetBlendState(BLEND_MODE_ADD);

	// Z比較無し
	SetDepthEnable(false);

	// フォグ無効
	SetFogEnable(false);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for (int nCntThander = 0; nCntThander < MAX_THANDER; nCntThander++)
	{
		if (g_aThander[nCntThander].bUse)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			//mtxWorld = XMMatrixInverse(nullptr, mtxView);
			//mtxWorld.r[3].m128_f32[0] = 0.0f;
			//mtxWorld.r[3].m128_f32[1] = 0.0f;
			//mtxWorld.r[3].m128_f32[2] = 0.0f;

			// 処理が速いしお勧め
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// スケールを反映
			mtxScl = XMMatrixScaling(g_aThander[nCntThander].scale.x, g_aThander[nCntThander].scale.y, g_aThander[nCntThander].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_aThander[nCntThander].pos.x, g_aThander[nCntThander].pos.y, g_aThander[nCntThander].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_aThander[nCntThander].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ライティングを有効に
	SetLightEnable(true);

	// 通常ブレンドに戻す
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	SetDepthEnable(true);

	// フォグ有効
	SetFogEnable(true);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexThander(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-THANDER_SIZE_X / 2, THANDER_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(THANDER_SIZE_X / 2, THANDER_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-THANDER_SIZE_X / 2, -THANDER_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(THANDER_SIZE_X / 2, -THANDER_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorThander(int nIdxThander, XMFLOAT4 col)
{
	g_aThander[nIdxThander].material.Diffuse = col;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetThander(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxThander = -1;

	for (int nCntThander = 0; nCntThander < MAX_THANDER; nCntThander++)
	{
		if (!g_aThander[nCntThander].bUse)
		{
			g_aThander[nCntThander].pos = pos;
			g_aThander[nCntThander].rot = { 0.0f, 0.0f, 0.0f };
			g_aThander[nCntThander].scale = { 1.0f, 1.0f, 1.0f };
			g_aThander[nCntThander].move = move;
			g_aThander[nCntThander].material.Diffuse = col;
			g_aThander[nCntThander].fSizeX = fSizeX;
			g_aThander[nCntThander].fSizeY = fSizeY;
			g_aThander[nCntThander].nLife = nLife;
			g_aThander[nCntThander].bUse = true;

			nIdxThander = nCntThander;

#ifdef DISP_SHADOW
			// 影の設定
			g_aThander[nCntThander].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
			if (g_aThander[nCntThander].nIdxShadow != -1)
			{
				SetColorShadow(g_aThander[nCntThander].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxThander;
}

THANDER* GetThander(void)
{
	return &g_aThander[0];

}
