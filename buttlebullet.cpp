//=============================================================================
//
// �o���b�g���f������ [buttlebullet.cpp]
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
// �}�N����`
//*****************************************************************************
#define	MODEL_BULLET			"data/MODEL/aquabullet.obj"	// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��
#define	BUTTLEBULLET_SPEED		(5.0f)						// �e�̈ړ��X�s�[�h


#define BULLET_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define BULLET_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BUTTLEBULLET			g_bBullet[MAX_BBULLET];				// �e

static BOOL				g_Load = FALSE;




//=============================================================================
// ����������
//=============================================================================
HRESULT InitButtleBullet(void)
{
	for (int i = 0; i < MAX_BBULLET; i++)
	{

		g_bBullet[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_bBullet[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_bBullet[i].scl = XMFLOAT3(0.0f, 0.0f, 0.0f);

		g_bBullet[i].spd = BUTTLEBULLET_SPEED;			// �ړ��X�s�[�h�N���A
		g_bBullet[i].size = BBULLET_SIZE;	// �����蔻��̑傫��

		g_bBullet[i].t = 0;

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_bBullet[0].model, &g_bBullet[0].diffuse[0]);

		g_bBullet[i].use = false;			// ���g�p

	}



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
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
				//temp = XMVector3LengthSq(temp);			// 2�_�Ԃ̋����i2�悵�����j
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

				// �e�̈ړ�����
				g_bBullet[i].pos.x -= sinf(g_bBullet[i].rot.y) * g_bBullet[i].spd;
				g_bBullet[i].pos.z -= cosf(g_bBullet[i].rot.y) * g_bBullet[i].spd;

				// �e�̈ʒu�ݒ�
				SetPositionShadow(g_bBullet[i].shadowIdx, XMFLOAT3(g_bBullet[i].pos.x, 0.1f, g_bBullet[i].pos.z));


				// �t�B�[���h�̊O�ɏo����e����������
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
// �`�揈��
//=============================================================================
void DrawButtleBullet(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BBULLET; i++)
	{
		if (g_bBullet[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_bBullet[i].scl.x, g_bBullet[i].scl.y, g_bBullet[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_bBullet[i].rot.x, g_bBullet[i].rot.y + XM_PI, g_bBullet[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_bBullet[i].pos.x, g_bBullet[i].pos.y, g_bBullet[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_bBullet[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_bBullet[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �e�̃p�����[�^���Z�b�g
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

			// �e�̐ݒ�
			g_bBullet[nCntBullet].shadowIdx = CreateShadow(g_bBullet[nCntBullet].pos, 0.5f, 0.5f);

			nIdxBullet = nCntBullet;

			PlaySound(SOUND_LABEL_SE_shot000);

			break;
		}
	}

	return nIdxBullet;
}


//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
BUTTLEBULLET* GetButtleBullet()
{
	return &g_bBullet[0];
}
