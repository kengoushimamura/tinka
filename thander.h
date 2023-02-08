//=============================================================================
//
// ���p�|���S������ [thander.h]
// Author : ��������
//
//=============================================================================
#pragma once
#define	MAX_THANDER			(64)		// �p�[�e�B�N���ő吔
#define	THANDER_SIZE			(15.0F)		//�����蔻��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nIdxShadow;		// �eID
	int				nLife;			// ����
	bool			bUse;			// �g�p���Ă��邩�ǂ���

} THANDER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitThander(void);
void UninitThander(void);
void UpdateThander(void);
void DrawThander(void);

int SetThander(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorThander(int nIdxParticle, XMFLOAT4 col);
THANDER* GetThander(void);
