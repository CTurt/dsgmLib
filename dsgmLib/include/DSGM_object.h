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
	DSGM_Scale *scale;
	bool hide;
	
	DSGM_OAMStructLabel oam;
	DSGM_SpriteEntry;
} DSGM_ObjectInstanceBase;

typedef struct DSGM_objectInstance {
	DSGM_ObjectInstanceBase;
	struct {} *variables;
} DSGM_ObjectInstance;

typedef struct {
	DSGM_Object *object;
	DSGM_ObjectInstance *objectInstances;
	int objectInstanceCount;
} DSGM_ObjectGroup;

typedef struct {
	u8 screen;
	int ID;
	int groupN;
} DSGM_ObjectInstanceRelation;

#include "DSGM_room.h"

void DSGM_SetupObjectGroups(DSGM_Room *room, u8 screen, int objectGroupCount);
void DSGM_SetupObjectInstances(DSGM_ObjectGroup *group, DSGM_Object *object, u8 screen, int objectInstanceCount, ...);
void DSGM_RedistributeSpriteNumbers(DSGM_Room *room, u8 screen);
void DSGM_ActivateObjectInstance(DSGM_Room *room, DSGM_ObjectInstance *objectInstance);
DSGM_ObjectGroup *DSGM_GetObjectGroupFull(DSGM_Room *room, u8 screen, DSGM_Object *object);

#define DSGM_CreateObjectInstance(screen, x, y, object) DSGM_CreateObjectInstanceFull(&DSGM_Rooms[DSGM_currentRoom], (DSGM_ObjectInstance **)&me, screen, x, y, object)
DSGM_ObjectInstance *DSGM_CreateObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance **meP, u8 screen, int x, int y, DSGM_Object *object);

#define DSGM_DeleteObjectInstance(objectInstance) DSGM_DeleteObjectInstanceFull(&DSGM_Rooms[DSGM_currentRoom], (DSGM_ObjectInstance *)objectInstance)
void DSGM_DeleteObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance *objectInstance);

#define DSGM_GetObjectInstanceRelation(me) DSGM_GetObjectInstanceRelationFull(&DSGM_Rooms[DSGM_currentRoom], (DSGM_ObjectInstance *)me)
DSGM_ObjectInstanceRelation DSGM_GetObjectInstanceRelationFull(DSGM_Room *room, DSGM_ObjectInstance *me);

#define DSGM_GetMeFromObjectInstanceRelation(relation) DSGM_GetMeFromObjectInstanceRelationFull(&DSGM_Rooms[DSGM_currentRoom], me)
void *DSGM_GetMeFromObjectInstanceRelationFull(DSGM_Room *room, DSGM_ObjectInstanceRelation *relation);

#define DSGM_AddCollisionEvent(object, collider, function) DSGM_AddCollisionEvent(object, collider, (DSGM_CollisionEventFunction)function)
void (DSGM_AddCollisionEvent)(DSGM_Object *object, DSGM_Object *collider, DSGM_CollisionEventFunction function);

#define DSGM_StylusOverObjectInstance(me) DSGM_StylusOverObjectInstanceFull(&DSGM_Rooms[DSGM_currentRoom], (DSGM_ObjectInstance *)me)
inline bool DSGM_StylusOverObjectInstanceFull(DSGM_Room *room, DSGM_ObjectInstance *me);

#define DSGM_ObjectInstanceOnScreen(me) DSGM_ObjectInstanceOnScreenFull(&DSGM_Rooms[DSGM_currentRoom], (DSGM_ObjectInstance *)me)
inline bool DSGM_ObjectInstanceOnScreenFull(DSGM_Room *room, DSGM_ObjectInstance *me);

#define DSGM_ObjectInstanceCollision(me, collider) DSGM_ObjectInstanceCollision((DSGM_ObjectInstance *)me, (DSGM_ObjectInstance *)collider)
inline bool (DSGM_ObjectInstanceCollision)(DSGM_ObjectInstance *me, DSGM_ObjectInstance *collider);

#define DSGM_GetObjectInstanceRotset(me) DSGM_GetObjectInstanceRotset((DSGM_ObjectInstance *)me)
inline int (DSGM_GetObjectInstanceRotset)(DSGM_ObjectInstance *me);

#define DSGM_GetObjectInstanceID(me) DSGM_GetObjectInstanceIDFull(&DSGM_Rooms[DSGM_currentRoom], (DSGM_ObjectInstance *)me)
inline int DSGM_GetObjectInstanceIDFull(DSGM_Room *room, DSGM_ObjectInstance *me);

#define DSGM_InitObjectInstanceRotScale(me) DSGM_InitObjectInstanceRotScale((DSGM_ObjectInstance *)me)
void (DSGM_InitObjectInstanceRotScale)(DSGM_ObjectInstance *me);

#define DSGM_InitSharedObjectInstanceRotScale(me, rotset) DSGM_InitSharedObjectInstanceRotScale((DSGM_ObjectInstance *)me, rotset)
void (DSGM_InitSharedObjectInstanceRotScale)(DSGM_ObjectInstance *me, int rotset);

#define DSGM_AnimateObjectInstance(me, startFrame, endFrame, frequency) DSGM_AnimateObjectInstance((DSGM_ObjectInstance *)me, startFrame, endFrame, frequency)
void (DSGM_AnimateObjectInstance)(DSGM_ObjectInstance *me, int startFrame, int endFrame, int frequency);

#define DSGM_ReturnAnimateObjectInstance(me, returnFrame, startFrame, endFrame, frequency) DSGM_ReturnAnimateObjectInstance((DSGM_ObjectInstance *)me, returnFrame, startFrame, endFrame, frequency)
void (DSGM_ReturnAnimateObjectInstance)(DSGM_ObjectInstance *me, int returnFrame, int startFrame, int endFrame, int frequency);
