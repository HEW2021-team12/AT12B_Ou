#pragma once

#include "main.h"
#include "renderer.h"


typedef struct
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;
	float timer_u;
	float timer_v;
	float rot;

}TIMER;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);