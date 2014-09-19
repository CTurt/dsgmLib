#pragma once

#include "DSGM.h"

#define DSGM_ALL_ROOMS -1

#define DSGM_ResetRoom() DSGM_SwitchRoom(DSGM_currentRoom, true)

void DSGM_Init(void) {
	DSGM_InitGFX();
	DSGM_InitRand();
	DSGM_InitNitro();
	DSGM_InitSoundFull(DSGM_SOUND_STREAM_COUNT);
	
	DSGM_ClearPalettes(DSGM_Palettes, DSGM_PALETTE_COUNT);
	DSGM_SetupRooms(DSGM_ALL_ROOMS);
}

#define DSGM_GetGroup(objectInstance) DSGM_GetObjectGroupFull(&DSGM_Rooms[DSGM_currentRoom], objectInstance->screen, objectInstance->object)
/*inline DSGM_ObjectGroup *DSGM_GetGroup(DSGM_ObjectInstance *objectInstance) {
	return DSGM_GetObjectGroupFull(&DSGM_Rooms[DSGM_currentRoom], objectInstance->screen, objectInstance->object);
}*/

#define DSGM_GetObjectGroup(screen, object) DSGM_GetObjectGroupFull(&DSGM_Rooms[DSGM_currentRoom], screen, object)
/*inline DSGM_ObjectGroup *DSGM_GetObjectGroup(u8 screen, DSGM_Object *object) {
	return DSGM_GetObjectGroupFull(&DSGM_Rooms[DSGM_currentRoom], screen, object);
}*/

#define DSGM_PlaySound(sound) DSGM_PlaySoundFull(&DSGM_Sounds[sound])
/*inline DSGM_SoundInstance *DSGM_PlaySound(int sound) {
	return DSGM_PlaySoundFull(&DSGM_Sounds[sound]);
}*/

#define DSGM_PlaySoundAdvanced(sound, volume, panning) DSGM_PlaySoundAdvancedFull(&DSGM_Sounds[sound], volume, panning)
/*inline DSGM_SoundInstance *DSGM_PlaySoundAdvanced(int sound, u8 volume, u8 panning) {
	return DSGM_PlaySoundAdvancedFull(&DSGM_Sounds[sound], volume, panning);
}*/

inline bool DSGM_BackgroundIsNitro(int background) {
	return DSGM_BackgroundIsNitroFull(&DSGM_Backgrounds[background]);
}

void DSGM_LoadBackground(u8 screen, int layerNumber, int background, bool attachedToView) {
	DSGM_Layer layer;
	layer.background = &DSGM_Backgrounds[background];
	layer.screen = screen;
	layer.layerNumber = layerNumber;
	layer.attachedToView = attachedToView;
	layer.x = 0;
	layer.y = 0;
	DSGM_LoadBackgroundFull(&layer);
}

void DSGM_ScrollBackground(u8 screen, int layerNumber, int x, int y) {
	DSGM_layers[screen][layerNumber].x = x;
	DSGM_layers[screen][layerNumber].y = y;
}

void DSGM_ResetAll(void) {
	DSGM_ClearText(DSGM_TOP);
	DSGM_ClearText(DSGM_BOTTOM);
	DSGM_ResetSprites(DSGM_Sprites, DSGM_SPRITE_COUNT);
	DSGM_ClearPalettes(DSGM_Palettes, DSGM_PALETTE_COUNT);
	DSGM_ResetSound();
	DSGM_FreeAll();
}

void DSGM_SwitchRoom(int room, bool reset) {
	DSGM_invalidateRoom = 1;
	
	DSGM_ResetAll();
	
	DSGM_currentRoom = room;
	
	if(reset) {
		DSGM_SetupRooms(room);
	}
	
	DSGM_LoadRoom(&DSGM_Rooms[room]);
}
