#include "DSGM.h"

bool DSGM_invalidateRoom = 0;

void DSGM_SetupViews(DSGM_Room *room) {
	room->view[DSGM_TOP].x = room->initialView[DSGM_TOP].x;
	room->view[DSGM_BOTTOM].x = room->initialView[DSGM_BOTTOM].x;
	room->view[DSGM_TOP].y = room->initialView[DSGM_TOP].y;
	room->view[DSGM_BOTTOM].y = room->initialView[DSGM_BOTTOM].y;
}

void DSGM_LoadRoom(DSGM_Room *room) {
	u8 screen;
	int layerNumber;
	int group;
	int object;
	
	DSGM_Debug("Loading room...\n");
	
	DSGM_invalidateRoom = 0;
	
	{
		bool requiresMode3[2] = {false, false};
		
		for(screen = 0; screen < 2; screen++) {
			if(room->layers[screen][3].background == DSGM_DRAWABLE_BACKGROUND || room->layers[screen][3].background->type == BgType_ExRotation) {
				DSGM_Debug("Screen %d requires Mode 3\n", screen);
				requiresMode3[screen] = true;
			}
		}
		
		DSGM_InitCustomGFX(requiresMode3[DSGM_TOP], requiresMode3[DSGM_BOTTOM]);
	}
	
	for(screen = 0; screen < 2; screen++) {
		// Load backgrounds
		for(layerNumber = 0; layerNumber < 4; layerNumber++) {
			if(room->layers[screen][layerNumber].background != DSGM_NO_BACKGROUND) {
				DSGM_Debug("Screen %d, layerNumber %d, background %p\n", screen, layerNumber, room->layers[screen][layerNumber].background);
				if(room->layers[screen][layerNumber].background == DSGM_DEFAULT_FONT) DSGM_InitText(&room->layers[screen][layerNumber]);
				else if(room->layers[screen][layerNumber].background == DSGM_DRAWABLE_BACKGROUND) {
					if(layerNumber == 3) DSGM_InitDrawableBackground(&room->layers[screen][layerNumber]);
				}
				else {
					DSGM_LoadBackgroundFull(&room->layers[screen][layerNumber]);
					if(room->layers[screen][layerNumber].background->type == BgType_Text4bpp) DSGM_InitText(&room->layers[screen][layerNumber]);
				}
				DSGM_ScrollBackgroundFull(&room->view[screen], &room->layers[screen][layerNumber]);
				bgUpdate();
			}
		}
	}
	
	for(screen = 0; screen < 2; screen++) {
		// Load sprites (all DSGM_ObjectInstances who have a sprite)
		for(group = 0; group < room->objectGroupCount[screen]; group++) {
			for(object = 0; object < room->objectGroups[screen][group].objectInstanceCount; object++) {
				DSGM_ObjectInstance *objectInstance = &room->objectGroups[screen][group].objectInstances[object];
				DSGM_ActivateObjectInstance(room, objectInstance);
				DSGM_ValidateRoom();
			}
		}
	}
	
	DSGM_Debug("Loaded\n");
}

//int res = 0;

void DSGM_LoopRoom(DSGM_Room *room) {
	u8 screen;
	int layerNumber;
	int group;
	int object;
	int sound;
	int rotset;
	
	//if(!res) {
	//	mmStop();
	//	mmStart(/*DSGM_Sounds[FlatOutLies].ID*/0, MM_PLAY_LOOP);
	//	res = 1;
	//}
	
	if(room->handler) room->handler();
	DSGM_ValidateRoom();
	
	//+1?
	for(sound = 0; sound < DSGM_soundInstanceCount; sound++) {
		DSGM_SetSoundInstanceVolumeFull(&DSGM_soundInstances[sound], DSGM_soundInstances[sound].volume);
		DSGM_SetSoundInstancePanningFull(&DSGM_soundInstances[sound], DSGM_soundInstances[sound].panning);
	}
	
	for(screen = 0; screen < 2; screen++) {
		for(layerNumber = 0; layerNumber < 4; layerNumber++) {
			if(room->layers[screen][layerNumber].background != DSGM_NO_BACKGROUND) {
				DSGM_ScrollBackgroundFull(&room->view[screen], &room->layers[screen][layerNumber]);
			}
		}
		
		//DSGM_Debug("Group count %d\n", room->objectGroupCount[screen]);
		for(group = 0; group < room->objectGroupCount[screen]; group++) {
			for(object = 0; object < room->objectGroups[screen][group].objectInstanceCount; object++) {
				#define objectInstance (&room->objectGroups[screen][group].objectInstances[object])
				
				// Calculate position
				int x = objectInstance->x - room->view[screen].x;
				int y = objectInstance->y - room->view[screen].y;
				if(x < 256 && x > -128 && y < 192 && y > -64 && !objectInstance->hide) {
					x = objectInstance->angle ? x - DSGM_GetSpriteWidth(objectInstance->object->sprite) / 2 : x;
					y = objectInstance->angle ? y - DSGM_GetSpriteHeight(objectInstance->object->sprite) / 2 : y;
				}
				else {
					x = 255;
					y = 191;
				}
				
				objectInstance->absoluteX = x;
				objectInstance->absoluteY = y;
				
				// Calculate GFX index
				objectInstance->gfxIndex = oamGfxPtrToOffset(objectInstance->screen == DSGM_TOP ? &oamMain : &oamSub, (objectInstance->screen == DSGM_TOP ? objectInstance->object->sprite->topTiles : objectInstance->object->sprite->bottomTiles)[objectInstance->frame]);
				
				// Copy into OAM
				memcpy(&(objectInstance->screen == DSGM_TOP ? oamMain : oamSub).oamMemory[objectInstance->spriteNumber], &objectInstance->oam, sizeof(DSGM_SpriteEntry));
				
				// Maybe one of these is better than memcpy?
				//DC_FlushAll();
				//dmaCopy();
				//dmaCopyHalfWords();
				//swifastcopy();
				
				if(objectInstance->object->loop) objectInstance->object->loop(objectInstance);
				DSGM_ValidateRoom();
				
				if(screen == DSGM_BOTTOM && objectInstance->object->touch) {
					if(DSGM_newpress.Stylus && DSGM_StylusOverObjectInstanceFull(room, objectInstance)) {
						objectInstance->object->touch(objectInstance);
						DSGM_ValidateRoom();
					}
				}
				
				int collisionEvent;
				for(collisionEvent = 0; collisionEvent < objectInstance->object->collisionEventCount; collisionEvent++) {
					int collider;
					DSGM_ObjectGroup *colliderGroup = DSGM_GetObjectGroupFull(room, screen, objectInstance->object->collisionEvents[collisionEvent].collider);
					for(collider = 0; collider < colliderGroup->objectInstanceCount; collider++) {
						if(DSGM_ObjectInstanceCollision(objectInstance, &colliderGroup->objectInstances[collider])) {
							objectInstance->object->collisionEvents[collisionEvent].function(objectInstance, &colliderGroup->objectInstances[collider]);
							DSGM_ValidateRoom();
						}
					}
				}
				
				#undef objectInstance
			}
		}
		
		for(rotset = 0; rotset < 32; rotset++) {
			DSGM_SetRotset(screen, rotset, DSGM_rotations[screen][rotset], DSGM_scales[screen][rotset].x, DSGM_scales[screen][rotset].y);
		}
	}
	
	//if(!res) {
	//	mmStop();
	//	mmStart(/*DSGM_Sounds[FlatOutLies].ID*/0, MM_PLAY_LOOP);
	//	res = 1;
	//}
}

void DSGM_LeaveRoom(DSGM_Room *room) {
	DSGM_Debug("Leaving room\n");
	
	u8 screen;
	int group;
	int object;
	
	for(screen = 0; screen < 2; screen++) {
		for(group = 0; group < room->objectGroupCount[screen]; group++) {
			for(object = 0; object < room->objectGroups[screen][group].objectInstanceCount; object++) {
				DSGM_Debug("Freeing object instance custom variables\n");
				free(room->objectGroups[screen][group].objectInstances[object].variables);
			}
			//DSGM_Debug("Freeing object events at %p\n", room->objectGroups[screen][group].objectInstances[0].object->collisionEvents);
			//free(room->objectGroups[screen][group].objectInstances[0].object->collisionEvents);
			//room->objectGroups[screen][group].objectInstances[0].object->collisionEvents = NULL;
			//room->objectGroups[screen][group].objectInstances[0].object->collisionEventCount = 0;
			DSGM_Debug("Freeing object events at %p\n", room->objectGroups[screen][group].object->collisionEvents);
			free(room->objectGroups[screen][group].object->collisionEvents);
			room->objectGroups[screen][group].object->collisionEvents = NULL;
			room->objectGroups[screen][group].object->collisionEventCount = 0;
			DSGM_Debug("Freeing object instances at address %p on %s screen\n", room->objectGroups[screen][group].objectInstances, screen == DSGM_TOP ? "top" : "bottom");
			free(room->objectGroups[screen][group].objectInstances);
		}
		
		DSGM_Debug("Freeing object groups at address %p on %s screen\n", room->objectGroups[screen], screen == DSGM_TOP ? "top" : "bottom");
		free(room->objectGroups[screen]);
	}
}

unsigned char DSGM_SaveRoom(DSGM_Room *room, char *filename) {
	/*u8 screen;
	int layerNumber;
	int group;
	int object;
	FILE *f = fopen(filename, "ab");
	if(!f) return 0;
	
	DSGM_Debug("Saving room...\n");
	
	for(screen = 0; screen < 2; screen++) {
		// Save backgrounds
		for(layerNumber = 0; layerNumber < 4; layerNumber++) {
			// Store pointer to background
			fwrite(&room->layers[screen][layerNumber].background, sizeof(DSGM_Background *), 1, f) {
		}
	}
	
	for(screen = 0; screen < 2; screen++) {
		// Save object instances
		for(group = 0; group < room->objectGroupCount[screen]; group++) {
			for(object = 0; object < room->objectGroups[screen][group].objectInstanceCount; object++) {
				DSGM_ObjectInstance *objectInstance = &room->objectGroups[screen][group].objectInstances[object];
				fwrite(objectInstance, sizeof(DSGM_ObjectInstance), 1, f);
			}
		}
	}
	
	fclose(f);
	
	DSGM_Debug("Saved\n");*/
	
	return 1;
}

inline int DSGM_GetRoomNumberFull(DSGM_Room *rooms, DSGM_Room *room) {
	return room - rooms;
}
