#pragma once

extern FILE *DSGM_save;
extern char DSGM_saveName[256];

inline bool DSGM_StartSaving(void);
inline void DSGM_FinishSaving(void);

#define DSGM_WriteRoom() DSGM_WriteRoomFull(&DSGM_Rooms[DSGM_currentRoom])
void DSGM_WriteRoomFull(DSGM_Room *room);

/*#define DSGM_ReadRoom() do {\
	DSGM_ReadRoomFull(&DSGM_Rooms[DSGM_currentRoom]);\
	DSGM_ResetAll();\
	DSGM_LoadRoom(&DSGM_Rooms[DSGM_currentRoom]);\
	return;\
} while(0)*/

/*#define DSGM_ReadRoom() do {\
	DSGM_ReadRoomFull(&DSGM_Rooms[DSGM_currentRoom]);\
	DSGM_SwitchRoom(DSGM_currentRoom, false);\
} while(0)*/

#define DSGM_ReadRoom() DSGM_ReadRoomFull(&DSGM_Rooms[DSGM_currentRoom])
void DSGM_ReadRoomFull(DSGM_Room *room);
