//=============================================================================
//
// シャワー用パーティクルエフェクト処理 [shawar.cpp]
// Author : 島村健剛
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "shawar.h"
#include "player.h"
#include "enemy.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数

#define	SHAWAR_SIZE_X		(40.0f)		// 頂点サイズ
#define	SHAWAR_SIZE_Y		(40.0f)		// 頂点サイズ
#define	VALUE_MOVE_SHAWAR	(5.0f)		// 移動速度


#define	DISP_SHADOW						// 影の表示
//#undef DISP_SHADOW


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexShawar(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点バッファ

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;						// テクスチャ番号

static SHAWAR					g_aShawar[MAX_SHAWAR];				// パーティクルワーク
static XMFLOAT3					g_posBase;							// ビルボード発生位置
static float					g_fWidthBase = 5.0f;				// 基準の幅
static float					g_fHeightBase = 10.0f;				// 基準の高さ
static float					g_roty = 0.0f;						// 移動方向
static float					g_spd = 0.0f;						// 移動スピード

static char* g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShawar(void)
{
	// 頂点情報の作成
	MakeVertexShawar();

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
	for (int nCntShawar = 0; nCntShawar < MAX_SHAWAR; nCntShawar++)
	{
		g_aShawar[nCntShawar].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aShawar[nCntShawar].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aShawar[nCntShawar].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aShawar[nCntShawar].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aShawar[nCntShawar].material, sizeof(g_aShawar[nCntShawar].material));
		g_aShawar[nCntShawar].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aShawar[nCntShawar].fSizeX = SHAWAR_SIZE_X;
		g_aShawar[nCntShawar].fSizeY = SHAWAR_SIZE_Y;
		g_aShawar[nCntShawar].nIdxShadow = -1;
		g_aShawar[nCntShawar].nLife = 0;
		g_aShawar[nCntShawar].bUse = false;
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
void UninitShawar(void)
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
void UpdateShawar(void)
{

	for (int nCntShawar = 0; nCntShawar < MAX_SHAWAR; nCntShawar++)
	{
		if (g_aShawar[nCntShawar].bUse)
		{	// 使用中
			g_aShawar[nCntShawar].pos.x -= g_aShawar[nCntShawar].move.x * sinf(g_aShawar[nCntShawar].rot.y);
			g_aShawar[nCntShawar].pos.z -= g_aShawar[nCntShawar].move.z * cosf(g_aShawar[nCntShawar].rot.y);
			g_aShawar[nCntShawar].pos.y -= g_aShawar[nCntShawar].move.y;

			g_aShawar[nCntShawar].move.x += (0.0f - g_aShawar[nCntShawar].move.x) * 0.015f;
			g_aShawar[nCntShawar].move.y += (0.0f - g_aShawar[nCntShawar].move.y) * 0.015f;
			g_aShawar[nCntShawar].move.z += (0.0f - g_aShawar[nCntShawar].move.z) * 0.015f;

#ifdef DISP_SHADOW
			if (g_aShawar[nCntShawar].nIdxShadow != -1)
			{// 影使用中
				float colA;

				// 影の位置設定
				SetPositionShadow(g_aShawar[nCntShawar].nIdxShadow, XMFLOAT3(g_aShawar[nCntShawar].pos.x, 0.1f, g_aShawar[nCntShawar].pos.z));

				// 影の色の設定
				colA = g_aShawar[nCntShawar].material.Diffuse.w;
				SetColorShadow(g_aShawar[nCntShawar].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
			}
#endif

			g_aShawar[nCntShawar].nLife--;
			if (g_aShawar[nCntShawar].nLife <= 0)
			{
				g_aShawar[nCntShawar].bUse = false;
				ReleaseShadow(g_aShawar[nCntShawar].nIdxShadow);
				g_aShawar[nCntShawar].nIdxShadow = -1;
			}
			else
			{
				if (g_aShawar[nCntShawar].nLife <= 20)
				{
					// α値設定
					g_aShawar[nCntShawar].material.Diffuse.w -= 0.05f;
					if (g_aShawar[nCntShawar].material.Diffuse.w < 0.0f)
					{
						g_aShawar[nCntShawar].material.Diffuse.w = 0.0f;
					}
				}
			}
		}
	}



	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShawar(void)
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

	for (int nCntShawar = 0; nCntShawar < MAX_SHAWAR; nCntShawar++)
	{
		if (g_aShawar[nCntShawar].bUse)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// ビルボード処理
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
			mtxScl = XMMatrixScaling(g_aShawar[nCntShawar].scale.x, g_aShawar[nCntShawar].scale.y, g_aShawar[nCntShawar].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_aShawar[nCntShawar].pos.x, g_aShawar[nCntShawar].pos.y, g_aShawar[nCntShawar].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_aShawar[nCntShawar].material);

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
HRESULT MakeVertexShawar(void)
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
		vertex[0].Position = XMFLOAT3(-SHAWAR_SIZE_X / 2, SHAWAR_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(SHAWAR_SIZE_X / 2, SHAWAR_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-SHAWAR_SIZE_X / 2, -SHAWAR_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(SHAWAR_SIZE_X / 2, -SHAWAR_SIZE_Y / 2, 0.0f);

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
void SetColorShawar(int nIdxShawar, XMFLOAT4 col)
{
	g_aShawar[nIdxShawar].material.Diffuse = col;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetShawar(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT3 rot, float fSizeX, float fSizeY, int nLife)
{
	int nIdxShawar = -1;

	for (int nCntShawar = 0; nCntShawar < MAX_SHAWAR; nCntShawar++)
	{
		if (!g_aShawar[nCntShawar].bUse)
		{
			g_aShawar[nCntShawar].pos = pos;
			g_aShawar[nCntShawar].rot = rot;
			g_aShawar[nCntShawar].scale = { 1.0f, 1.0f, 1.0f };
			g_aShawar[nCntShawar].move = move;
			g_aShawar[nCntShawar].material.Diffuse = col;
			g_aShawar[nCntShawar].fSizeX = fSizeX;
			g_aShawar[nCntShawar].fSizeY = fSizeY;
			g_aShawar[nCntShawar].nLife = nLife;
			g_aShawar[nCntShawar].bUse = true;

			nIdxShawar = nCntShawar;

#ifdef DISP_SHADOW
			// 影の設定
			g_aShawar[nCntShawar].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
			if (g_aShawar[nCntShawar].nIdxShadow != -1)
			{
				SetColorShadow(g_aShawar[nCntShawar].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxShawar;
}

SHAWAR* GetShawar(void)
{
	return &g_aShawar[0];

}
