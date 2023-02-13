//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : ��������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "meshfield.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/book.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define ENEMY_OFFSET_Y		(0.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// �G�l�~�[

static BOOL				g_Load = FALSE;




//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = true;

		g_Enemy[i].pos = XMFLOAT3( 0.0f, ENEMY_OFFSET_Y, 20.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(0.5f, 0.5f, 0.5f);

		g_Enemy[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Enemy[0].model, &g_Enemy[0].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].move_time = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tbl_adr = NULL;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Enemy[i].tbl_size = 0;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[i].use = true;			// true:�����Ă�

	}

	g_Enemy[0].pos = XMFLOAT3(100.0f, ENEMY_OFFSET_Y, 0.0f);
	g_Enemy[1].pos = XMFLOAT3(-150.0f, ENEMY_OFFSET_Y, 0.0f);
	g_Enemy[2].pos = XMFLOAT3(50.0f, ENEMY_OFFSET_Y, 100.0f);
	g_Enemy[3].pos = XMFLOAT3(90.0f, ENEMY_OFFSET_Y, -150.0f);
	g_Enemy[4].pos = XMFLOAT3(0.0f, ENEMY_OFFSET_Y, 0.0f);


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = false;
		}
	}

	g_Load = FALSE;

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == true)			// ���̃G�l�~�[���g���Ă���H
		{									// Yes
			// �e���G�l�~�[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);

			// ���C�L���X�g���đ����̍��������߂�
			XMFLOAT3 HitPosition;		// ��_
			XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
			bool ans = RayHitField(g_Enemy[i].pos, &HitPosition, &Normal);
			if (ans)
			{
				g_Enemy[i].pos.y = HitPosition.y + ENEMY_OFFSET_Y;
			}
			else
			{
				g_Enemy[i].pos.y = ENEMY_OFFSET_Y;
				Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			}

			//////////////////////////////////////////////////////////////////////
			// �p������
			//////////////////////////////////////////////////////////////////////

			XMVECTOR vx, nvx, up;
			XMVECTOR quat;
			float len, angle;


			g_Enemy[i].UpVector = Normal;
			up = { 0.0f, 1.0f, 0.0f, 0.0f };
			vx = XMVector3Cross(up, XMLoadFloat3(&g_Enemy[i].UpVector));

			nvx = XMVector3Length(vx);
			XMStoreFloat(&len, nvx);
			nvx = XMVector3Normalize(vx);
			//nvx = vx / len;
			angle = asinf(len);

			//quat = XMQuaternionIdentity();

		//	quat = XMQuaternionRotationAxis(nvx, angle);
			quat = XMQuaternionRotationNormal(nvx, angle);


			quat = XMQuaternionSlerp(XMLoadFloat4(&g_Enemy[i].Quaternion), quat, 0.05f);
			XMStoreFloat4(&g_Enemy[i].Quaternion, quat);

		}
	}



}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	SetFuchi(2);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Enemy[i].model);
	}
	SetFuchi(0);


	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
