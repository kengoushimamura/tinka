//=============================================================================
//
// �o�g���p�G�l�~�[���f������ [buttleenemy.cpp]
// Author : ��������
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
// �}�N����`
//*****************************************************************************
#define	MODEL_B_ENEMY			"data/MODEL/makihito.obj"		// �ǂݍ��ރ��f����


#define	VALUE_MOVE			(5.0f)								// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)						// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)								// �e�̑傫��
#define ENEMY_OFFSET_Y		(3.0f)								// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static bENEMY			g_bEnemy[MAX_B_ENEMY];				// �G�l�~�[

static BOOL				g_bLoad = FALSE;




//=============================================================================
// ����������
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

		g_bEnemy[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_bEnemy[i].size = bENEMY_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_bEnemy[0].model, &g_bEnemy[0].diffuse[0]);

		XMFLOAT3 pos = g_bEnemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_bEnemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_bEnemy[i].move_time = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		g_bEnemy[i].tbl_adr = NULL;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_bEnemy[i].tbl_size = 0;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_bEnemy[i].use = true;			// true:�����Ă�

	}



	g_bLoad = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateBattleEnemy(void)
{
	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		if (g_bEnemy[i].use == true)			// ���̃G�l�~�[���g���Ă���H
		{										// Yes
			if (g_bEnemy[i].tbl_adr != NULL)	// ���`��Ԃ����s����H
			{									// ���`��Ԃ̏���
				// �ړ�����
				int		index = (int)g_bEnemy[i].move_time;
				float	time = g_bEnemy[i].move_time - index;
				int		size = g_bEnemy[i].tbl_size;

				float dt = 1.0f / g_bEnemy[i].tbl_adr[index].frame;		// 1�t���[���Ői�߂鎞��
				g_bEnemy[i].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

				if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
				{
					g_bEnemy[i].move_time = 0.0f;
					index = 0;
				}

				// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
				XMVECTOR p1 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
				XMVECTOR p0 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_bEnemy[i].pos, p0 + vec * time);

				// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
				XMVECTOR r1 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 1].rot);	// ���̊p�x
				XMVECTOR r0 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_bEnemy[i].rot, r0 + rot * time);

				// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
				XMVECTOR s1 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 1].scl);	// ����Scale
				XMVECTOR s0 = XMLoadFloat3(&g_bEnemy[i].tbl_adr[index + 0].scl);	// ���݂�Scale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_bEnemy[i].scl, s0 + scl * time);

			}


			// ���C�L���X�g���đ����̍��������߂�
			XMFLOAT3 HitPosition;		// ��_
			XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
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


			// �e���G�l�~�[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_bEnemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_bEnemy[i].shadowIdx, pos);
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattleEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		if (g_bEnemy[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_bEnemy[i].scl.x, g_bEnemy[i].scl.y, g_bEnemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_bEnemy[i].rot.x, g_bEnemy[i].rot.y + XM_PI, g_bEnemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_bEnemy[i].pos.x, g_bEnemy[i].pos.y, g_bEnemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_bEnemy[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_bEnemy[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
bENEMY* GetBattleEnemy()
{
	return &g_bEnemy[0];
}
