#pragma once

#include <nds.h>

#include <filesystem.h>
#include <maxmod9.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include "DSGM_malloc.h"
#include "DSGM_file.h"
#include "DSGM_input.h"
#include "DSGM_alarm.h"
#include "DSGM_view.h"
#include "DSGM_background.h"
#include "DSGM_text.h"
#include "DSGM_palette.h"
#include "DSGM_sprite.h"
#include "DSGM_object.h"
#include "DSGM_sound.h"
#include "DSGM_room.h"

#ifndef BUILDINGLIB
	#include "project.h"
#endif

#define DSGM_TOP 1
#define DSGM_BOTTOM 0

void DSGM_Debug(char *text, ...);

void DSGM_InitGFX(void);
void DSGM_InitRand(void);
void DSGM_InitNitro(void);

inline int DSGM_Random(int min, int max);

void DSGM_Update(void);
