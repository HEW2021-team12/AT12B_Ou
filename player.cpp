/*==============================================================================

   ���_�Ǘ� [player.cpp]
	Author :	�^�@���� �Îs
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "enemy.h"
#include "map.h"
#include "keyboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_PlayerTexture = 0;
PLAYER		g_Player;
float		control_timer;
float		gravity_timer;
float		vertigo_timer;

float		g_DrawGrav = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	g_PlayerTexture = LoadTexture("data/TEXTURE/player1.png");

	g_Player.pos.x = CHIP_SIZE * 11;
	g_Player.pos.y = CHIP_SIZE * 3;
	g_Player.size.x = PLAYER_SIZE;
	g_Player.size.y = PLAYER_SIZE;
	g_Player.rot = 0.0f;
	g_Player.rot_vel = 0.1f;
	control_timer = 0.0f;
	gravity_timer = 0.0f;
	g_Player.vertigo_isUse = false;
	vertigo_timer = 0.0f;
	g_Player.difference.x = 0.0f;
	g_Player.difference.y = 0.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	if (g_Player.rot <= -2.0f || g_Player.rot >= 3.0f)
	{
		g_Player.vertigo_isUse = true;
	}

	if (g_Player.vertigo_isUse == false)
	{
		//ZeroGravity();
		ControlPlayer();
		FramePlayer();
	}

	if (g_Player.vertigo_isUse == true)
	{
		vertigo_timer ++;
	}

	if (vertigo_timer >= 100.0f)
	{
		g_Player.vertigo_isUse = false;
		g_Player.rot = 0.0f;
		vertigo_timer = 0.0f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawSpriteColorRotate(g_PlayerTexture,
		SCREEN_WIDTH / 2 + g_Player.difference.x,
		SCREEN_HEIGHT / 2 + g_Player.difference.y + g_DrawGrav,
		g_Player.size.x, g_Player.size.y,
		0.0f, 0.0f,
		1.0f,1.0f,
		col, g_Player.rot);
}

void ControlPlayer(void)
{
	g_Player.pos.x += g_Player.vel.x;
	g_Player.pos.y += g_Player.vel.y;

	// X���̓����蔻��
	if (GetMapEnter(D3DXVECTOR2(g_Player.pos.x + g_Player.vel.x, g_Player.pos.y),
		D3DXVECTOR2(g_Player.size.x, g_Player.size.y))
		== 1)
	{
		g_Player.pos.x -= g_Player.vel.x;
	}

	// Y���̓����蔻��
	if (GetMapEnter(D3DXVECTOR2(g_Player.pos.x, g_Player.pos.y + g_Player.vel.y),
		D3DXVECTOR2(g_Player.size.x, g_Player.size.y))
		== 1)
	{
		g_Player.pos.y -= g_Player.vel.y;
	}

	//��(�ړ�)
	if (Keyboard_IsKeyDown(KK_LEFT)
		|| IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_LEFT))
	{

		g_Player.vel.x -= PLAYER_ACCELE;

		// X���x�̏���l
		if (g_Player.vel.x < -PLAYER_MAX_SPD)
		{
			g_Player.vel.x = -PLAYER_MAX_SPD;
		}

		control_timer++;
		if (control_timer >= 20.0f)
		{
			g_Player.rot -= g_Player.rot_vel;
			control_timer = 0.0f;
		}
	}

	//�E(�ړ�)
	if (Keyboard_IsKeyDown(KK_RIGHT)
		|| IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		g_Player.vel.x += PLAYER_ACCELE;

		// X���x�̏���l
		if (g_Player.vel.x > PLAYER_MAX_SPD)
		{
			g_Player.vel.x = PLAYER_MAX_SPD;
		}
		control_timer++;
		if (control_timer >= 20.0f)
		{
			g_Player.rot += g_Player.rot_vel;
			control_timer = 0.0f;
		}
	}

	//��(�ړ�)
	if (Keyboard_IsKeyDown(KK_UP)
		|| IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_UP))
	{
		g_Player.vel.y -= PLAYER_ACCELE;

		// y���x�̏���l
		if (g_Player.vel.y < -PLAYER_MAX_SPD)
		{
			g_Player.vel.y= -PLAYER_MAX_SPD;
		}

	}
	
	//��(�ړ�)
	if (Keyboard_IsKeyDown(KK_DOWN)
		|| IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_DOWN))
	{
		g_Player.vel.y += PLAYER_ACCELE;

		// y���x�̏���l
		if (g_Player.vel.y > PLAYER_MAX_SPD)
		{
			g_Player.vel.y = PLAYER_MAX_SPD;
		}
		
	}

	//����]
	if (Keyboard_IsKeyDown(KK_Q)
		|| IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		g_Player.rot -= 0.01f;
	}

	//�E��]
	if (Keyboard_IsKeyDown(KK_E)
		|| IsButtonPressed(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		g_Player.rot += 0.01f;
	}

}

void ZeroGravity(void)
{
	if (gravity_timer < 20.0f)
	{
		g_DrawGrav -= GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 20.0f && gravity_timer < 40.0f)
	{
		g_DrawGrav += GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 40.0f && gravity_timer < 60.0f)
	{
		g_DrawGrav += GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 60.0f && gravity_timer < 80.0f)
	{
		g_DrawGrav -= GRAVITY_VEL;
		gravity_timer++;
	}

	if (gravity_timer >= 80.0f)
	{
		gravity_timer = 0.0f;
	}
}

void FramePlayer(void)
{
	//��(�g)
	if (g_Player.pos.x <= 0.0f + (g_Player.size.x / 2))
	{
		g_Player.pos.x = 0.0f + (g_Player.size.x / 2);
	}
	//�E(�g)
	if (g_Player.pos.x >= (MAP_X * 60.0f) - (g_Player.size.x / 2))
	{
		g_Player.pos.x = (MAP_X * 60.0f) - (g_Player.size.x / 2);
	}

	//��(�g)
	if (g_Player.pos.y <= 0.0f + (g_Player.size.y / 2))
	{
		g_Player.pos.y = 0.0f + (g_Player.size.y / 2);
	}
	//��(�g)
	if (g_Player.pos.y >= (MAP_Y * 60.0f) - (g_Player.size.y / 2))
	{
		g_Player.pos.y = (MAP_Y * 60.0f) - (g_Player.size.y / 2);
	}
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

D3DXVECTOR2 GetPlayerPosition(void)
{
	return g_Player.pos;
}