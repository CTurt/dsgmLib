#include "DSGM.h"

void *me = NULL;
DSGM_ObjectInstance *DSGM_invalidObjectInstance = NULL;

void DSGM_SetupObjectGroups(DSGM_Room *room, u8 screen, int objectGroupCount) {
	room->objectGroupCount[screen] = objectGroupCount;
	if(objectGroupCount > 0) {
		DSGM_Debug("Allocating %d object group(s) on %s screen\n", objectGroupCount, screen == DSGM_TOP ? "top" : "bottom");
		room->objectGroups[screen] = malloc(objectGroupCount * sizeof(DSGM_ObjectGroup));
		DSGM_Debug("Address %p was given\n", room->objectGroups[screen]);
	}
	else room->objectGroups[screen] = NULL;
}

void DSGM_SetupObjectInstances(DSGM_ObjectGroup *group, DSGM_Object *object, u8 screen, int objectInstanceCount, ...) {
	va_list properties;
	int i;
	group->object = object;
	group->objectInstanceCount = objectInstanceCount;
	if(objectInstanceCount > 0) {
		DSGM_Debug("Allocating %d object instance(s) on %s screen\n", objectInstanceCount, screen == DSGM_TOP ? "top" : "bottom");
		group->objectInstances = malloc(objectInstanceCount * sizeof(DSGM_ObjectInstance));
		DSGM_Debug("Address %p was given\n", group->objectInstances);
	}
	else group->objectInstances = NULL;
	va_start(properties, objectInstanceCount);
	for(i = 0; i < objectInstanceCount; i++) {
		memset(&group->objectInstances[i], 0, sizeof(DSGM_ObjectInstance));
		group->objectInstances[i].object = object;
		group->objectInstances[i].screen = screen;
		group->objectInstances[i].x = va_arg(properties, int);
		group->objectInstances[i].y = va_arg(properties, int);
		group->objectInstances[i].bitshift = DSGM_DEFAULT_BITSHIFT;
		group->objectInstances[i].variables = malloc(object->customVariablesSize);
		memset(group->objectInstances[i].variables, 0, sizeof(object->customVariablesSize));
	}
	va_end(properties);
}

void DSGM_RedistributeSpriteNumbers(DSGM_Room *room, u8 screen) {
	int group;
	int object;
	int rotset;
	
	DSGM_nextFreeSprite[screen] = 0;
	
	for(group = 0; group < room->objectGroupCount[screen]; group++) {
		for(object = 0; object < room->objectGroups[screen][group].objectInstanceCount; object++) {
			if(room->objectGroups[screen][group].objectInstances[object].object->sprite != DSGM_NO_SPRITE) {
				DSGM_ObjectInstance *objectInstance = &room->objectGroups[screen][group].objectInstances[object];
				
				objectInstance->spriteNumber = DSGM_NextFreeSpriteNumber(screen);
				
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
			}
		}
	}
	
	for(rotset = 0; rotset < 32; rotset++) {
		DSGM_SetRotset(screen, rotset, DSGM_rotations[screen][rotset], DSGM_scales[screen][rotset].x, DSGM_scales[screen][rotset].y);
	}
}

void DSGM_ActivateObjectInstance(DSGM_Room *room, DSGM_ObjectInstance *objectInstance) {
	{
		u8 screen;
		for(screen = 0; screen < 2; screen++) {
			int rotset;
			for(rotset = 0; rotset < 32; rotset++) {
				DSGM_SetRotset(screen, rotset, DSGM_rotations[screen][rotset], DSGM_scales[screen][rotset].x, DSGM_scales[screen][rotset].y);
			}
		}
	}
	
	DSGM_Sprite *sprite = objectInstance->object->sprite;
	u8 screen = objectInstance->screen;
	int spriteNumber = -1;
	
	if(sprite) {
		DSGM_Palette *palette = NULL;
		
		int x, y;
		
		spriteNumber = DSGM_NextFreeSpriteNumber(screen);
		objectInstance->spriteNumber = spriteNumber;
		objectInstance->screen = screen;
		
		if(!DSGM_PaletteLoaded(screen, palette)) {
			DSGM_LoadPaletteFull(screen, palette);
		}
		
		if(!DSGM_SpriteLoaded(screen, sprite)) {
			DSGM_LoadSpriteFull(screen, sprite);
		}
		
		// Initially create sprite offscreen
		DSGM_CreateSprite(screen, spriteNumber, 255, 191, objectInstance->priority, objectInstance->frame, objectInstance->hFlip, objectInstance->vFlip, sprite);
		
		// Extract DSGM_SpriteEntry properties into objectInstance
		memcpy(&objectInstance->oam, &(objectInstance->screen == DSGM_TOP ? oamMain : oamSub).oamMemory[objectInstance->spriteNumber], sizeof(DSGM_SpriteEntry));
		
		// Run create event before showing sprite so the create event can change the position, frame, flipping, etc...
		if(objectInstance->object->create) objectInstance->object->create(objectInstance);
		
		// Don't need to take into account double size here
		x = objectInstance->x - room->view[screen].x;
		y = objectInstance->y - room->view[screen].y;
		if(!(x < 256 && x > -128 && y < 192 && y > -64 && !objectInstance->hide)) {
			x = 255;
			y = 191;
		}
		
		// Recreate sprite
		DSGM_CreateSprite(screen, spriteNumber, x, y, objectInstance->priority, objectInstance->frame, objectInstance->hFlip, objectInstance->vFlip, sprite);
	}
	else {
		if(objectInstance->object->create) objectInstance->object->create(objectInstance);
	}
}

DSGM_ObjectGroup *DSGM_GetObjectGroupFull(DSGM_Room *room, u8 screen, DSGM_Object *object) {
	int i;
	for(i = 0; i < room->objectGroupCount[screen]; i++) {
		if(room->objectGroups[screen][i].object == object) return &room->objectGroups[screen][i];
	}
	return NULL;
}

DSGM_ObjectInstance *DSGM_CreateObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance **meP, u8 screen, int x, int y, DSGM_Object *object) {
	DSGM_ObjectInstanceRelation relation = { 0, 0 };
	if(meP && *meP) relation = DSGM_GetObjectInstanceRelationFull(room, *meP);
	
	DSGM_ObjectGroup *group = DSGM_GetObjectGroupFull(room, screen, object);
	
	DSGM_Debug("Creating object instance %p\n", object);
	
	if(group) {
		DSGM_Debug("Object group found at %p\n", group);
		
		DSGM_Debug("Reallocating object instances at %p\n", group->objectInstances);
		group->objectInstances = realloc(group->objectInstances, (group->objectInstanceCount + 1) * sizeof(DSGM_ObjectInstance));
		DSGM_Debug("Gave address %p for size %d\n", group->objectInstances, (group->objectInstanceCount + 1) * sizeof(DSGM_ObjectInstance));
		
		memset(&group->objectInstances[group->objectInstanceCount], 0, sizeof(DSGM_ObjectInstance));
		group->objectInstances[group->objectInstanceCount].object = object;
		group->objectInstances[group->objectInstanceCount].screen = screen;
		group->objectInstances[group->objectInstanceCount].x = x;
		group->objectInstances[group->objectInstanceCount].y = y;
		group->objectInstances[group->objectInstanceCount].bitshift = DSGM_DEFAULT_BITSHIFT;
		group->objectInstances[group->objectInstanceCount].variables = malloc(object->customVariablesSize);
		DSGM_Debug("Allocated custom variables %p for size %d\n", group->objectInstances[group->objectInstanceCount].variables, object->customVariablesSize);
		memset(group->objectInstances[group->objectInstanceCount].variables, 0, sizeof(object->customVariablesSize));
		
		DSGM_ActivateObjectInstance(room, &group->objectInstances[group->objectInstanceCount]);
		
		if(meP && *meP) *meP = DSGM_GetMeFromObjectInstanceRelationFull(room, &relation);
		
		return &group->objectInstances[group->objectInstanceCount++];
	}
	else {
		DSGM_Debug("No object group for object, creating object group\n");
		
		DSGM_Debug("Reallocating object groups at %p\n", room->objectGroups[screen]);
		room->objectGroups[screen] = realloc(room->objectGroups[screen], (room->objectGroupCount[screen] + 1) * sizeof(DSGM_ObjectGroup));
		DSGM_Debug("Gave address %p\n", room->objectGroups[screen]);
		
		group = &room->objectGroups[screen][room->objectGroupCount[screen]++];
		
		DSGM_SetupObjectInstances(group, object, screen, 1, x, y);
		/*{
			group->objectInstanceCount = 1;
			group->objectInstances = malloc(sizeof(DSGM_ObjectInstance));
			
			memset(&group->objectInstances[0], 0, sizeof(DSGM_ObjectInstance));
			group->objectInstances[0].object = object;
			group->objectInstances[0].screen = screen;
			group->objectInstances[0].x = x;
			group->objectInstances[0].y = y;
			group->objectInstances[0].bitshift = DSGM_DEFAULT_BITSHIFT;
			group->objectInstances[0].variables = malloc(object->customVariablesSize);
			memset(group->objectInstances[0].variables, 0, sizeof(object->customVariablesSize));
		}*/
		
		DSGM_ActivateObjectInstance(room, &group->objectInstances[0]);
		
		if(meP && *meP) *meP = DSGM_GetMeFromObjectInstanceRelationFull(room, &relation);
		
		return &group->objectInstances[0];
	}
}

void DSGM_DeleteObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance **meP, DSGM_ObjectInstance *objectInstance) {
	DSGM_ObjectInstanceRelation relation = { 0, 0 };
	if(meP && *meP) relation = DSGM_GetObjectInstanceRelationFull(room, *meP);
	
	DSGM_ObjectGroup *group = DSGM_GetObjectGroupFull(room, objectInstance->screen, objectInstance->object);
	
	if(!group) {
		DSGM_Debug("PEBCAK! Supplied a bad object instance %p, object %p, no object group could be found\n", objectInstance, objectInstance->object);
		return;
	}
	
	bool spriteNumbersChange = 0;
	u8 screen = objectInstance->screen;
	
	if(objectInstance->object->sprite != DSGM_NO_SPRITE) {
		spriteNumbersChange = 1;
	}
	
	int ID = DSGM_GetObjectInstanceIDFull(room, objectInstance);
	DSGM_Debug("Deleting object instance with ID %d\n", ID);
	
	if(objectInstance->object->destroy) objectInstance->object->destroy(objectInstance);
	
	DSGM_DeinitObjectInstanceRotScale(objectInstance);
	
	DSGM_invalidObjectInstance = objectInstance;
	
	if(ID < group->objectInstanceCount - 1) {
		DSGM_Debug("Shifting %d object instances for deletion\n", (group->objectInstanceCount - ID - 1));
		memcpy(&group->objectInstances[ID], &group->objectInstances[ID + 1], (group->objectInstanceCount - ID - 1) * sizeof(DSGM_ObjectInstance));
	}
	
	group->objectInstances = realloc(group->objectInstances, --group->objectInstanceCount * sizeof(DSGM_ObjectInstance));
	
	if(spriteNumbersChange) {
		int i;
		for(i = 0; i < 128; i++) {	
			oamSet(screen == DSGM_TOP ? &oamMain : &oamSub, i, 0, 0, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, NULL, -1, false, true, false, false, false);
		}
		DSGM_RedistributeSpriteNumbers(room, screen);
	}
	
	if(meP && *meP) *meP = DSGM_GetMeFromObjectInstanceRelationFull(room, &relation);
}

DSGM_ObjectInstanceRelation DSGM_GetObjectInstanceRelationFull(DSGM_Room *room, DSGM_ObjectInstance *me) {
	DSGM_ObjectInstanceRelation relation = { 0, 0 };
	if(me) {
		DSGM_ObjectGroup *group = DSGM_GetObjectGroupFull(room, me->screen, me->object);
		relation.screen = me->screen;
		relation.ID = ((void *)me - (void *)(group->objectInstances)) / sizeof(DSGM_ObjectInstance);
		relation.groupN = ((void *)group - (void *)room->objectGroups[me->screen]) / sizeof(DSGM_ObjectGroup);
	}
	
	DSGM_Debug("Getting relation: group %d, ID %d\n", relation.groupN, relation.ID);
	
	return relation;
}

inline void *DSGM_GetMeFromObjectInstanceRelationFull(DSGM_Room *room, DSGM_ObjectInstanceRelation *relation) {
	DSGM_Debug("Getting me value with group %d, ID %d\n", relation->groupN, relation->ID);
	return (void *)&room->objectGroups[relation->screen][relation->groupN].objectInstances[relation->ID];
}

void (DSGM_AddCollisionEvent)(DSGM_Object *object, DSGM_Object *collider, DSGM_CollisionEventFunction function) {
	DSGM_Debug("Adding collision event\n");
	object->collisionEvents = realloc(object->collisionEvents, (object->collisionEventCount + 1) * sizeof(DSGM_CollisionEvent));
	object->collisionEvents[object->collisionEventCount].collider = collider;
	object->collisionEvents[object->collisionEventCount].function = function;
	object->collisionEventCount++;
}

inline bool DSGM_StylusOverObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance *me) {
	return DSGM_stylus.x >= me->x - room->view[me->screen].x && DSGM_stylus.x <= me->x - room->view[me->screen].x + DSGM_GetSpriteWidth(me->object->sprite) && DSGM_stylus.y >= me->y - room->view[me->screen].y && DSGM_stylus.y <= me->y - room->view[me->screen].y + DSGM_GetSpriteHeight(me->object->sprite);
}

inline bool DSGM_ObjectInstanceOnScreenFull(DSGM_Room *room, DSGM_ObjectInstance *me) {	
	return ((!(me->x - room->view[me->screen].x >= 255)) && (!(me->y - room->view[me->screen].y >= 191))) &&\
	((!(me->x - room->view[me->screen].x <= -DSGM_GetSpriteWidth(me->object->sprite))) && (!(me->y - room->view[me->screen].y <= -DSGM_GetSpriteHeight(me->object->sprite)))) &&\
	!me->hide;
}

inline bool (DSGM_ObjectInstanceCollision)(DSGM_ObjectInstance *me, DSGM_ObjectInstance *collider) {
	int w1 = DSGM_GetSpriteWidth(me->object->sprite);
	int w2 = DSGM_GetSpriteWidth(collider->object->sprite);
	int h1 = DSGM_GetSpriteHeight(me->object->sprite);
	int h2 = DSGM_GetSpriteHeight(collider->object->sprite);
	
	return ((collider->x > me->x - w2) && (collider->x < me->x + w1)) && ((collider->y > me->y - h2) && (collider->y < me->y + h1));
}

inline int (DSGM_GetObjectInstanceRotset)(DSGM_ObjectInstance *me) {
	return me->angle - DSGM_rotations[me->screen];
}

inline int DSGM_GetObjectInstanceIDFull(DSGM_Room *room, DSGM_ObjectInstance *me) {
	DSGM_ObjectInstance *group = DSGM_GetObjectGroupFull(room, me->screen, me->object)->objectInstances;
	return me - group;
}

void (DSGM_InitObjectInstanceRotScale)(DSGM_ObjectInstance *me) {
	int rotset = DSGM_NextFreeRotset(me->screen);
	DSGM_rotsetTracker[me->screen][rotset]++;
	me->angle = &DSGM_rotations[me->screen][rotset];
	me->scale = &DSGM_scales[me->screen][rotset];
	me->rotationIndex = rotset;
	me->isSizeDouble = true;
	me->isRotateScale = true;
}

void (DSGM_InitSharedObjectInstanceRotScale)(DSGM_ObjectInstance *me, int rotset) {
	DSGM_rotsetTracker[me->screen][rotset]++;
	me->angle = &DSGM_rotations[me->screen][rotset];
	me->scale = &DSGM_scales[me->screen][rotset];
	me->rotationIndex = rotset;
	me->isSizeDouble = true;
	me->isRotateScale = true;
}

void (DSGM_DeinitObjectInstanceRotScale)(DSGM_ObjectInstance *me) {
	if(me->angle) {
		int rotset = ((void *)me->angle - (void *)&DSGM_rotations[me->screen][0]) / sizeof(DSGM_rotations[0][0]);
		DSGM_rotsetTracker[me->screen][rotset]--;
		DSGM_Debug("Freeing up one usage of rotset %d\n", rotset);
	}
}

void (DSGM_MoveObjectInstanceAtAngle)(DSGM_ObjectInstance *me, int angle) {
	if(me->bx >> me->bitshift != me->x || me->by >> me->bitshift != me->y) {
		me->bx = me->x << me->bitshift;
		me->by = me->y << me->bitshift;
	}
	
	me->bx += cosLerp(angle);
	me->by -= sinLerp(angle);
	
	me->x = me->bx >> me->bitshift;
	me->y = me->by >> me->bitshift;
}

void (DSGM_MoveObjectInstanceAtRotatedAngle)(DSGM_ObjectInstance *me) {
	if(me->bx >> me->bitshift != me->x || me->by >> me->bitshift != me->y) {
		me->bx = me->x << me->bitshift;
		me->by = me->y << me->bitshift;
	}
	
	me->bx += cosLerp(*me->angle);
	me->by -= sinLerp(*me->angle);
	
	me->x = me->bx >> me->bitshift;
	me->y = me->by >> me->bitshift;
}

void (DSGM_AnimateObjectInstance)(DSGM_ObjectInstance *me, int startFrame, int endFrame, int frequency) {
	me->animationTimer++;
	if(me->animationTimer == frequency) {
		if(me->frame > endFrame || me->frame < startFrame) me->frame = startFrame;
		if(me->frame < endFrame) me->frame++;
		else me->frame = startFrame;
		me->animationTimer = 0;
	}
}

void (DSGM_ReturnAnimateObjectInstance)(DSGM_ObjectInstance *me, int returnFrame, int startFrame, int endFrame, int frequency) {
	me->animationTimer++;
	if((me->animationTimer % (frequency / 2)) == 0) me->frame = returnFrame;
	if((me->animationTimer % frequency) == 0) {
		if(startFrame + me->animationTimer / frequency > endFrame) me->animationTimer = 0;
		me->frame = startFrame + me->animationTimer / frequency;
	}
}
