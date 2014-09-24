#pragma once

#define DSGM_ValidateRoom() do {\
	if(DSGM_invalidateRoom) {\
		DSGM_invalidateRoom = 0;\
		return;\
	}\
} while(0)

typedef void (*DSGM_RoomHandler)(void);

typedef struct {
	DSGM_Layer layers[2][4];
	
	DSGM_View initialView[2];
	DSGM_View view[2];
	
	DSGM_RoomHandler handler;
	
	DSGM_ObjectGroup *objectGroups[2];
	int objectGroupCount[2];
} DSGM_Room;

extern bool DSGM_invalidateRoom;

void DSGM_SetupViews(DSGM_Room *room);

void DSGM_LoadRoom(DSGM_Room *room);
void DSGM_LoopRoom(DSGM_Room *room);
void DSGM_LeaveRoom(DSGM_Room *room);

#define DSGM_GetRoomNumber(room) DSGM_GetRoomNumberFull(DSGM_Rooms, room) 
inline int DSGM_GetRoomNumberFull(DSGM_Room *rooms, DSGM_Room *room);
