//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShawar(void);
void UninitShawar(void);
void UpdateShawar(void);
void DrawShawar(void);

int SetShawar(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorShawar(int nIdxParticle, XMFLOAT4 col);
