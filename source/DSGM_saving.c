#include "DSGM.h"

FILE *DSGM_save;
char DSGM_saveName[256];

// Saving applies to both saving and loading
inline bool DSGM_StartSaving(void) {
	DSGM_save = fopen(DSGM_saveName, "r+b");
	return DSGM_save ? true : false;
}

inline void DSGM_FinishSaving(void) {
	fclose(DSGM_save);
}

void DSGM_WriteRoomFull(DSGM_Room *room) {
	u8 screen;
	int layerNumber;
	int group;
	int object;
	
	DSGM_Debug("Saving room...\n");
	
	fprintf(DSGM_save, "ROOM");
	
	for(screen = 0; screen < 2; screen++) {
		for(layerNumber = 0; layerNumber < 4; layerNumber++) {
			// Save layer
			fwrite(&room->layers[screen][layerNumber], sizeof(DSGM_Layer), 1, DSGM_save);
		}
	}
	
	for(screen = 0; screen < 2; screen++) {
		// Save object instances
		fwrite(&room->objectGroupCount[screen], sizeof(int), 1, DSGM_save);
		
		for(group = 0; group < room->objectGroupCount[screen]; group++) {
			fwrite(&room->objectGroups[screen][group].objectInstanceCount, sizeof(int), 1, DSGM_save);
			
			for(object = 0; object < room->objectGroups[screen][group].objectInstanceCount; object++) {
				DSGM_ObjectInstance *objectInstance = &room->objectGroups[screen][group].objectInstances[object];
				fwrite(objectInstance, sizeof(DSGM_ObjectInstance), 1, DSGM_save);
			}
		}
	}
	
	DSGM_Debug("Saved room\n");
}

void DSGM_ReadRoomFull(DSGM_Room *room) {
	u8 screen;
	int layerNumber;
	int group;
	int object;
	
	DSGM_Debug("Loading room...\n");
	
	char magic[5] = { 0 };
	fread(magic, sizeof(char), 4, DSGM_save);
	if(strncmp(magic, "ROOM", 4)) {
		DSGM_Debug("Failed! Bad magic...\n");
		return;
	}
	
	for(screen = 0; screen < 2; screen++) {
		for(layerNumber = 0; layerNumber < 4; layerNumber++) {
			// Load layer
			fread(&room->layers[screen][layerNumber], sizeof(DSGM_Layer), 1, DSGM_save);
		}
	}
	
	for(screen = 0; screen < 2; screen++) {
		// Load object instances
		fread(&room->objectGroupCount[screen], sizeof(int), 1, DSGM_save);
		room->objectGroups[screen] = realloc(room->objectGroups[screen], room->objectGroupCount[screen] * sizeof(DSGM_ObjectGroup));
		
		for(group = 0; group < room->objectGroupCount[screen]; group++) {
			fread(&room->objectGroups[screen][group].objectInstanceCount, sizeof(int), 1, DSGM_save);
			room->objectGroups[screen][group].objectInstances = realloc(room->objectGroups[screen][group].objectInstances, room->objectGroups[screen][group].objectInstanceCount * sizeof(DSGM_ObjectInstance));
			
			for(object = 0; object < room->objectGroups[screen][group].objectInstanceCount; object++) {
				DSGM_ObjectInstance *objectInstance = &room->objectGroups[screen][group].objectInstances[object];
				fread(objectInstance, sizeof(DSGM_ObjectInstance), 1, DSGM_save);
			}
		}
	}
	
	DSGM_Debug("Loaded room\n");
}
