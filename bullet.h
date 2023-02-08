//=============================================================================
//
// �e���ˏ��� [bullet.h]
// Author : ��������
//
//=============================================================================
#pragma once
#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BULLET		(256)	// �e�ő吔
#define BULLET_SIZE		(5.0f)		//�����蔻��T�C�Y

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	rot;			// �p�x
	XMFLOAT3	scl;			// �X�P�[��
	MATERIAL	material;		// �}�e���A��
	float		spd;			// �ړ���
	float		fWidth;			// ��
	float		fHeight;		// ����
	int			shadowIdx;		// �eID
	bool		use;			// �g�p���Ă��邩�ǂ���

	//XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	//XMFLOAT3			pos;				// ���f���̈ʒu
	//XMFLOAT3			rot;				// ���f���̌���(��])
	//XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	//bool				use;
	//bool				load;
	//DX11_MODEL			model;				// ���f�����
	//XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	//float				spd;				// �ړ��X�s�[�h
	//float				size;				// �����蔻��̑傫��
	//int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�



} BULLET;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BULLET *GetBullet(void);

