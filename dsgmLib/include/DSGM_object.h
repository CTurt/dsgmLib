#pragma once

#define DSGM_NO_EVENT NULL

struct DSGM_objectInstance;

typedef void (*DSGM_Event)(struct DSGM_objectInstance *me);
typedef void (*DSGM_CollisionEventFunction)(struct DSGM_objectInstance *me, struct DSGM_objectInstance *collider);

typedef struct {
	struct DSGM_object *collider;
	DSGM_CollisionEventFunction function;
} DSGM_CollisionEvent;

typedef struct DSGM_object {
	DSGM_Sprite *sprite;
	
	DSGM_Event create;
	DSGM_Event loop;
	DSGM_Event destroy;
	DSGM_Event touch;
	
	DSGM_CollisionEvent *collisionEvents;
	unsigned int collisionEventCount;
	
	size_t customVariablesSize;
} DSGM_Object;

typedef struct {
	DSGM_Object *object;
	u8 screen;
	int spriteNumber;
	int x;
	int y;
	u8 frame;
	int animationTimer;
	int *angle;
	bool hide;
	
	DSGM_OAMStructLabel oam;
	DSGM_SpriteEntry;
} DSGM_ObjectInstanceBase;

typedef struct DSGM_objectInstance {
	DSGM_ObjectInstanceBase;
	struct {} *variables;
} DSGM_ObjectInstance;

typedef struct {
	DSGM_ObjectInstance *objectInstances;
	int objectInstanceCount;
} DSGM_ObjectGroup;

#include "DSGM_room.h"

void DSGM_SetupObjectGroups(DSGM_Room *room, u8 screen, int objectGroupCount);
void DSGM_SetupObjectInstances(DSGM_ObjectGroup *group, DSGM_Object *object, u8 screen, int objectInstanceCount, ...);
DSGM_ObjectGroup *DSGM_GetObjectGroupFull(DSGM_Room *room, u8 screen, DSGM_Object *object);

#define DSGM_AddCollisionEvent(object, collider, function) DSGM_AddCollisionEvent(object, collider, (DSGM_CollisionEventFunction)function)
void (DSGM_AddCollisionEvent)(DSGM_Object *object, DSGM_Object *collider, DSGM_CollisionEventFunction function);

#define DSGM_StylusOverObjectInstance(me) DSGM_StylusOverObjectInstanceFull(&DSGM_Rooms[DSGM_currentRoom], (DSGM_ObjectInstance *)me)
inline bool DSGM_StylusOverObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance *me);

#define DSGM_ObjectInstanceOnScreen(me) DSGM_ObjectInstanceOnScreenFull(&DSGM_Rooms[DSGM_currentRoom], me)
inline bool DSGM_ObjectInstanceOnScreenFull(DSGM_Room *room, DSGM_ObjectInstance *me);

#define DSGM_ObjectInstanceCollision(me, collider) DSGM_ObjectInstanceCollision((DSGM_ObjectInstance *)me, (DSGM_ObjectInstance *)collider)
inline bool (DSGM_ObjectInstanceCollision)(DSGM_ObjectInstance *me, DSGM_ObjectInstance *collider);

#define DSGM_GetObjectInstanceRotset(me) DSGM_GetObjectInstanceRotset((DSGM_ObjectInstance *)me)
inline int (DSGM_GetObjectInstanceRotset)(DSGM_ObjectInstance *me);

#define DSGM_InitObjectInstanceRotation(me) DSGM_InitObjectInstanceRotation((DSGM_ObjectInstance *)me)
void (DSGM_InitObjectInstanceRotation)(DSGM_ObjectInstance *me);

#define DSGM_InitSharedObjectInstanceRotation(me, rotset) DSGM_InitSharedObjectInstanceRotation((DSGM_ObjectInstance *)me, rotset)
void (DSGM_InitSharedObjectInstanceRotation)(DSGM_ObjectInstance *me, int rotset);

#define DSGM_AnimateObjectInstance(me, startFrame, endFrame, frequency) DSGM_AnimateObjectInstance((DSGM_ObjectInstance *)me, startFrame, endFrame, frequency)
void (DSGM_AnimateObjectInstance)(DSGM_ObjectInstance *me, int startFrame, int endFrame, int frequency);

#define DSGM_ReturnAnimateObjectInstance(me, returnFrame, startFrame, endFrame, frequency) DSGM_ReturnAnimateObjectInstance((DSGM_ObjectInstance *)me, returnFrame, startFrame, endFrame, frequency)
void (DSGM_ReturnAnimateObjectInstance)(DSGM_ObjectInstance *me, int returnFrame, int startFrame, int endFrame, int frequency);
