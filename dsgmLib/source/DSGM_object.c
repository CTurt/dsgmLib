#include "DSGM.h"

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
	group->objectInstanceCount = objectInstanceCount;
	if(objectInstanceCount > 0) {
		DSGM_Debug("Allocating %d object instance(s) on %s screen\n", objectInstanceCount, screen == DSGM_TOP ? "top" : "bottom");
		group->objectInstances = malloc(objectInstanceCount * sizeof(DSGM_ObjectInstance));
		DSGM_Debug("Address %p was given\n", group->objectInstances);
	}
	else group->objectInstances = NULL;
	va_start(properties, objectInstanceCount);
	for(i = 0; i < objectInstanceCount; i++) {
		// Needs to be updated for new DSGM_ObjectInstance struct
		group->objectInstances[i].object = object;
		group->objectInstances[i].screen = screen;
		group->objectInstances[i].x = va_arg(properties, int);
		group->objectInstances[i].y = va_arg(properties, int);
		group->objectInstances[i].spriteNumber = 0;
		group->objectInstances[i].frame = 0;
		group->objectInstances[i].animationTimer = 0;
		group->objectInstances[i].hFlip = false;
		group->objectInstances[i].vFlip = false;
		group->objectInstances[i].angle = NULL;
		group->objectInstances[i].hide = false;
		group->objectInstances[i].variables = malloc(object->customVariablesSize);
	}
	va_end(properties);
}

DSGM_ObjectGroup *DSGM_GetObjectGroupFull(DSGM_Room *room, u8 screen, DSGM_Object *object) {
	int i;
	for(i = 0; i < room->objectGroupCount[screen]; i++) {
		if(room->objectGroups[screen][i].objectInstanceCount > 0) {
			if(room->objectGroups[screen][i].objectInstances[0].object == object) return &room->objectGroups[screen][i];
		}
	}
	return NULL;
}

void (DSGM_AddCollisionEvent)(DSGM_Object *object, DSGM_Object *collider, DSGM_CollisionEventFunction function) {
	DSGM_Debug("Adding collision event\n");
	object->collisionEvents = realloc(object->collisionEvents, (object->collisionEventCount + 1) * sizeof(DSGM_CollisionEvent));
	object->collisionEvents[object->collisionEventCount].collider = collider;
	object->collisionEvents[object->collisionEventCount].function = function;
	object->collisionEventCount++;
}

inline bool DSGM_StylusOverObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance *me) {
	return DSGM_stylus.px >= me->x - room->view[me->screen].x && DSGM_stylus.px <= me->x - room->view[me->screen].x + DSGM_GetSpriteWidth(me->object->sprite) && DSGM_stylus.py >= me->y - room->view[me->screen].y && DSGM_stylus.py <= me->y - room->view[me->screen].y + DSGM_GetSpriteHeight(me->object->sprite);
}

inline bool DSGM_ObjectInstanceOnScreenFull(DSGM_Room *room, DSGM_ObjectInstance *me) {
	return ((!me->x - room->view[me->screen].x >= 255) && (!me->y - room->view[me->screen].y >= 191)) &&\
	((!me->x - room->view[me->screen].x <= -DSGM_GetSpriteWidth(me->object->sprite)) && (!me->y - room->view[me->screen].y <= -DSGM_GetSpriteHeight(me->object->sprite))) &&\
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
	return ((unsigned int)me->angle - (unsigned int)DSGM_rotations[me->screen]) / sizeof(me->angle);
}

void (DSGM_InitObjectInstanceRotScale)(DSGM_ObjectInstance *me) {
	int rotset = DSGM_NextFreeRotset(me->screen);
	me->angle = &DSGM_rotations[me->screen][rotset];
	me->rotationIndex = rotset;
	me->isSizeDouble = true;
	me->isRotateScale = true;
}

void (DSGM_InitSharedObjectInstanceRotScale)(DSGM_ObjectInstance *me, int rotset) {
	me->angle = &DSGM_rotations[me->screen][rotset];
	me->rotationIndex = rotset;
	me->isSizeDouble = true;
	me->isRotateScale = true;
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
