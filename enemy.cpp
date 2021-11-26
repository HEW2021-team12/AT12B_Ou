//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "player.h"
#include "map.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ROT_SPD				(0.05f)
#define ENEMY_SPD			(2.0f)
#define ENEMY_SIZE			(55.0f)
#define MOVE_TIMER			(50)
#define SERCH_RANGE			(250.0f)
#define FIND_SPD			(2.0f)

#define SEARCH_TIMER		(CHIP_SIZE / ENEMY_SPD)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_EnemyTexture = 0;
static ENEMY g_Enemy;							  // �G�l�~�[�\����
static float MAPleft, MAPright, MAPtop, MAPunder; // �G�l�~�[�̍s���͈�
static int g_Enemy_Timer;
static int g_Search_Timer;
int move;
D3DXVECTOR2 g_NotMove;
D3DXVECTOR2 g_Move;
bool g_MoveCntX, g_MoveCntY;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	g_EnemyTexture = LoadTexture("data/TEXTURE/enemy.png");

	MAPleft = SCREEN_WIDTH / 2;
	MAPright = SCREEN_WIDTH / 2;

	MAPtop = SCREEN_HEIGHT / 2;
	MAPunder = SCREEN_HEIGHT / 2;

	// �G�l�~�[�\���̂̏�����
	g_Enemy.use   = true;
	g_Enemy.w     = ENEMY_SIZE;
	g_Enemy.h     = ENEMY_SIZE;
	g_Enemy.pos   = D3DXVECTOR2(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 - 150.0f);
	g_Enemy.vel   = D3DXVECTOR2(ENEMY_SPD,ENEMY_SPD);

	g_Enemy_Timer = 0;
	g_Search_Timer = 0;

	// �G�l�~�[UV
	g_Enemy.u = 0.0f;
	g_Enemy.v = 0.0f;
	g_Enemy.uh = 0.0f;
	g_Enemy.vh = 0.0f;
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{

	if (g_Enemy.use == true)
	{
		// �ǐՔ���
		if (SerchPlayer(GetPlayerPosition(), g_Enemy.pos))
		{// ������(�͈͓��ɓ�����)

			// ���ŃT�[�`
			{
			//���݈ʒu
			D3DXVECTOR2 position = g_Enemy.pos;

			//�ڕW�n�_
			D3DXVECTOR2 destination = GetPlayerPosition();

			//����
			D3DXVECTOR2 components;

			//�傫��
			float magnitude;

			//�����i�P�ʃx�N�g���j
			D3DXVECTOR2 direction;

			//�����v�Z
			components.x = destination.x - position.x;
			components.y = destination.y - position.y;

			//�傫���v�Z
			magnitude = (float)sqrt(components.x * components.x + components.y * components.y);

			//�����v�Z�i���K���j
			direction.x = components.x / magnitude;
			direction.y = components.y / magnitude;

			// ���̈ړ��ʂ̕����傫���Ƃ�
			if (fabsf(direction.x) > fabsf(direction.y))
			{
				// ���ɍs���Ƃ�
				if (direction.x < 0)
				{
					g_Enemy.u = 1.0f;
					g_Enemy.v = 0.0f;
					g_Enemy.uh = -0.5f;
					g_Enemy.vh = 0.5f;
				}
				else // �E�ɍs���Ƃ�
				{
					g_Enemy.u = 0.5f;
					g_Enemy.v = 0.0f;
					g_Enemy.uh = 0.5f;
					g_Enemy.vh = 0.5f;
				}
			}
			else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
			{
				// ��ɍs���Ƃ�
				if (direction.y < 0)
				{
					g_Enemy.u = 0.5f;
					g_Enemy.v = 0.5f;
					g_Enemy.uh = 0.5f;
					g_Enemy.vh = 0.5f;
				}
				else // ���ɍs���Ƃ�
				{
					g_Enemy.u = 0.0f;
					g_Enemy.v = 0.5f;
					g_Enemy.uh = 0.5f;
					g_Enemy.vh = 0.5f;
				}
			}

			//�ړ���̈ʒu���v�Z
			g_Enemy.pos.x += direction.x * FIND_SPD;
			if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x + direction.x * FIND_SPD, g_Enemy.pos.y),
				D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
				== 1)
			{
				g_Enemy.pos.x -= direction.x * FIND_SPD;
				g_NotMove.x += fabsf(direction.x);

				if (!g_MoveCntX)
				{
					// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
					if (g_NotMove.x > CHIP_SIZE)
					{
						g_MoveCntX = true;
						g_NotMove.x = 0.0f;
					}
				}
			}

			g_Enemy.pos.y += direction.y * FIND_SPD;
			if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x, g_Enemy.pos.y + direction.y * FIND_SPD),
				D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
				== 1)
			{
				g_Enemy.pos.y -= direction.y * FIND_SPD;
				g_NotMove.y += fabsf(direction.y);

				if (!g_MoveCntY)
				{
					// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
					if (g_NotMove.y > CHIP_SIZE)
					{
						g_MoveCntY = true;
						g_NotMove.y = 0.0f;
					}
				}

			}

			// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
			if (g_MoveCntX)
			{
				g_Move.y += ENEMY_SPD;

				g_Enemy.pos.y += ENEMY_SPD;
				if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x, g_Enemy.pos.y + ENEMY_SPD),
					D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
					== 1)
				{
					g_Enemy.pos.y -= ENEMY_SPD;
				}

				// STOP_CNT���ړ�������
				if (g_Move.y > CHIP_SIZE * 3 / 2)
				{
					g_MoveCntX = false;
					g_Move.y = 0.0f;
				}
			}
			if (g_MoveCntY)
			{
				g_Move.x += ENEMY_SPD;

				g_Enemy.pos.x += ENEMY_SPD;
				if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x + ENEMY_SPD, g_Enemy.pos.y),
					D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
					== 1)
				{
					g_Enemy.pos.x -= ENEMY_SPD;
				}

				// STOP_CNT���ړ�������
				if (g_Move.x > CHIP_SIZE * 3 / 2)
				{
					g_MoveCntY = false;
					g_Move.x = 0.0f;
				}
			}

			}
			
		}
		// �����Ă��Ȃ�
		else 
		{
			g_Enemy_Timer++;

			if (g_Enemy_Timer > MOVE_TIMER)
			{
				move = rand() % 5;
				g_Enemy_Timer = 0;
				srand((unsigned)time(NULL));
			}

			// �G�l�~�[�ړ�
			// 1:��@2:���@3:�E�@4:���@0:�Ȃ�(�ҋ@)
			if (move == 1) // ��
			{
				g_Enemy.pos.y -= g_Enemy.vel.y;
				if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x, g_Enemy.pos.y - g_Enemy.vel.y),
					D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
					== 1)
				{
					g_Enemy.pos.y += g_Enemy.vel.y;
				}
				g_Enemy.u = 0.5f;
				g_Enemy.v = 0.5f;
				g_Enemy.uh = 0.5f;
				g_Enemy.vh = 0.5f;
			}

			if (move == 2) // ��
			{
				g_Enemy.pos.y += g_Enemy.vel.y;
				if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x, g_Enemy.pos.y + g_Enemy.vel.y),
					D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
					== 1)
				{
					g_Enemy.pos.y -= g_Enemy.vel.y;
				}
				g_Enemy.u = 0.0f;
				g_Enemy.v = 0.5f;
				g_Enemy.uh = 0.5f;
				g_Enemy.vh = 0.5f;
			}

			if (move == 3) // �E
			{
				g_Enemy.pos.x += g_Enemy.vel.x;
				if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x + g_Enemy.vel.x, g_Enemy.pos.y),
					D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
					== 1)
				{
					g_Enemy.pos.x -= g_Enemy.vel.x;
				}
				g_Enemy.u = 0.5f;
				g_Enemy.v = 0.0f;
				g_Enemy.uh = 0.5f;
				g_Enemy.vh = 0.5f;
			}

			if (move == 4) // ��
			{
				g_Enemy.pos.x -= g_Enemy.vel.x;
				if (GetMapEnter(D3DXVECTOR2(g_Enemy.pos.x - g_Enemy.vel.x, g_Enemy.pos.y),
					D3DXVECTOR2(g_Enemy.w, g_Enemy.h))
					== 1)
				{
					g_Enemy.pos.x += g_Enemy.vel.x;
				}
				g_Enemy.u = 1.0f;
				g_Enemy.v = 0.0f;
				g_Enemy.uh = -0.5f;
				g_Enemy.vh = 0.5f;
			}

			if (move == 0) // �ҋ@
			{
				g_Enemy.u = 0.0f;
				g_Enemy.v = 0.0f;
				g_Enemy.uh = 0.5f;
				g_Enemy.vh = 0.5f;
			}

		}
		
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	if (g_Enemy.use == true)
	{
		PLAYER* player = GetPlayer();

		// ��ʓ��ɓ����Ă��邩
		if (fabsf(g_Enemy.pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
			fabsf(g_Enemy.pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
		{
			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy.pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
			float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy.pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
			float pw = g_Enemy.w;		// �G�l�~�[�̕\����
			float ph = g_Enemy.h;		// �G�l�~�[�̕\������
			D3DXCOLOR col;

			if (SerchPlayer(GetPlayerPosition(), g_Enemy.pos))
			{
				col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			DrawSpriteColor(g_EnemyTexture, px, py, pw, ph,
				g_Enemy.u, g_Enemy.v,
				g_Enemy.uh, g_Enemy.vh,
				col);
		}

		
	}
}

//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_Enemy;
}


//=============================================================================
// �G�̔�������
//=============================================================================
void SetEnemy(D3DXVECTOR2 pos)
{
	// �������g�p�̃f�[�^��T��
	if (g_Enemy.use == false)		// ���g�p��Ԃ̓G�f�[�^��������
	{
		g_Enemy.use = true;			// �g�p��Ԃ֕ύX����
		g_Enemy.pos = pos;			// ���W���Z�b�g
		return;						// �G���Z�b�g�ł����̂ŏI������
	}
	
}

bool SerchPlayer(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
{
	D3DXVECTOR2 maxRange, minRange;

	minRange.x = Enemypos.x - SERCH_RANGE;
	minRange.y = Enemypos.y - SERCH_RANGE;
	maxRange.x = Enemypos.x + SERCH_RANGE;
	maxRange.y = Enemypos.y + SERCH_RANGE;

	//X���̔���
	if (minRange.x < Playerpos.x &&
		maxRange.x > Playerpos.x)
	{
		//Y���̔���
		if (minRange.y < Playerpos.y &&
			maxRange.y > Playerpos.y)
		{
			//�S�Ă̏�����TRUE�Ȃ�q�b�g����
			return true;
		}
	}

	return false;
}