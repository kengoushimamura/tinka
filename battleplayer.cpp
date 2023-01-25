//=============================================================================
//
// ���f������ [player.cpp]
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
// �}�N����`
//*****************************************************************************
#define	MODEL_bPLAYER		"data/MODEL/body2.obj"			// �ǂݍ��ރ��f����
#define	MODEL_bPLAYER_r_arm	"data/MODEL/r_arm2.obj"			// �ǂݍ��ރ��f����
#define	MODEL_bPLAYER_l_arm	"data/MODEL/l_arm2.obj"			// �ǂݍ��ރ��f����
#define	MODEL_bPLAYER_r_leg	"data/MODEL/leg2.obj"			// �ǂݍ��ރ��f����
#define	MODEL_bPLAYER_l_leg	"data/MODEL/leg2.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// ��]��

#define bPLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(14.0f)							// �v���C���[�̑��������킹��

#define bPLAYER_PARTS_MAX	(4)								// �v���C���[�̃p�[�c�̐�



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static bPLAYER		g_bPlayer;						// �v���C���[

static bPLAYER		g_Parts[bPLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

static float		roty = 0.0f;

static LIGHT		g_Light;





// �v���C���[�̊K�w�A�j���[�V�����f�[�^

//�E�r�̃A�j���[�V����
static INTERPOLATION_DATA move_tbl_right_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     XM_PI / 4),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     XM_PI / 2),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2, XM_PI * 3 / 4),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     XM_PI / 2),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },

};

//���r�̃A�j���[�V����
static INTERPOLATION_DATA move_tbl_left_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     -XM_PI / 4),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     -XM_PI / 2),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2, -XM_PI * 3 / 4),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(5.0f,  5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2,     -XM_PI / 2),			XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
};

//�E���̃A�j���[�V����
static INTERPOLATION_DATA move_tbl_right_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(XM_PI / 4, 0.0f,  0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(0.0f,      0.0f,  0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-2.5f,  -5.0f, 0.0f), XMFLOAT3(0.0f,      0.0f,  0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },

};

//�����̃A�j���[�V����
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

//�\���̂̒�����|�C���^�[����������
static bPLAYER* Parent[] =
{
	&g_bPlayer,
};





//=============================================================================
// ����������
//=============================================================================
HRESULT InitBattlePlayer(void)
{
	g_bPlayer.load = TRUE;
	LoadModel(MODEL_bPLAYER, &g_bPlayer.model);

	g_bPlayer.pos = XMFLOAT3(-10.0f, PLAYER_OFFSET_Y + 50.0f, -50.0f);
	g_bPlayer.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_bPlayer.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_bPlayer.spd = 0.0f;			// �ړ��X�s�[�h�N���A

	g_bPlayer.use = TRUE;			// true:�����Ă�
	g_bPlayer.size = bPLAYER_SIZE;	// �����蔻��̑傫��

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_bPlayer.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_bPlayer.shadowIdx = CreateShadow(pos, bPLAYER_SHADOW_SIZE, bPLAYER_SHADOW_SIZE);

	// �L�[�����������̃v���C���[�̌���
	roty = 0.0f;

	g_bPlayer.parent = -1;				// �{�́i�e�j�Ȃ̂ŕςȂ̂�����Ȃ��悤�Ɂ[�P


	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �e�q�֌W
		g_Parts[i].parent = -1;			// �� �����ɐe�̃A�h���X������@

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].move_time = 0.0f;	// ���s���Ԃ��N���A
		g_Parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g



		// �p�[�c�̓ǂݍ���
		g_Parts[i].load = FALSE;
	}


	//�E�r�̋N��
	g_Parts[0].pos = XMFLOAT3(-5.0f, 5.0f, 0.0f);
	g_Parts[0].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
	g_Parts[0].use = true;
	g_Parts[0].parent = 0;																	// �� �����ɐe�̃A�h���X������
	g_Parts[0].tblNo = 0;																	// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
	g_Parts[0].tblMax = sizeof(move_tbl_right_arm) / sizeof(INTERPOLATION_DATA);			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g
	g_Parts[0].load = true;
	LoadModel(MODEL_bPLAYER_r_arm, &g_Parts[0].model);

	//���r�̋N��
	g_Parts[1].pos = XMFLOAT3(5.0f, 5.0f, 0.0f);
	g_Parts[1].rot = XMFLOAT3(0.0f, XM_PI / 2, -XM_PI / 2);
	g_Parts[1].use = true;
	g_Parts[1].parent = 0;																// �� �����ɐe�̃A�h���X������
	g_Parts[1].tblNo = 1;																// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
	g_Parts[1].tblMax = sizeof(move_tbl_left_arm) / sizeof(INTERPOLATION_DATA);			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g
	g_Parts[1].load = true;
	LoadModel(MODEL_bPLAYER_l_arm, &g_Parts[1].model);

	//�E���̋N��
	g_Parts[2].pos = XMFLOAT3(-2.5f, -5.0f, 0.0f);
	g_Parts[2].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	g_Parts[2].use = true;
	g_Parts[2].parent = 0;																	// �� �����ɐe�̃A�h���X������
	g_Parts[2].tblNo = 2;																	// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
	g_Parts[2].tblMax = sizeof(move_tbl_right_leg) / sizeof(INTERPOLATION_DATA);			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g
	g_Parts[2].load = true;
	LoadModel(MODEL_bPLAYER_r_leg, &g_Parts[2].model);

	//�����̋N��
	g_Parts[3].pos = XMFLOAT3(2.5f, -5.0f, 0.0f);
	g_Parts[3].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	g_Parts[3].use = true;
	g_Parts[3].parent = 0;																// �� �����ɐe�̃A�h���X������
	g_Parts[3].tblNo = 3;																// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
	g_Parts[3].tblMax = sizeof(move_tbl_left_leg) / sizeof(INTERPOLATION_DATA);			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g
	g_Parts[3].load = true;
	LoadModel(MODEL_bPLAYER_l_leg, &g_Parts[3].model);


	// �N�H�[�^�j�I���̏�����
	XMStoreFloat4(&g_bPlayer.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattlePlayer(void)
{
	// ���f���̉������
	if (g_bPlayer.load == TRUE)
	{
		UnloadModel(&g_bPlayer.model);
		g_bPlayer.load = FALSE;
	}

	// �p�[�c�̉������
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// �p�[�c�̉������
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}

}

//=============================================================================
// �X�V����
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


	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 HitPosition;		// ��_
	XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
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


	// �e���ˏ���
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

				// �r���{�[�h�̐ݒ�
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

				// �r���{�[�h�̐ݒ�
				SetShawar(pos, move, XMFLOAT4(0.0f, 0.0f, 1.0f, 0.85f), fSize, fSize, nLife);

			}
		}
	}


	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_bPlayer.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_bPlayer.shadowIdx, pos);


	// �K�w�A�j���[�V����
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == TRUE)
		{
			//// �ړ�����
			int		index = (int)g_Parts[i].move_time;
			float	time = g_Parts[i].move_time - index;
			int		size = sizeof(move_tbl_right_arm) / sizeof(INTERPOLATION_DATA);

			float dt = 1.0f / g_MoveTblAdr[g_Parts[i].tblNo][index].frame;				// 1�t���[���Ői�߂鎞��
			g_Parts[i].move_time += dt;													// �A�j���[�V�����̍��v���Ԃɑ���

			if (index > (size - 2))														// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
			{
				g_Parts[i].move_time = 0.0f;
				index = 0;
			}

			// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
			XMVECTOR p1 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 1].pos);	// ���̏ꏊ
			XMVECTOR p0 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 0].pos);	// ���݂̏ꏊ
			XMVECTOR vec = p1 - p0;
			XMStoreFloat3(&g_Parts[i].pos, p0 + vec * time);

			// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
			XMVECTOR r1 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 1].rot);	// ���̊p�x
			XMVECTOR r0 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 0].rot);	// ���݂̊p�x
			XMVECTOR rot = r1 - r0;
			XMStoreFloat3(&g_Parts[i].rot, r0 + rot * time);

			// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
			XMVECTOR s1 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 1].scl);	// ����Scale
			XMVECTOR s0 = XMLoadFloat3(&g_MoveTblAdr[g_Parts[i].tblNo][index + 0].scl);	// ���݂�Scale
			XMVECTOR scl = s1 - s0;
			XMStoreFloat3(&g_Parts[i].scl, s0 + scl * time);

		}
	}

	if (g_bPlayer.spd == 0.0f)
	{	//�����ĂȂ��Ƃ���init�̃|�[�Y
		g_Parts[0].pos = XMFLOAT3(-5.0f, 5.0f, 0.0f);
		g_Parts[0].rot = XMFLOAT3(0.0f, XM_PI / 2, XM_PI / 2);
		g_Parts[1].pos = XMFLOAT3(5.0f, 5.0f, 0.0f);
		g_Parts[1].rot = XMFLOAT3(0.0f, XM_PI / 2, -XM_PI / 2);
		g_Parts[2].pos = XMFLOAT3(-2.5f, -5.0f, 0.0f);
		g_Parts[2].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
		g_Parts[3].pos = XMFLOAT3(2.5f, -5.0f, 0.0f);
		g_Parts[3].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	}



	// �|�C���g���C�g
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
	// �p������
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
	// �f�o�b�O�\��
	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_bPlayer.pos.x, g_bPlayer.pos.y, g_bPlayer.pos.z, Normal.y);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattlePlayer(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_bPlayer.scl.x, g_bPlayer.scl.y, g_bPlayer.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_bPlayer.rot.x, g_bPlayer.rot.y + XM_PI, g_bPlayer.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_bPlayer.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_bPlayer.pos.x, g_bPlayer.pos.y, g_bPlayer.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_bPlayer.mtxWorld, mtxWorld);


	// �����̐ݒ�
	SetFuchi(1);

	// ���f���`��
	DrawModel(&g_bPlayer.model);

	// �K�w�A�j���[�V����
	for (int i = 0; i < bPLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent == 0)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&Parent[g_Parts[i].parent]->mtxWorld));
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);


		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}

	SetFuchi(0);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
bPLAYER* GetbPlayer(void)
{
	return &g_bPlayer;
}
