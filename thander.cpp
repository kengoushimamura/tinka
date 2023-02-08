//=============================================================================
//
// ���p�|���S������ [thander.cpp]
// Author : ��������
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�

#define	THANDER_SIZE_X		(5.0f)		// ���_�T�C�Y
#define	THANDER_SIZE_Y		(200.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_THANDER	(5.0f)		// �ړ����x


#define	DISP_SHADOW						// �e�̕\��
//#undef DISP_SHADOW


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexThander(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_�o�b�t�@

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;						// �e�N�X�`���ԍ�

static THANDER					g_aThander[MAX_THANDER];				// �p�[�e�B�N�����[�N
static XMFLOAT3					g_posBase;							// �r���{�[�h�����ʒu
static float					g_fWidthBase = 5.0f;				// ��̕�
static float					g_fHeightBase = 10.0f;				// ��̍���
static float					g_roty = 0.0f;						// �ړ�����
static float					g_spd = 0.0f;						// �ړ��X�s�[�h

static char* g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/thander.png",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitThander(void)
{
	// ���_���̍쐬
	MakeVertexThander();

	// �e�N�X�`������
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

	// �p�[�e�B�N�����[�N�̏�����
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
// �I������
//=============================================================================
void UninitThander(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateThander(void)
{

	for (int nCntThander = 0; nCntThander < MAX_THANDER; nCntThander++)
	{
		if (g_aThander[nCntThander].bUse)
		{	// �g�p��
			//g_aThander[nCntThander].pos.x += g_aThander[nCntThander].move.x;
			//g_aThander[nCntThander].pos.z += g_aThander[nCntThander].move.z;
			//g_aThander[nCntThander].pos.y += g_aThander[nCntThander].move.y;

			//g_aThander[nCntThander].move.x += (0.0f - g_aThander[nCntThander].move.x) * 0.015f;
			//g_aThander[nCntThander].move.y -= (0.0f - g_aThander[nCntThander].move.y) * 0.015f;
			//g_aThander[nCntThander].move.z += 0.25f;

#ifdef DISP_SHADOW
			if (g_aThander[nCntThander].nIdxShadow != -1)
			{// �e�g�p��
				float colA;

				// �e�̈ʒu�ݒ�
				SetPositionShadow(g_aThander[nCntThander].nIdxShadow, XMFLOAT3(g_aThander[nCntThander].pos.x, 0.1f, g_aThander[nCntThander].pos.z));

				// �e�̐F�̐ݒ�
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
					// ���l�ݒ�
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
// �`�揈��
//=============================================================================
void DrawThander(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	// ���C�e�B���O�𖳌���
	SetLightEnable(false);

	// ���Z�����ɐݒ�
	SetBlendState(BLEND_MODE_ADD);

	// Z��r����
	SetDepthEnable(false);

	// �t�H�O����
	SetFogEnable(false);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for (int nCntThander = 0; nCntThander < MAX_THANDER; nCntThander++)
	{
		if (g_aThander[nCntThander].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			//mtxWorld = XMMatrixInverse(nullptr, mtxView);
			//mtxWorld.r[3].m128_f32[0] = 0.0f;
			//mtxWorld.r[3].m128_f32[1] = 0.0f;
			//mtxWorld.r[3].m128_f32[2] = 0.0f;

			// ������������������
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_aThander[nCntThander].scale.x, g_aThander[nCntThander].scale.y, g_aThander[nCntThander].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_aThander[nCntThander].pos.x, g_aThander[nCntThander].pos.y, g_aThander[nCntThander].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_aThander[nCntThander].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(true);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(true);

	// �t�H�O�L��
	SetFogEnable(true);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexThander(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = XMFLOAT3(-THANDER_SIZE_X / 2, THANDER_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(THANDER_SIZE_X / 2, THANDER_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-THANDER_SIZE_X / 2, -THANDER_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(THANDER_SIZE_X / 2, -THANDER_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorThander(int nIdxThander, XMFLOAT4 col)
{
	g_aThander[nIdxThander].material.Diffuse = col;
}

//=============================================================================
// �p�[�e�B�N���̔�������
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
			// �e�̐ݒ�
			g_aThander[nCntThander].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
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
