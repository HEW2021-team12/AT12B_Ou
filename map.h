/*==============================================================================

   �}�b�v�Ǘ�[map.h]
														 Author :	�^ ���� �Îs
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MAP_H_
#define MAP_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//���͐�΂ɂ�����Ȃ����ƁI
#define VIEW_SCREEN_WIDTH	(16.0f)		// �\���͈�(��)
#define VIEW_SCREEN_HEIGHT	(9.0f)		// �\���͈�(�c)

#define CHIP_SIZE			(60.0f)		// �}�X�̑傫��

// ���Ń}�b�v
#define MAP_X				(27)		// �}�b�v�̉��̃}�X��
#define MAP_Y				(18)		// �}�b�v�̏c�̃}�X��

// ����ȍ~�̃}�b�v


//�}�b�v�\����
typedef struct
{
	float		u = 0.0f;
	float		v = 0.0f;
	float		uh = 0.0f;
	float		vh = 0.0f;
}MAP;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMap	(void);
void UninitMap	(void);
void UpdateMap	(void);
void DrawMap	(void);
void ChangeUv   (void);

int GetMapEnter(D3DXVECTOR2 pos, D3DXVECTOR2 size);
int GetMap(int x, int y);
#endif //MAP_H_
