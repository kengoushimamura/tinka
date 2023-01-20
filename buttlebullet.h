//=============================================================================
//
// �o���b�g���f������ [buttlebullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BBULLET		(20)					// �e��

#define	BBULLET_SIZE		(5.0f)				// �����蔻��̑傫��



//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct BUTTLEBULLET
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	bool				use;
	bool				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float				spd;				// �ړ��X�s�[�h
	float				size;				// �����蔻��̑傫��
	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�
	int					t;					//�o�ߎ���

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitButtleBullet(void);
void UninitButtleBullet(void);
void UpdateButtleBullet(void);
void DrawButtleBullet(void);

int SetButtleBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BUTTLEBULLET* GetButtleBullet(void);

