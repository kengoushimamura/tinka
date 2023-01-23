//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
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
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit2(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Battle = TYPE_FULL_SCREEN;

static bool	g_bPause = true;	// �|�[�YON/OFF


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBattle(void)
{
	g_ViewPortType_Battle = TYPE_FULL_SCREEN;

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitBattlePlayer();

	// �G�l�~�[�̏�����
	InitBattleEnemy();

	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// �e�̏�����
	InitButtleBullet();

	// �X�R�A�̏�����
	//InitScore();

	InitShawar();

	InitCommand();


	// �p�[�e�B�N���̏�����
	InitParticle();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_sample001);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattle(void)
{
	// �p�[�e�B�N���̏I������
	UninitParticle();

	UninitCommand();

	UninitShawar();

	// �X�R�A�̏I������
	//UninitScore();

	// �e�̏I������
	UninitButtleBullet();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	// �G�l�~�[�̏I������
	UninitBattleEnemy();

	// �v���C���[�̏I������
	UninitBattlePlayer();

	// �e�̏I������
	UninitShadow();

}

//=============================================================================
// �X�V����
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

	// �n�ʏ����̍X�V
	UpdateMeshField();

	UpdateCommand();

	// �v���C���[�̍X�V����
	UpdateBattlePlayer();

	// �G�l�~�[�̍X�V����
	UpdateBattleEnemy();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	// �e�̍X�V����
	UpdateButtleBullet();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle();

	UpdateShawar();

	// �e�̍X�V����
	UpdateShadow();

	// �����蔻�菈��
	CheckHit2();

	// �X�R�A�̍X�V����
	UpdateScore();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattle0(void)
{
	// 3D�̕���`�悷�鏈��
	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// �G�l�~�[�̕`�揈��
	DrawBattleEnemy();

	// �v���C���[�̕`�揈��
	DrawBattlePlayer();

	// �e�̕`�揈��
	DrawButtleBullet();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	DrawShawar();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(false);

	// ���C�e�B���O�𖳌�
	SetLightEnable(false);

	// �X�R�A�̕`�揈��
	DrawScore();

	DrawCommand();

	// ���C�e�B���O��L����
	SetLightEnable(true);

	// Z��r����
	SetDepthEnable(true);
}


void DrawBattle(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Battle);

#endif

	// �v���C���[���_
	pos = GetbPlayer()->pos;
	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
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

		// �G�l�~�[���_
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

		// �G�l�~�[���_
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
// �����蔻�菈��
//=============================================================================
void CheckHit2(void)
{
	bENEMY* benemy = GetBattleEnemy();		// �o�g���p�G�l�~�[�̃|�C���^�[��������
	ENEMY* enemy = GetEnemy();				//�G�l�~�[�̃|�C���^�[��������
	bPLAYER* bplayer = GetbPlayer();		// �v���C���[�̃|�C���^�[��������
	BUTTLEBULLET* bbullet = GetButtleBullet();	// �e�̃|�C���^�[��������
	SHAWAR* shawar = GetShawar();				//���̃|�C���^�[��������


	// �G�ƃv���C���[�L����
	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (benemy[i].use == false)
			continue;

		//BC�̓����蔻��
		if (CollisionBC(bplayer->pos, benemy[i].pos, bplayer->size, benemy[i].size))
		{
			// �G�L�����N�^�[�͓|�����
			benemy[i].use = false;
			ReleaseShadow(benemy[i].shadowIdx);

			// �X�R�A�𑫂�
			AddScore(100);
		}
	}


	//�e�ƓG
	for (int i = 0; i < MAX_BBULLET; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (bbullet[i].use == false)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_B_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (benemy[j].use == false)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bbullet[i].pos, benemy[j].pos, bbullet[i].size, benemy[j].size))
			{
				// ������������e�͖��g�p�ɖ߂�
				bbullet[i].use = false;
				ReleaseShadow(bbullet[i].shadowIdx);

				// �G�L�����N�^�[�͓|�����
				benemy[j].use = false;
				ReleaseShadow(enemy[j].shadowIdx);

				// �X�R�A�𑫂�
				AddScore(10);
			}
		}

	}

	//���ƓG
	for (int i = 0; i < MAX_SHAWAR; i++)
	{
		//���̗L���t���O���`�F�b�N����
		if (shawar[i].bUse == false)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_B_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (benemy[j].use == false)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(shawar[i].pos, benemy[j].pos, SHAWAR_SIZE, benemy[j].size))
			{
				// �����������琅�͖��g�p�ɖ߂�
				shawar[i].bUse = false;
				ReleaseShadow(shawar[i].nIdxShadow);

				// �G�L�����N�^�[�͓|�����
				benemy[j].use = false;
				ReleaseShadow(enemy[j].shadowIdx);

				// �X�R�A�𑫂�
				AddScore(10);
			}
		}

	}



	// �o�g���p�G�l�~�[���S�����S�������ԑJ��
	int benemy_count = 0;
	for (int i = 0; i < MAX_B_ENEMY; i++)
	{
		if (benemy[i].use == false) continue;
		benemy_count++;
	}

	// �o�g���p�G�l�~�[���O�C
	if (benemy_count == 0)
	{
		// �Q�[�����[�h�G�l�~�[���S�����S�������ԑJ��
		int enemy_count = 0;
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].use == false) continue;
			enemy_count++;
		}

		// �Q�[�����[�h�G�l�~�[���O�C�H
		if (enemy_count == 0)
		{//���U���g��ʂ�
			SetFade(FADE_OUT, MODE_RESULT);
		}
		else//�܂�����
		{//�Q�[�����[�h�֖߂�
			SetFade(FADE_OUT, MODE_GAME);
		}
	}



}


