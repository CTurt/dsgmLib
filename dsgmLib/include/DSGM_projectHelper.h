#pragma once

#include "DSGM.h"

#define DSGM_ALL_ROOMS -1

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

void DSGM_LoadBackground(u8 screen, int layer, int background, bool attachedToView) {
	DSGM_BackgroundInstance backgroundInstance;
	backgroundInstance.background = &DSGM_Backgrounds[background];
	backgroundInstance.screen = screen;
	backgroundInstance.layer = layer;
	backgroundInstance.attachedToView = attachedToView;
	backgroundInstance.x = 0;
	backgroundInstance.y = 0;
	DSGM_LoadBackgroundFull(&backgroundInstance);
}

void DSGM_ScrollBackground(u8 screen, int layer, int x, int y) {
	DSGM_Rooms[DSGM_currentRoom].backgroundInstances[screen][layer].x = x;
	DSGM_Rooms[DSGM_currentRoom].backgroundInstances[screen][layer].y = y;
}

void DSGM_SwitchRoom(int room, bool reset) {
	//DSGM_LeaveRoom(&DSGM_Rooms[DSGM_currentRoom]);
	
	DSGM_ClearText(DSGM_TOP);
	DSGM_ClearText(DSGM_BOTTOM);
	DSGM_ResetSprites(DSGM_Sprites, DSGM_SPRITE_COUNT);
	DSGM_ClearPalettes(DSGM_Palettes, DSGM_PALETTE_COUNT);
	DSGM_ResetSound();
	DSGM_FreeAll();
	
	DSGM_currentRoom = room;
	
	if(reset) {
		DSGM_SetupRooms(room);
	}
	
	DSGM_LoadRoom(&DSGM_Rooms[room]);
}
