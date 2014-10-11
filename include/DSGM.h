#pragma once

#include <nds.h>

#include <fat.h>
#include <filesystem.h>

#include <maxmod9.h>

#include <dswifi9.h>
#include <dsgmDSWiFi.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define DSGM_TOP 1
#define DSGM_BOTTOM 0

#define DSGM_ALIGN_CHAR __attribute__((aligned(sizeof(char))))
#define DSGM_ALIGN_SHORT __attribute__((aligned(sizeof(short))))
#define DSGM_ALIGN_INT __attribute__((aligned(sizeof(int))))
#define DSGM_ALIGN_LONG __attribute__((aligned(sizeof(long))))

typedef char DSGM_CharStructLabel[0];
typedef short DSGM_ShortStructLabel[0];
typedef int DSGM_IntStructLabel[0];

#include "DSGM_3D.h"
#include "DSGM_misc.h"
#include "DSGM_malloc.h"
#include "DSGM_file.h"
#include "DSGM_input.h"
#include "DSGM_alarm.h"
#include "DSGM_view.h"
#include "DSGM_background.h"
#include "DSGM_text.h"
#include "DSGM_palette.h"
#include "DSGM_sprite.h"
typedef DSGM_SpriteEntry DSGM_OAMStructLabel[0];
#include "DSGM_object.h"
#include "DSGM_sound.h"
#include "DSGM_room.h"
#include "DSGM_drawable.h"
#include "DSGM_wireless.h"
#include "DSGM_displayList.h"
#include "DSGM_MD2.h"

#ifndef BUILDINGLIB
	#include "project.h"
#endif

void DSGM_Debug(char *text, ...);
void DSGM_Log(bool wait, char *text, ...);

void DSGM_InitGFX(void);
void DSGM_InitCustomGFX(bool topScreenDrawable, bool bottomScreenDrawable, bool main3D);
void DSGM_InitRand(void);
void DSGM_InitNitro(void);

void DSGM_Update(void);
