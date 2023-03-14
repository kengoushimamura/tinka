//=============================================================================
//
// �V�����[�p�p�[�e�B�N���G�t�F�N�g���� [shawar.cpp]
// Author : ��������
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�

#define	SHAWAR_SIZE_X		(40.0f)		// ���_�T�C�Y
#define	SHAWAR_SIZE_Y		(40.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_SHAWAR	(5.0f)		// �ړ����x


#define	DISP_SHADOW						// �e�̕\��
//#undef DISP_SHADOW


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexShawar(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_�o�b�t�@

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;						// �e�N�X�`���ԍ�

static SHAWAR					g_aShawar[MAX_SHAWAR];				// �p�[�e�B�N�����[�N
static XMFLOAT3					g_posBase;							// �r���{�[�h�����ʒu
static float					g_fWidthBase = 5.0f;				// ��̕�
static float					g_fHeightBase = 10.0f;				// ��̍���
static float					g_roty = 0.0f;						// �ړ�����
static float					g_spd = 0.0f;						// �ړ��X�s�[�h

static char* g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitShawar(void)
{
	// ���_���̍쐬
	MakeVertexShawar();

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
// �I������
//=============================================================================
void UninitShawar(void)
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
void UpdateShawar(void)
{

	for (int nCntShawar = 0; nCntShawar < MAX_SHAWAR; nCntShawar++)
	{
		if (g_aShawar[nCntShawar].bUse)
		{	// �g�p��
			g_aShawar[nCntShawar].pos.x -= g_aShawar[nCntShawar].move.x * sinf(g_aShawar[nCntShawar].rot.y);
			g_aShawar[nCntShawar].pos.z -= g_aShawar[nCntShawar].move.z * cosf(g_aShawar[nCntShawar].rot.y);
			g_aShawar[nCntShawar].pos.y -= g_aShawar[nCntShawar].move.y;

			g_aShawar[nCntShawar].move.x += (0.0f - g_aShawar[nCntShawar].move.x) * 0.015f;
			g_aShawar[nCntShawar].move.y += (0.0f - g_aShawar[nCntShawar].move.y) * 0.015f;
			g_aShawar[nCntShawar].move.z += (0.0f - g_aShawar[nCntShawar].move.z) * 0.015f;

#ifdef DISP_SHADOW
			if (g_aShawar[nCntShawar].nIdxShadow != -1)
			{// �e�g�p��
				float colA;

				// �e�̈ʒu�ݒ�
				SetPositionShadow(g_aShawar[nCntShawar].nIdxShadow, XMFLOAT3(g_aShawar[nCntShawar].pos.x, 0.1f, g_aShawar[nCntShawar].pos.z));

				// �e�̐F�̐ݒ�
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
					// ���l�ݒ�
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
// �`�揈��
//=============================================================================
void DrawShawar(void)
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

	for (int nCntShawar = 0; nCntShawar < MAX_SHAWAR; nCntShawar++)
	{
		if (g_aShawar[nCntShawar].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// �r���{�[�h����
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
			mtxScl = XMMatrixScaling(g_aShawar[nCntShawar].scale.x, g_aShawar[nCntShawar].scale.y, g_aShawar[nCntShawar].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_aShawar[nCntShawar].pos.x, g_aShawar[nCntShawar].pos.y, g_aShawar[nCntShawar].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_aShawar[nCntShawar].material);

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
HRESULT MakeVertexShawar(void)
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
		vertex[0].Position = XMFLOAT3(-SHAWAR_SIZE_X / 2, SHAWAR_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(SHAWAR_SIZE_X / 2, SHAWAR_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-SHAWAR_SIZE_X / 2, -SHAWAR_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(SHAWAR_SIZE_X / 2, -SHAWAR_SIZE_Y / 2, 0.0f);

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
void SetColorShawar(int nIdxShawar, XMFLOAT4 col)
{
	g_aShawar[nIdxShawar].material.Diffuse = col;
}

//=============================================================================
// �p�[�e�B�N���̔�������
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
			// �e�̐ݒ�
			g_aShawar[nCntShawar].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
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
