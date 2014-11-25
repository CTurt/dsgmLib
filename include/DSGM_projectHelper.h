#pragma once

#include "DSGM.h"

#define DSGM_ALL_ROOMS -1

#define DSGM_ResetRoom() DSGM_SwitchRoom(DSGM_currentRoom, true)
#define DSGM_GotoPreviousRoom(persistent) DSGM_SwitchRoom(DSGM_currentRoom - 1, persistent)
#define DSGM_GotoNextRoom(persistent) DSGM_SwitchRoom(DSGM_currentRoom + 1, persistent)

void DSGM_Init(void) {
	DSGM_InitGFX();
	DSGM_InitRand();
	DSGM_InitNitro();
	DSGM_InitSoundFull(DSGM_SOUND_STREAM_COUNT);
	
	DSGM_ClearPalettes(DSGM_Palettes, DSGM_PALETTE_COUNT);
	DSGM_SetupRooms(DSGM_ALL_ROOMS);
}

int main(int argc, char **argv) {
	DSGM_Init();
	
	DSGM_LoadRoom(&DSGM_Rooms[DSGM_currentRoom]);
	
	while(1) {
		DSGM_LoopRoom(&DSGM_Rooms[DSGM_currentRoom]);
		
		DSGM_Update();
	}
	
	return 0;
}

#define DSGM_GetGroup(objectInstance) DSGM_GetObjectGroupFull(&DSGM_Rooms[DSGM_currentRoom], objectInstance->screen, objectInstance->object)

#define DSGM_GetObjectGroup(screen, object) DSGM_GetObjectGroupFull(&DSGM_Rooms[DSGM_currentRoom], screen, object)

#define DSGM_PlaySound(sound) DSGM_PlaySoundFull(&DSGM_Sounds[sound])

#define DSGM_PlaySoundAdvanced(sound, volume, panning) DSGM_PlaySoundAdvancedFull(&DSGM_Sounds[sound], volume, panning)

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
	DSGM_ResetSound();
	DSGM_ClearPalettes(DSGM_Palettes, DSGM_PALETTE_COUNT);
	DSGM_FreeAll();
}

void DSGM_SwitchRoom(int room, bool reset) {
	if(room >= DSGM_ROOM_COUNT) room = 0;
	if(room < 0) room = DSGM_ROOM_COUNT - 1;
	
	DSGM_ResetAll();
	
	DSGM_currentRoom = room;
	
	if(reset) {
		DSGM_SetupRooms(room);
	}
	
	DSGM_LoadRoom(&DSGM_Rooms[room]);
	
	DSGM_invalidateRoom = 1;
}
