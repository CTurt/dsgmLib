#pragma once

typedef struct {
	DSGM_BackgroundInstance backgroundInstances[2][4];
	
	DSGM_View initialView[2];
	DSGM_View view[2];
	
	DSGM_ObjectGroup *objectGroups[2];
	int objectGroupCount[2];
} DSGM_Room;

void DSGM_SetupViews(DSGM_Room *room);

void DSGM_LoadRoom(DSGM_Room *room);
void DSGM_LoopRoom(DSGM_Room *room);
void DSGM_LeaveRoom(DSGM_Room *room);
