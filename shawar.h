//=============================================================================
//
// �p�[�e�B�N������ [shawar.h]
// Author : 
//
//=============================================================================
#pragma once
#define	MAX_SHAWAR			(64)		// �p�[�e�B�N���ő吔
#define	SHAWAR_SIZE			(15.0F)		//�����蔻��

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

} SHAWAR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShawar(void);
void UninitShawar(void);
void UpdateShawar(void);
void DrawShawar(void);

int SetShawar(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorShawar(int nIdxParticle, XMFLOAT4 col);
SHAWAR* GetShawar(void);
